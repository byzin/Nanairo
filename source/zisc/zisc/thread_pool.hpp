/*!
  \file thread_pool.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef _ZISC_THREAD_POOL_HPP_
#define _ZISC_THREAD_POOL_HPP_

// Standard C++ library
#include <atomic>
#include <cstddef>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <utility>
// Zisc
#include "zisc/zisc_config.hpp"

namespace zisc {

// Forward declaration
template <uint, uint> class AlignedMemoryPool;

/*!
  \brief 
  \details
  ThreadPool class is designed based on the following URL
  http://progsch.net/wordpress/?p=81 and https//github.com/progschj/ThreadPool.
  */
class ThreadPool
{
 public:
  //! Create threads as many CPU threads as
  ThreadPool();

  //! Create threads
  ThreadPool(const uint num_of_threads);

  //! Terminate threads
  ~ThreadPool();


  //! Add task
  template <typename Type>
  std::future<Type> enqueue(std::function<Type (void)>&& task);

  //! Add task and a thread number is delivered
  template <typename Type>
  std::future<Type> enqueue(std::function<Type (int)>&& task);

  //! Get the number of logical cores
  static uint logicalCores();

  //! Execute loop task in parallel
  template <typename Iterator>
  std::future<void> loop(std::function<void (Iterator)>&& task, 
                         Iterator begin, 
                         Iterator end);

  //! Execute loop task in parallel
  template <typename Iterator>
  std::future<void> loop(std::function<void (int, Iterator)>&& task, 
                         Iterator begin, 
                         Iterator end);

  //! Execute loop task in parallel
  template <typename Iterator, uint kSize, uint kAligned>
  std::future<void> loop(std::function<void (Iterator)>&& task, 
                         Iterator begin, 
                         Iterator end,
                         AlignedMemoryPool<kSize, kAligned>& memory_pool);

  //! Execute loop task in parallel
  template <typename Iterator, uint kSize, uint kAligned>
  std::future<void> loop(std::function<void (int, Iterator)>&& task, 
                         Iterator begin, 
                         Iterator end,
                         AlignedMemoryPool<kSize, kAligned>& memory_pool);

  //! Get the number of threads
  uint numOfThreads() const;

 private:
  template <typename Task>
  struct TaskManager
  {
    TaskManager(Task& task, const uint distance) : finish_loop_{[](){}},
                                                   task_{std::move(task)},
                                                   counter_{distance} {}
    static void operator delete(void* /* memory */) {}
    static void* operator new(const std::size_t /* size */, void* memory) 
    {
      return memory;
    }

    std::packaged_task<void ()> finish_loop_;
    Task task_;
    std::atomic<uint> counter_;
  };

  //! Create worker threads
  void createWorkers(const uint num_of_threads);

  //! Append a task to the task queue
  template <typename ReturnType, typename ...Types>
  std::future<ReturnType> enqueueTask(std::function<ReturnType (Types...)>& task);

  //! Execute loop task in parallel
  template <typename Task, typename Iterator>
  std::future<void> loopTask(Task& task, Iterator begin, Iterator end);

  //! Execute loop task in parallel
  template <typename Task, typename Iterator, uint kSize, uint kAligned>
  std::future<void> loopTask(Task& task,
                             Iterator begin,
                             Iterator end,
                             AlignedMemoryPool<kSize, kAligned>& memory_pool);


  std::mutex queue_mutex_;
  std::condition_variable condition_;
  std::queue<std::function<void (int)>> task_queue_;
  std::vector<std::thread> workers_;
  bool is_finish_;
};

} // namespace zisc

#include "thread_pool-inl.hpp"

#endif // _ZISC_THREAD_POOL_HPP_
