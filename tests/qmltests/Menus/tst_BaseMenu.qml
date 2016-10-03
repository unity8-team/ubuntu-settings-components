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
import Ubuntu.Settings.Menus.Style 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    Column {
        anchors.fill: parent

        BaseMenu {
            id: baseMenu
        }

        BaseMenu {
            id: dividerMenu
            divider.visible: true
        }

        BaseMenu {
            id: clickOverride
            property bool clicked: false
            function onClickedCallback() { clicked = true }
        }

        BaseMenu {
            id: removableMenu
            removable: true
        }

        BaseMenu {
            id: pointerModeMenu
            pointerMode: true
        }

        BaseMenu {
            id: touchModeMenu
            pointerMode: false
        }

        BaseMenu {
            id: menuHeightMenu
            menuHeight: units.gu(10)
        }
    }

    SignalSpy {
        id: signalSpy
    }

    UbuntuTestCase {
        name: "BaseMenu"
        when: windowShown

        function cleanup() {
            signalSpy.clear()
        }

        function test_dontHighlightWhenPressed() {
            baseMenu.highlightWhenPressed = false;
            mousePress(baseMenu, baseMenu.width/2, baseMenu.height/2)
            compare(Qt.colorEqual(baseMenu.highlightColor, baseMenu.backColor), true)
            mouseRelease(baseMenu)
        }

        function test_doHighlightWhenPressed() {
            baseMenu.highlightWhenPressed = true;
            mousePress(baseMenu, baseMenu.width/2, baseMenu.height/2)
            compare(Qt.colorEqual(baseMenu.highlightColor, baseMenu.menuStyle.highlightColor), true)
            mouseRelease(baseMenu)
        }

        function test_showDivider() {
            compare(baseMenu.divider.visible, false)

            baseMenu.divider.visible = true
            compare(baseMenu.divider.visible, true)

            baseMenu.divider.visible = false
            compare(baseMenu.divider.visible, false)
        }

        function test_hideDivider() {
            compare(dividerMenu.divider.visible, true)

            dividerMenu.divider.visible = false
            compare(dividerMenu.divider.visible, false)

            dividerMenu.divider.visible = true
            compare(dividerMenu.divider.visible, true)
        }

        function test_noMenuHeight() {
            compare(baseMenu.implicitHeight > baseMenu.menuHeight, true)
        }

        function test_dividerMenuHeight() {
            menuHeightMenu.divider.visible = true
            compare(menuHeightMenu.implicitHeight, menuHeightMenu.menuHeight + dividerMenu.divider.height)
        }

        function test_noDividerMenuHeight() {
            menuHeightMenu.divider.visible = false
            compare(menuHeightMenu.implicitHeight, menuHeightMenu.menuHeight)
        }

        function test_clickEvent() {
            signalSpy.target = baseMenu
            signalSpy.signalName = "onClicked"
            mouseClick(baseMenu, baseMenu.width/2, baseMenu.height/2)
            compare(signalSpy.count, 1)
        }

        function test_disabledClickEvent() {
            signalSpy.target = baseMenu
            signalSpy.signalName = "onClicked"
            baseMenu.enabled = false
            mouseClick(baseMenu, baseMenu.width/2, baseMenu.height/2)
            compare(signalSpy.count, 0)
            baseMenu.enabled = true
        }

        function test_triggeredEvent() {
            signalSpy.target = baseMenu
            signalSpy.signalName = "onTriggered"
            mouseClick(baseMenu, baseMenu.width/2, baseMenu.height/2)
            compare(signalSpy.count, 1)
            compare(signalSpy.signalArguments[0][0], null)
        }

        function test_clickedSignalOverriding() {
            signalSpy.target = clickOverride
            signalSpy.signalName = "onTriggered"
            clickOverride.clicked = false
            mouseClick(clickOverride, clickOverride.width/2, baseMenu.height/2)
            compare(clickOverride.clicked, true)
            compare(signalSpy.count, 0)
        }

        function test_notRemovable() {
            compare(baseMenu.removable, false)
            compare(baseMenu.leadingActions, null)
            verify(findChild(baseMenu, "removeAction") === undefined)
        }

        function test_removable() {
            compare(removableMenu.removable, true)
            compare(removableMenu.leadingActions !== null, true)
            verify(findChild(baseMenu, "removeAction") === undefined)
        }

        function test_removal() {
            // test_removable()
            signalSpy.target = removableMenu
            signalSpy.signalName = "itemRemoved"
            mouseFlick(removableMenu,
                       removableMenu.width / 2,
                       removableMenu.height / 2,
                       removableMenu.width,
                       removableMenu.height / 2,
                       true, true, units.gu(1), 10);

            var removeAction = findChild(removableMenu, "removeAction");
            verify(removeAction !== undefined);
            tryCompare(removeAction, "visible", true)
            mouseClick(removeAction, removeAction.width/2, removeAction.height/2)
            tryCompare(signalSpy, "count", 1)
        }

        function test_pointerMode() {
            compare(pointerModeMenu.menuStyle, PointerStyle, "menuStyle doesn't match Pointer")
        }

        function test_touchMode() {
            compare(touchModeMenu.menuStyle, TouchStyle, "menuStyle doesn't match Touch")
        }
    }
}
