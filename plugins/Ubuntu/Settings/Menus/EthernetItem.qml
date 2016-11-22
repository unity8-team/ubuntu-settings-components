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
 * Author: Pete Woods <pete.woods@canonical.com>
 */

import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Settings.Menus.Style 0.1

BaseMenu {
    id: menu
    property alias statusIcon: statusIcon.name
    property alias statusText: labelStatus.text
    property alias name: labelName.text
    property alias checked: switcher.checked

    menuHeight: slotsLayout.height

    StyledSlotsLayout {
        id: slotsLayout
        objectName: "ethernetSlotsLayout"
        style: menuStyle

        mainSlot: ColumnLayout {
            spacing: units.gu(0.5)

            Label {
                id: labelName
                objectName: "labelName"
                elide: Text.ElideRight
                color: menu.foregroundColor
                font.bold: true
                font.pixelSize: menuStyle.fontSize
            }

            Row {
                id: statusRow
                spacing: units.gu(1)
                height: labelStatus.height

                Label {
                    id: labelStatus
                    objectName: "labelStatus"
                    elide: Text.ElideRight
                    opacity: 0.6
                    color: menu.foregroundColor
                    font.pixelSize: menuStyle.fontSize
                }

                Icon {
                    id: statusIcon
                    objectName: "statusIcon"
                    color: menuStyle.iconColor

                    height: menuStyle.iconSize
                    width: height

                    visible: name !== ""
                }
            }
        }

        Switch {
            id: switcher
            objectName: "switcher"
            onClicked: menu.triggered(checked)

            SlotsLayout.position: SlotsLayout.Trailing
        }
    }
}
