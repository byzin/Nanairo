/*!
  \file path_state.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_PATH_STATE_HPP
#define NANAIRO_PATH_STATE_HPP

// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Sampling/Sampler/sampler.hpp"

namespace nanairo {

//! \addtogroup Core
//! \{

/*!
  */
class PathState
{
 public:
  //! Initialize a path state
  PathState() noexcept;

  //! Initialize a path state
  PathState(const uint32 s) noexcept;


  //! Increment a length of a path
  void incrementLength() noexcept;

  //! Return the current dimension of path sample
  uint32 dimension() const noexcept;

  //! Return a length of a path
  uint32 length() const noexcept;

  //! Return a sample
  uint32 sample() const noexcept;

  //! Set the current dimension of path sample
  void setDimension(const uint32 d) noexcept;

  //! Set the current dimension of path sample
  void setDimension(const SampleDimension d) noexcept;

  //! Set a sample
  void setLength(const uint32 l) noexcept;

  //! Set a sample
  void setSample(const uint32 s) noexcept;

 private:
  uint32 sample_ = 0;
  uint32 length_ = 0;
  uint32 dimension_ = 0;
};

//! \}

} // namespace nanairo

#include "path_state-inl.hpp"

#endif // NANAIRO_PATH_STATE_HPP
