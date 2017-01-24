/*
 * Copyright 2013-2016 Canonical Ltd.
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
 * Authors:
 *      Renato Araujo Oliveira Filho <renato@canonical.com>
 *      Olivier Tilloy <olivier.tilloy@canonical.com>
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Settings.Components 0.1 as USC

SimpleMessageMenu {
    id: menu

    property alias actionEnabled: actionButton.enabled
    property alias actionButtonText: actionButton.text

    property alias replyEnabled: reply.activateEnabled
    property alias replyButtonText: reply.buttonText
    property alias replyHintText: reply.textHint
    property alias replyExpanded: reply.visible

    signal actionActivated
    signal replied(string value)

    footer: Item {
        id: buttons

        implicitHeight: layout.height
        anchors { right: parent.right; left: parent.left }

        Column {
            id: layout
            anchors {
                left: parent.left
                right: parent.right
            }

            StyledSlotsLayout {
                id: slotsLayout
                style: menuStyle
                padding.top: 0

                mainSlot: Row {
                    id: row
                    spacing: units.gu(2)

                    Button { id:foobutt
                        objectName: "messageButton"
                        text: i18n.dtr("ubuntu-settings-components", "Message")

                        onClicked: {
                            menu.replyExpanded = !menu.replyExpanded;
                        }

                        width: actionButton.width
                    }

                    Button {
                        id: actionButton
                        objectName: "actionButton"
                        text: i18n.dtr("ubuntu-settings-components", "Call back")
                        color: enabled ? theme.palette.normal.positive : theme.palette.disabled.positive

                        onClicked: {
                            menu.actionActivated();
                        }

                        width: (parent.width - row.spacing) / 2
                    }
                }
            }

            USC.ActionTextField {
                id: reply

                visible: false
                activateEnabled: true
                buttonText: i18n.dtr("ubuntu-settings-components", "Send")

                onActivated: {
                    menu.replied(value);
                }
            }
        }
    }
}
