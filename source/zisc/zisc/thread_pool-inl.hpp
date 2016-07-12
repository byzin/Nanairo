/*!
  \file thread_pool-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef ZISC_THREAD_POOL_INL_HPP
#define ZISC_THREAD_POOL_INL_HPP

#include "thread_pool.hpp"
// Standard C++ library
#include <atomic>
#include <cmath>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <thread>
#include <type_traits>
#include <queue>
#include <utility>
// Zisc
#include "aligned_memory_pool.hpp"
#include "type_traits.hpp"
#include "utility.hpp"
#include "zisc/zisc_config.hpp"

namespace zisc {

/*!
  \details
  No detailed.
  */
inline
ThreadPool::ThreadPool() noexcept :
    ThreadPool(std::thread::hardware_concurrency())
{
}

/*!
  \details
  No detailed.
  */
inline
ThreadPool::ThreadPool(const uint num_of_threads) noexcept :
    is_finish_{false}
{
  createWorkers(num_of_threads);
}

/*!
  \details
  No detailed.
  */
inline
ThreadPool::~ThreadPool()
{
  {
    std::unique_lock<std::mutex> lock{queue_mutex_};
    is_finish_ = true;
  }
  condition_.notify_all();
  for (auto& worker : workers_)
    worker.join();
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
std::future<Type> ThreadPool::enqueue(std::function<Type (void)>&& task) noexcept
{
  return enqueueTask(task);
}

/*!
  \details
  No detailed.
  */
template <typename Type> inline
std::future<Type> ThreadPool::enqueue(std::function<Type (int)>&& task) noexcept
{
  return enqueueTask(task);
}

/*!
  \details
  No detailed.
  */
inline
uint ThreadPool::logicalCores() noexcept
{
  return cast<uint>(std::thread::hardware_concurrency());
}

/*!
  \details
  No detailed.
  */
template <typename Iterator> inline
std::future<void> ThreadPool::loop(std::function<void (Iterator)>&& task, 
                                   Iterator begin, 
                                   Iterator end) noexcept
{
  return loopTask(task, begin, end);
}

/*!
  \details
  No detailed.
  */
template <typename Iterator> inline
std::future<void> ThreadPool::loop(std::function<void (int, Iterator)>&& task, 
                                   Iterator begin, 
                                   Iterator end) noexcept
{
  return loopTask(task, begin, end);
}

/*!
  \details
  No detailed.
  */
template <typename Iterator, uint kSize, uint kAlignment> inline
std::future<void> ThreadPool::loop(
    std::function<void (Iterator)>&& task, 
    Iterator begin, 
    Iterator end,
    AlignedMemoryPool<kSize, kAlignment>& memory_pool) noexcept
{
  return loopTask(task, begin, end, memory_pool);
}

/*!
  \details
  No detailed.
  */
template <typename Iterator, uint kSize, uint kAlignment> inline
std::future<void> ThreadPool::loop(
    std::function<void (int, Iterator)>&& task, 
    Iterator begin, 
    Iterator end,
    AlignedMemoryPool<kSize, kAlignment>& memory_pool) noexcept
{
  return loopTask(task, begin, end, memory_pool);
}

namespace zisc_thread_pool {

//! Process a task
template <typename Type> inline
void processTask(std::packaged_task<Type (void)>& task, 
                 const int /* thread_number */) noexcept
{
  task();
}

//! Process a task
template <typename Type> inline
void processTask(std::packaged_task<Type (int)>& task, 
                 const int thread_number) noexcept
{
  task(thread_number);
}
 
//! Process a task
template <typename Iterator> inline
void processTask(std::function<void (Iterator)>& task, 
                 const int /* thread_number */, 
                 Iterator iterator) noexcept
{
  task(iterator);
}

//! Process a task
template <typename Iterator> inline
void processTask(std::function<void (int, Iterator)>& task, 
                 const int thread_number, 
                 Iterator iterator) noexcept
{
  task(thread_number, iterator);
}

//! Return the distance of two iterators
template <typename Iterator> inline
uint distance(Iterator begin, 
              Iterator end, 
              EnableIfIterator<Iterator> = kEnabler) noexcept
{
  return cast<uint>(std::distance(begin, end));
}

//! Return the distance of two iterators
template <typename Iterator> inline
uint distance(Iterator begin, 
              Iterator end, 
              EnableIfInteger<Iterator> = kEnabler) noexcept
{
  return cast<uint>(end - begin);
}

} // namespace zisc_thread_pool

/*!
  \details
  No detailed.
  */
template <typename ReturnType, typename ...Types> inline
std::future<ReturnType> ThreadPool::enqueueTask(
    std::function<ReturnType (Types...)>& task) noexcept
{
  using PackagedTask = std::packaged_task<ReturnType (Types...)>;

  auto shared_task = std::make_shared<PackagedTask>(std::move(task));
  std::function<void (int)> wrapped_task{[shared_task](const int thread_number)
  {
    zisc_thread_pool::processTask(*shared_task, thread_number);
  }};

  auto result = shared_task->get_future();
  {
    std::unique_lock<std::mutex> lock{queue_mutex_};
    task_queue_.emplace(std::move(wrapped_task));
  }
  condition_.notify_one();

  return result;
}

/*!
  */
inline
bool ThreadPool::isFinished() const noexcept
{
 return (is_finish_ && task_queue_.empty());
}

/*!
  \details
  No detailed.
  */
template <typename Task, typename Iterator> inline
std::future<void> ThreadPool::loopTask(Task& task, 
                                       Iterator begin, 
                                       Iterator end) noexcept
{
  const uint distance = zisc_thread_pool::distance(begin, end);

  auto shared_task = new Task{std::move(task)};
  auto finish_loop = new std::packaged_task<void ()>{[](){}};
  auto shared_counter = new std::atomic<uint>{distance};

  auto finish = [shared_task, finish_loop, shared_counter]()
  {
    (*finish_loop)();
    delete shared_counter;
    delete finish_loop;
    delete shared_task;
  };

  auto result = finish_loop->get_future();

  if (begin == end) {
    finish();
    return result;
  }

  {
    std::unique_lock<std::mutex> lock{queue_mutex_};
    for (auto iterator = begin; iterator != end; ++iterator) {
      std::function<void (int)> wrapped_task{
      [shared_task, shared_counter, iterator, finish](const int thread_number)
      {
        zisc_thread_pool::processTask(*shared_task, thread_number, iterator);
        const uint count = --(*shared_counter);
        if (count == 0)
          finish();
      }};
      task_queue_.emplace(std::move(wrapped_task));
    }
  }
  condition_.notify_all();

  return result;
}

/*!
  \details
  No detailed.
  */
template <typename Task, typename Iterator, uint kSize, uint kAlignment> inline
std::future<void> ThreadPool::loopTask(
    Task& task, 
    Iterator begin, 
    Iterator end,
    AlignedMemoryPool<kSize, kAlignment>& memory_pool) noexcept
{
  using Manager = TaskManager<Task>;

  const auto distance = cast<uint>(end - begin);

  auto task_manager = memory_pool.template allocate<Manager>(task, distance);
  auto result = task_manager->finish_loop_.get_future();

  if (begin == end) {
    task_manager->finish_loop_();
    delete task_manager;
    return result;
  }

  {
    std::function<void (int)> wrapped_task;
    std::unique_lock<std::mutex> lock{queue_mutex_};
    for (auto iterator = begin; iterator != end; ++iterator) {
      wrapped_task = [task_manager, iterator](const int thread_number)
      {
        zisc_thread_pool::processTask(task_manager->task_, thread_number, iterator);
        const auto count = --task_manager->counter_;
        if (count == 0) {
          task_manager->finish_loop_();
          delete task_manager;
        }
      };
      task_queue_.emplace(std::move(wrapped_task));
    }
  }
  condition_.notify_all();

  return result;
}

/*!
  \details
  No detailed.

  \return The number of worker threads
  */
inline
uint ThreadPool::numOfThreads() const noexcept
{
  return cast<uint>(workers_.size());
}

/*!
  \details
  No detailed.
  */
inline
void ThreadPool::createWorkers(const uint num_of_threads) noexcept
{
  uint threads = (num_of_threads == 0)
      ? std::thread::hardware_concurrency()
      : num_of_threads;
  workers_.reserve(threads);
  for (int thread_number = 0; thread_number < cast<int>(threads); ++thread_number) {
    auto work = [this, thread_number]()
    {
      while (!isFinished()) {
        std::unique_lock<std::mutex> lock{queue_mutex_};
        while (!is_finish_ && task_queue_.empty())
          condition_.wait(lock);
        if (!isFinished()) {
          std::function<void (int)> task{std::move(task_queue_.front())};
          task_queue_.pop();
          lock.unlock();
          task(thread_number);
        }
      }
    };
    workers_.emplace_back(work);
  }
}

} // namespace zisc

#endif // ZISC_THREAD_POOL_INL_HPP
