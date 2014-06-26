/*
 * Copyright 2014 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by Nicholas Dedekind <nick.dedekind@gmail.com>
 */

import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem
import Ubuntu.Settings.Components 0.1 as USC
import Ubuntu.Settings.Menus 0.1 as USM
import QtQuick.Layouts 1.1

ListItem.Empty {
    id: menu

    property alias iconSource: icon.source
    property alias text: label.text
    property alias stateText: stateLabel.text
    property alias progress: progressBar.value
    property bool active: false

    property alias maximum: progressBar.maximumValue

    implicitHeight: row.height + units.gu(2)

    RowLayout {
        id: row
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
            leftMargin: menu.__contentsMargins
            rightMargin: menu.__contentsMargins
        }

        UbuntuShape {
            id: imageShape
            width: units.gu(7)
            height: width
            Layout.alignment: Qt.AlignVCenter

            image: Image {
                objectName: "icon"
                id: icon
            }
        }

        ColumnLayout {
            spacing: units.gu(0.5)
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                id: label
                objectName: "text"
                Layout.fillWidth: true
            }

            ProgressBar {
                id: progressBar
                objectName: "progress"
                visible: active
                value: 0.0

                Layout.maximumHeight: units.gu(2)
                Layout.fillWidth: true
            }

            Label {
                id: stateLabel
                objectName: "stateText"
                Layout.fillWidth: true
                visible: active
            }
        }
    }
}
