/*
 * Copyright (C) 2016 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import Qt.labs.folderlistmodel 2.1
import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItems
import Ubuntu.Components.Popups 1.3
import Ubuntu.Settings.Components 0.1

Dialog {
    id: root
    objectName: "filePicker"

    property string currentFilePath: ""

    signal accept(string path)
    signal reject()

    function hideFunc() {
        FilePickerProperties.lastFolder = fsModel.folder
        currentFilePath = ""
    }

    function rejectFunc() {
        hideFunc()
        root.reject()
    }

    function acceptFunc() {
        var path = currentFilePath
        hideFunc()
        root.accept(path)
    }

    property var fsModel: FolderListModel {
        showDirs: true
        showFiles: true
        showHidden: true
        showDirsFirst: true
        showDotAndDotDot: false
        showOnlyReadable: false
        sortField: FolderListModel.Name

        folder: (FilePickerProperties.lastFolder === "") ?
            FilePickerHelper.homeDirUrl() :
            FilePickerProperties.lastFolder
    }

    ColumnLayout {
        spacing: units.gu(1)
        anchors {
            left: parent.left
            right: parent.right
        }
        height: root.height - units.gu(10)

        RowLayout {
            anchors { left: parent.left; right: parent.right }
            spacing: units.gu(1)

            Icon {
                objectName: "filePickerBackButton"
                name: "back"
                Layout.alignment: Qt.AlignVCenter
                width: units.gu(2.5)
                height: width

                enabled: (FilePickerHelper.pathFromUrl(fsModel.folder)
                          !== FilePickerHelper.rootPath())
                opacity: enabled ? 1 : 0.5

                MouseArea {
                    anchors {
                        fill: parent
                        margins: -units.gu(2)
                    }
                    onClicked: {
                        if (parent.enabled) {
                            fsModel.folder = fsModel.parentFolder
                        }
                    }
                }
            }

            Label {
                objectName: "filePickerBreadcrumb"
                Layout.alignment: Qt.AlignVCenter
                text: {
                    var prettyTree = [
                        i18n.dtr("ubuntu-settings-components", "Device")
                    ];
                    var paths = FilePickerHelper.pathsFromUrl(fsModel.folder);
                    for (var i = 0; i < paths.length; i++) {
                        prettyTree.push(paths[i]);
                    }
                    return i18n.dtr("ubuntu-settings-components",
                                    "%1 (%2 file)",
                                    "%1 (%2 files)",
                                    fsModel.count
                            ).arg(prettyTree.join("/"))
                             .arg(fsModel.count)
                }
                Layout.fillWidth: true
                elide: Text.ElideMiddle
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: theme.palette.normal.background

            border {
                width: 1
                color: theme.palette.normal.overlaySecondaryText
            }

            ListView {
                id: list
                objectName: "filePickerList"
                anchors.fill: parent
                clip: true
                model: fsModel

                delegate: ListItem {
                    objectName: "filePickerFileItem_" + index
                    height: fileLayout.height + divider.height
                    color: (filePath === currentFilePath) ? highlightColor : "transparent"

                    onClicked: {
                        if (fileIsDir) {
                            fsModel.folder = fileURL
                        } else {
                            currentFilePath = filePath
                        }
                    }

                    ListItemLayout {
                        id: fileLayout

                        title.text: fileName
                        title.elide: Text.ElideMiddle
                        title.wrapMode: Text.NoWrap
                        subtitle.text: fileIsDir ?
                            "" : FilePickerHelper.formatSize(fileSize)

                        Icon {
                            name: fileIsDir ? "folder" : "empty"
                            SlotsLayout.position: SlotsLayout.Leading;
                            width: units.gu(4)
                        }
                    }
                }
            }

            Scrollbar {
                flickableItem: list
                align: Qt.AlignTrailing
            }
        }

        RowLayout {
            spacing: units.gu(1)

            Button {
                objectName: "filePickerCancel"
                Layout.fillWidth: true
                text: i18n.dtr("ubuntu-settings-components", "Cancel")
                onClicked: rejectFunc()
            }

            Button {
                objectName: "filePickerAccept"
                Layout.fillWidth: true
                enabled: currentFilePath !== ""
                text: i18n.dtr("ubuntu-settings-components", "Accept")
                onClicked: acceptFunc()
                color: theme.palette.normal.positive
            }
        }
    }
}
