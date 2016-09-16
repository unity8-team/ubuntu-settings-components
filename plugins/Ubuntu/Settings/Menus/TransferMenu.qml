/*
 * Copyright 2014-2016 Canonical Ltd.
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
 * Author: Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Settings.Components 0.1

BaseMenu {
    id: menu

    property alias iconSource: icon.source
    property alias text: label.text
    property alias stateText: stateLabel.text
    property alias progress: progressBar.value
    property alias maximum: progressBar.maximumValue
    property bool active: false

    height: slotsLayout.height + (divider.visible ? divider.height : 0)

    SlotsLayout {
        id: slotsLayout
        objectName: "transferMenuSlotsLayout"

        UbuntuShapeForItem {
            SlotsLayout.position: SlotsLayout.Leading
            width: units.gu(6)
            height: units.gu(6)

            image: icon
            Icon {
                id: icon
                objectName: "icon"

                color: {
                    if (String(source).match(/^image:\/\/theme/)) {
                        return theme.palette.normal.backgroundText;
                    }
                    return Qt.rgba(0.0, 0.0, 0.0, 0.0);
                }
            }
        }

        mainSlot: ColumnLayout {
            spacing: units.gu(0.5)

            Label {
                id: label
                objectName: "text"
                Layout.fillWidth: true

                elide: Text.ElideRight
                maximumLineCount: 1
                font.weight: Font.DemiBold
            }

            ProgressBar {
                id: progressBar
                objectName: "progress"
                visible: menu.active
                value: 0.0
                showProgressPercentage: false

                Layout.preferredHeight: units.gu(1)
                Layout.fillWidth: true
            }

            Label {
                id: stateLabel
                objectName: "stateText"
                Layout.fillWidth: true
                visible: menu.active

                fontSize: "x-small"
                elide: Text.ElideRight
                maximumLineCount: 1
            }
        }
    }
}
