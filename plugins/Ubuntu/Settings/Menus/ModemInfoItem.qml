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

BaseMenu {
    id: menu

    property alias statusIcon: statusIcon.name
    property alias statusText: labelStatus.text
    property alias connectivityIcon: iconConnectivity.name
    property alias simIdentifierText: labelSimIdentifier.text
    property bool locked : false
    property bool roaming: false
    signal unlock

    menuHeight: slotsLayout.height

    StyledSlotsLayout {
        id: slotsLayout
        objectName: "menuItemInfoSlotsLayout"
        style: menuStyle

        LayoutMirroring.enabled: Qt.application.layoutDirection == Qt.RightToLeft
        LayoutMirroring.childrenInherit: true

        mainSlot: ColumnLayout {
            spacing: units.gu(0.5)

            Label {
                id: labelSimIdentifier
                elide: Text.ElideRight
                visible: text !== ""
                opacity: menu.locked ? 0.6 : 1.0
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
                    elide: Text.ElideRight
                    opacity: 0.6
                    color: menu.foregroundColor
                    font.pixelSize: menuStyle.fontSize
                }

                Row {
                    spacing: units.gu(0.5)
                    height: parent.height
                    Icon {
                        id: statusIcon
                        color: menuStyle.iconColor

                        height: menuStyle.iconSize
                        width: height

                        visible: name !== ""
                    }

                    Icon {
                        id: iconConnectivity
                        color: menuStyle.iconColor

                        width: menuStyle.iconSize
                        height: width

                        visible: name !== ""
                    }
                }

                Row {
                    spacing: units.gu(0.5)
                    height: parent.height

                    Label {
                        id: labelRoaming
                        visible: menu.roaming
                        elide: Text.ElideRight
                        text: i18n.dtr("ubuntu-settings-components", "Roaming")
                        font.pixelSize: menuStyle.subtitleFontSize
                        color: menu.foregroundColor
                        opacity: 0.6
                    }

                    Icon {
                        id: iconRoaming
                        color: menuStyle.iconColor
                        visible: menu.roaming

                        height: menuStyle.iconSize
                        width: height

                        name: "network-cellular-roaming"
                    }
                }
            }

            Button {
                id: buttonUnlock
                objectName: "buttonUnlockSim"
                visible: menu.locked
                color: menuStyle.buttonColor

                text: i18n.dtr("ubuntu-settings-components", "Unlock SIM")
                font.pixelSize: menuStyle.buttonFontSize
                Layout.preferredWidth: implicitWidth + units.gu(5)
                Layout.preferredHeight: menuStyle.buttonHeight
                height: menuStyle.buttonHeight

                onTriggered: menu.unlock()
            }
        }
    }
}
