/*!
  \file nanairoManager.qml
  \author Sho Ikeda

  Copyright (c) 2015-2018 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.11
import "scene.js" as Scene

QtObject {
  function currentWorkingDir() {
    return Qt.resolvedUrl(".").toString();
  }

  function generateSeedRandomly() {
    return 0;
  }

  function getFileName(filePath) {
    return filePath;
  }

  function getIdealThreadCount() {
    return 1;
  }

  function getAbsolutePath(relativePath) {
    return Qt.resolvedUrl(relativePath);
  }

  function getDefaultFixedFontFamily() {
    return "Inconsolata";
  }

  function getDefaultFontFamily() {
    return "Open Sans";
  }

  function getRelativePath(absolutePath) {
    return absolutePath.toString();
  }

  function isDebugMode() {
    return true;
  }

  function invokeRendering(sceneData, isPreviewing) {
    started();
  }

  function loadSceneData(filePath) {
    return Scene.getCornellBoxScene();
  }

  function saveSceneData(filePath, sceneData) {
  }

  onStopRendering: finished()

  signal finished()

  signal previewEvent(int transformationEventType,
                      int axisEventType,
                      int value)

  signal started()

  signal stopRendering()

  signal notifyOfRenderingProgress(double progress, string status)
}
