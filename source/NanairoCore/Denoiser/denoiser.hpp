/*!
  \file denoiser.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_DENOISER_HPP
#define NANAIRO_DENOISER_HPP

// Zisc
#include "zisc/memory_resource.hpp"
#include "zisc/fnv_1a_hash_engine.hpp"
#include "zisc/function_reference.hpp"
#include "zisc/unique_memory_pointer.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

// Forward declaration
class SampleStatistics;
class System;

//! \addtogroup Core
//! \{

/*!
  */
enum class DenoiserType : uint32
{
  kBayesianCollaborative      = zisc::Fnv1aHash32::hash("BayesianCollaborative"),
};

/*!
  */
class Denoiser
{
 public:
  //! Initialize a denoiser
  Denoiser(const SettingNodeBase* settings) noexcept;

  //! Finalize the denoiser
  virtual ~Denoiser() noexcept;


  //! Denoise input value
  virtual void denoise(System& system,
                       const uint32 cycle,
                       SampleStatistics* statistics) const noexcept = 0;

  //! Make a denoiser
  static zisc::UniqueMemoryPointer<Denoiser> makeDenoiser(
      System& system,
      const SettingNodeBase* settings) noexcept;

  //! Set a progress callback
  void setProgressCallback(const zisc::FunctionReference<void (double)>& callback)
      noexcept;

 protected:
  //! Return a progress callback
  const zisc::FunctionReference<void (double)>& progressCallback() const noexcept;

 private:
  //! Initialize the denoiser
  void initialize(const SettingNodeBase* settings) noexcept;


  zisc::FunctionReference<void (double)> progress_callback_;
};

//! \}

} // namespace nanairo

#include "denoiser-inl.hpp"

#endif // NANAIRO_DENOISER_HPP
