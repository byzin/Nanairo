/*!
  \file spectra_image_interface-inl.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SPECTRA_IMAGE_INTERFACE_INL_HPP
#define NANAIRO_SPECTRA_IMAGE_INTERFACE_INL_HPP

#include "spectra_image_interface.hpp"
// Zisc
#include "zisc/error.hpp"
#include "zisc/point.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"

namespace nanairo {

/*!
  */
inline
void SpectraImageInterface::addContribution(
    const uint x,
    const uint y,
    const SampledSpectra& contribution) noexcept
{
  const uint index = toIndex(x, y);
  addContribution(index, contribution);
}

/*!
  */
inline
void SpectraImageInterface::addContribution(
    const Index2d& index,
    const SampledSpectra& contribution) noexcept
{
  const uint i = toIndex(index[0], index[1]);
  addContribution(i, contribution);
}

/*!
  \details
  No detailed.
  */
inline
uint SpectraImageInterface::heightResolution() const noexcept
{
  const auto& r = resolution();
  return zisc::cast<uint>(r[1]);
}

/*!
  */
inline
const Index2d& SpectraImageInterface::resolution() const noexcept
{
  return resolution_;
}

/*!
  \details
  No detailed.
  */
inline
uint SpectraImageInterface::widthResolution() const noexcept
{
  const auto& r = resolution();
  return zisc::cast<uint>(r[0]);
}

/*!
  */
inline
uint SpectraImageInterface::toIndex(const uint x, const uint y) const noexcept
{
  const uint w = widthResolution();
  const uint index = (w * y) + x;
  return index;
}

} // namespace nanairo

#endif // NANAIRO_SPECTRA_IMAGE_INTERFACE_INL_HPP
