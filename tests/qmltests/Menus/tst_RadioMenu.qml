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

        RadioMenu {
            id: radioMenu
        }

        RadioMenu {
            id: checkedRadioMenu
            checked: true
        }

        RadioMenu {
            id: clickMenu
        }
    }

    SignalSpy {
        id: signalSpy
    }

    UbuntuTestCase {
        name: "BaseMenu"
        when: windowShown

        function getRadio(item) {
            var radio = findChild(item, "bullet");
            verify(radio !== undefined)
            return radio
        }

        function cleanup() {
            clickMenu.checked = false
            signalSpy.clear()
        }

        function test_checked() {
            compare(getRadio(radioMenu).visible, false, "radio should be hidden by default")
        }

        function test_unChecked() {
            compare(getRadio(checkedRadioMenu).visible, true, "radio should be visible on checked item")
        }

        function checkedData() {
            return [ {tag: "unchecked", checked: false}, {tag: "checked", checked: true}]
        }

        function test_radioVisiblity_data() {
            return checkedData()
        }

        function test_radioVisiblity(data) {
            clickMenu.checked = data.checked
            compare(getRadio(clickMenu).visible, data.checked, "radio should be %1 by default".arg(data.checked ? "visible" : "hidden"))
        }

        function test_clickEvent_data() {
            return checkedData()
        }

        function test_clickEvent(data) {
            signalSpy.target = clickMenu
            signalSpy.signalName = "onClicked"
            clickMenu.checked = data.checked
            mouseClick(clickMenu, clickMenu.width/2, clickMenu.height/2)
            compare(signalSpy.count, 1)
        }

        function test_triggeredEvent_data() {
            return checkedData()
        }

        function test_triggeredEvent(data) {
            signalSpy.target = clickMenu
            signalSpy.signalName = "onTriggered"
            clickMenu.checked = data.checked
            mouseClick(clickMenu, clickMenu.width/2, clickMenu.height/2)
            compare(signalSpy.count, 1)
            compare(signalSpy.signalArguments[0][0], true)
        }

        function test_onCheckedChanged_data() {
            return checkedData()
        }

        function test_onCheckedChanged(data) {
            clickMenu.checked = data.checked
            signalSpy.clear()
            signalSpy.target = clickMenu
            signalSpy.signalName = "onCheckedChanged"
            mouseClick(clickMenu, clickMenu.width/2, clickMenu.height/2)
            compare(signalSpy.count, data.checked ? 0 : 1)
            compare(clickMenu.checked, true)
        }

        function test_ClickOnCheckedDoesntUnchecks() {
            clickMenu.checked = true
            signalSpy.clear()
            signalSpy.target = clickMenu
            signalSpy.signalName = "onTriggered"
            compare(clickMenu.checked, true)
            mouseClick(clickMenu, clickMenu.width/2, clickMenu.height/2)
            compare(signalSpy.count, 1)
            compare(clickMenu.checked, true)
        }
    }
}
