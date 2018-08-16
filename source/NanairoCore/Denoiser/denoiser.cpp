/*!
  \file denoiser.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "denoiser.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/function_reference.hpp"
#include "zisc/memory_resource.hpp"
#include "zisc/unique_memory_pointer.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "bayesian_collaborative_denoiser.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Sampling/sample_statistics.hpp"
#include "NanairoCore/Setting/system_setting_node.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"

namespace nanairo {

/*!
  */
Denoiser::Denoiser(const SettingNodeBase* settings) noexcept
{
  initialize(settings);
}

/*!
  */
Denoiser::~Denoiser() noexcept
{
}

/*!
  */
void Denoiser::initialize(const SettingNodeBase* /* settings */) noexcept
{
}

/*!
  */
zisc::UniqueMemoryPointer<Denoiser> Denoiser::makeDenoiser(
    System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto system_settings = castNode<SystemSettingNode>(settings);
  auto& statistics_flag = system.sampleStatisticsFlag();

  auto pos = zisc::cast<std::size_t>(SampleStatistics::Type::kDenoisedExpectedValue);
  statistics_flag.set(pos, true);

  zisc::UniqueMemoryPointer<Denoiser> denoiser;
  auto data_resource = &system.dataMemoryManager();
  switch (system_settings->denoiserType()) {
   case DenoiserType::kBayesianCollaborative: {
    denoiser = zisc::UniqueMemoryPointer<BayesianCollaborativeDenoiser>::make(
      data_resource,
      settings);
    pos = zisc::cast<std::size_t>(SampleStatistics::Type::kVariance);
    statistics_flag.set(pos, true);
    pos = zisc::cast<std::size_t>(SampleStatistics::Type::kBayesianCollaborativeValues);
    statistics_flag.set(pos, true);
    break;
   }
   default: {
    zisc::raiseError("DenoiserError: Unsupported type is specified.");
   }
  }
  return denoiser;
}

/*!
  */
void Denoiser::setProgressCallback(
    const zisc::FunctionReference<void (double)>& callback) noexcept
{
  progress_callback_ = callback;
}

} // namespace nanairo
