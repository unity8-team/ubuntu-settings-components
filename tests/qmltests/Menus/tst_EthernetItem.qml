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
 * Authored by Pete Woods <pete.woods@canonical.com>
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

        EthernetItem {
            id: ethernetItem
        }

        EthernetItem {
            id: checkedEthernetItem
            checked: true
        }

        EthernetItem {
            id: clickEthernetItem
        }
    }

    SignalSpy {
        id: signalSpy
    }

    UbuntuTestCase {
        name: "BaseMenu"
        when: windowShown

        function getSwitchItem(item) {
            var switchItem = findChild(item, "switcher");
            verify(switchItem !== undefined)
            return switchItem
        }

        function cleanup() {
            clickEthernetItem.checked = false
            signalSpy.clear()
        }

        function test_checked() {
            compare(getSwitchItem(ethernetItem).checked, false, "should be unchecked by default")
        }

        function test_unChecked() {
            compare(getSwitchItem(checkedEthernetItem).checked, true, "should be checked")
        }

        function checkedData() {
            return [ {tag: "unchecked", checked: false}, {tag: "checked", checked: true}]
        }

        function test_changing_data() {
            return checkedData()
        }

        function test_changing(data) {
            clickEthernetItem.checked = data.checked
            compare(getSwitchItem(clickEthernetItem).checked, data.checked, "switch should be %1".arg(data.tag))
        }

        function test_clickEvent_data() {
            return checkedData()
        }

        function test_clickEvent(data) {
            signalSpy.target = clickEthernetItem
            signalSpy.signalName = "onClicked"
            clickEthernetItem.checked = data.checked
            mouseClick(clickEthernetItem, clickEthernetItem.width/2, clickEthernetItem.height/2)
            compare(signalSpy.count, 1)
        }

        function test_onCheckedChanged_data() {
            return checkedData()
        }

        function test_onCheckedChanged(data) {
            clickEthernetItem.checked = data.checked
            signalSpy.clear()
            signalSpy.target = clickEthernetItem
            signalSpy.signalName = "onTriggered"
            mouseClick(clickEthernetItem, clickEthernetItem.width/2, clickEthernetItem.height/2)
            compare(signalSpy.count, 1)
        }

        function getChild(name) {
            var child = findChild(ethernetItem, name)
            verify(child !== undefined)
            return child
        }

        function test_nameStatusIcon_data() {
            return [{name: "Ethernet (eth0)", statusText: "Connected", statusIcon: "network-wired-connected"},
                    {name: "Ethernet", statusText: "Connecting", statusIcon: "network-wired-active"},
                    {name: "Ethernet", statusText: "Disabled", statusIcon: "network-wired-disabled"}]
        }

        function test_nameStatusIcon(data) {
            ethernetItem.name = data.name
            ethernetItem.statusText = data.statusText
            ethernetItem.statusIcon = data.statusIcon

            compare(getChild("labelName").text, data.name)
            compare(getChild("labelStatus").text, data.statusText)
            compare(getChild("statusIcon").name, data.statusIcon)
        }
    }
}
