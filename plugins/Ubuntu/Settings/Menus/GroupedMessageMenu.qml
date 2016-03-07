/*
 * Copyright 2013 Canonical Ltd.
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
 */

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Components.ListItems 1.3 as ListItem

StandardMenu {
    id: menu

    property string count: "0"

    signal dismissed()

    iconSource: Qt.resolvedUrl("image://theme/message")

    component: Component {
        UbuntuShape {
            implicitHeight: label.implicitHeight + units.gu(2)
            implicitWidth: label.implicitWidth + units.gu(2)

            color: theme.palette.normal.backgroundText
            radius: "medium"

            Label {
                id: label
                objectName: "messageCount"

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                horizontalAlignment: Text.AlignRight
                font.weight: Font.DemiBold
                fontSize: "medium"
                text: menu.count

                color: theme.palette.normal.foregroundText
            }
        }
    }

    onItemRemoved: {
        menu.dismissed();
    }
}
