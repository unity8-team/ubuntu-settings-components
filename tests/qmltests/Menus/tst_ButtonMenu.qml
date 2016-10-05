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
 * Authored by Andrea Cimitan <andrea.cimitan@canonical.com>
 */

import QtQuick 2.4
import QtTest 1.0
import Ubuntu.Test 0.1
import Ubuntu.Settings.Menus 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    Flickable {
        id: flickable

        anchors.fill: parent
        contentWidth: column.width
        contentHeight: column.height

        Item {
            id: column

            width: flickable.width
            height: childrenRect.height

            ButtonMenu {
                id: buttonMenu
                text: i18n.tr("Button")
                buttonText: i18n.tr("Hello world!")
                iconSource: Qt.resolvedUrl("../../artwork/avatar.png")
            }
            ButtonMenu {
                id: buttonMenu2
                buttonText: i18n.tr("Button")
                anchors.top: buttonMenu.bottom
            }
        }
    }

    SignalSpy {
        id: menuItemClickSpy
        target: buttonMenu
        signalName: "clicked"
    }

    SignalSpy {
        id: buttonClickSpy
        target: buttonMenu
        signalName: "buttonClicked"
    }

    UbuntuTestCase {
        name: "ButtonMenu"
        when: windowShown

        property var button
        property var button2

        function init() {
            menuItemClickSpy.clear()
            buttonClickSpy.clear()

            button = findChild(buttonMenu, "button");
            button2 = findChild(buttonMenu2, "button");

            verify(button !== undefined);
            verify(button2 !== undefined);
        }

        function test_click() {
            mouseClick(button, button.width / 2, button.height / 2);
            compare(menuItemClickSpy.count, 1, "signal clicked not triggered");
            compare(buttonClickSpy.count, 1, "button signal clicked not triggered");
        }

        function test_clickOnItem() {
            mouseClick(buttonMenu, 1, button.height / 2);
            compare(menuItemClickSpy.count, 1, "signal clicked not triggered");
            compare(buttonClickSpy.count, 0, "button signal clicked triggered");
        }
    }
}
