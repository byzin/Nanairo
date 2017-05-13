/*!
  \file NFileDialog.qml
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

import QtQuick 2.6
import QtQuick.Controls 1.5
import QtQuick.Dialogs 1.2

FileDialog {
  id: file_dialog_item

  function openDialog() {
    folder = nanairoManager.currentWorkingDir()
    open();
  }
}
