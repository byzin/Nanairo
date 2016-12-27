/*!
  \file nanairoManager.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6

QtObject {

  signal finished()

  signal started()

  signal stopping()

  signal updated(string fps, string cycle, string time)

  signal cameraEventHandled(matrix4x4 matrix)

  signal previewEvent(int type, int axis_type, int value)

  function currentWorkingDur() {
    return ".";
  }

  function getFileName(filePath) {
    return filePath.toString();
  }

  function idealThreadCount() {
    return 1;
  }

  function invokeRendering() {
  }

  function invokePreviewing() {
  }

  function isDebugMode() {
    return true;
  }

  function makeDir(dirPath) {
  }

  function random() {
    return 1;
  }

  function loadSceneData(filePath) {
    var sceneData = {};
    return sceneData;
  }

  function saveSceneData(filePath, sceneData) {
  }

  function toAbsoluteFileUrl(relativePath) {
    return Qt.resolvedUrl(relativePath);
  }

  function toRelativeFilePath(absolutePath) {
    return absolutePath.toString();
  }
}
