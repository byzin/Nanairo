/*!
  \file nanairo_definitions.js
  \author Sho Ikeda

  Copyright (c) 2015 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

.pragma library

// Message
var warningMessageType = 0
var errorMessageType = 1

// Utility functions
function int32Max()
{
  return Math.pow(2, 31) - 1
}
