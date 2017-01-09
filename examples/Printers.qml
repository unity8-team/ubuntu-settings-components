/*
 * Copyright 2017 Canonical Ltd.
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
 * Authored by Jonas G. Drange <jonas.drange@canonical.com>
 */

import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3 as ListItems
import Ubuntu.Settings.Components 0.1
import Ubuntu.Settings.Printers 0.1

MainView {
    width: units.gu(50)
    height: units.gu(90)

    PageStack {
        id: pageStack

        Component.onCompleted: push(printersPage)

        Page {
            id: printersPage
            visible: false

            Column {
                anchors { left: parent.left; right: parent.right }

                Label {
                    text: "Printers"
                }

                ListView {
                    model:
                    delegate: ListItem {
                        // shall specify the height when Using ListItemLayout inside ListItem
                        height: modelLayout.height + (divider.visible ? divider.height : 0)
                        ListItemLayout {
                            id: modelLayout
                            title.text: modelData
                    }
                }
            }
        }
    }
}
