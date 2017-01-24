/*
 * Copyright 2016 Canonical Ltd.
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
 * Authored by Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import QtTest 1.0
import Ubuntu.Test 0.1
import Ubuntu.Settings.Menus 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    Column {
        anchors.fill: parent

        StandardMenu {
            id: standardMenu
            text: "Label"
        }

        StandardMenu {
            id: iconMenu
            text: "Icon Menu"
            iconName: "view-fullscreen"
        }

        StandardMenu {
            id: emptyMenu
        }
    }

    UbuntuTestCase {
        name: "StandardMenu"
        when: windowShown

        function test_iconInvisible() {
            var menuIcon = findChild(standardMenu, "standardMenuIcon")
            verify(menuIcon !== undefined)
            compare(menuIcon.visible, false)
        }

        function test_iconVisible() {
            var menuIcon = findChild(iconMenu, "standardMenuIcon")
            verify(menuIcon !== undefined)
            compare(menuIcon.visible, true)
        }

        function test_iconVisibilityChanges() {
            var menuIcon = findChild(emptyMenu, "standardMenuIcon")
            verify(menuIcon !== undefined)
            compare(menuIcon.visible, false)

            emptyMenu.iconName = "close"
            compare(menuIcon.visible, true)

            emptyMenu.iconName = ""
            compare(menuIcon.visible, false)

            emptyMenu.iconSource = "image://theme/tick"
            compare(menuIcon.visible, true)

            emptyMenu.iconSource = ""
            compare(menuIcon.visible, false)
        }
    }
}
