/*!
  \file nanairo.js
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */


.pragma library

function mergeDataList(dataList) {
  var sceneData = {};
  for (var i = 0; i < dataList.length; ++i) {
    var data = dataList[i];
    for (var key in data) {
      sceneData[key] = data[key];
    }
  }
  return sceneData;
}
