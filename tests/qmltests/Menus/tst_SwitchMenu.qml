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
 * Authored by Nick Dedekind <nick.dedekind@canonical.com>
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

            SwitchMenu {
                id: switchMenu
                text: i18n.tr("Switch")
            }
            SwitchMenu {
                id: switchMenu2
                text: i18n.tr("Switch")
                checked: true
                anchors.top: switchMenu.bottom
            }
        }
    }

    SignalSpy {
        id: signalSpyTriggered
        target: switchMenu
        signalName: "triggered"
    }

    UbuntuTestCase {
        name: "SwitchMenu"
        when: windowShown

        function init() {
            switchMenu.checked = false;
        }

        function cleanup() {
            signalSpyTriggered.clear()
        }

        function test_switchChanged() {
            var switcher = findChild(switchMenu, "switcher");
            verify(switcher !== undefined);

            compare(switcher.checked, false, "switcher should initially be unchecked");
            switchMenu.checked = true;
            compare(switcher.checked, true, "switcher should be checked");
        }

        function test_clickSwitcher() {
            var switcher = findChild(switchMenu, "switcher");
            verify(switcher !== undefined);
            mouseClick(switchMenu, switcher.width / 2, switcher.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on switcher click");
            compare(signalSpyTriggered.signalArguments[0][0], true, "triggered signal argument non valid SwitchMenu click");
            compare(switchMenu.checked, true)
        }

        function test_clickCheckedSwitcher() {
            test_clickSwitcher()
            cleanup()
            var switcher = findChild(switchMenu, "switcher");
            verify(switcher !== undefined);
            mouseClick(switchMenu, switcher.width / 2, switcher.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on switcher click");
            compare(signalSpyTriggered.signalArguments[0][0], false, "triggered signal argument non valid SwitchMenu click");
            compare(switchMenu.checked, false)
        }

        function test_clickSwitchMenu() {
            mouseClick(switchMenu, switchMenu.width / 2, switchMenu.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on SwitchMenu click");
            compare(signalSpyTriggered.signalArguments[0][0], true, "triggered signal argument non valid SwitchMenu click");
            compare(switchMenu.checked, true)
        }

        function test_clickCheckedSwitchMenu() {
            test_clickSwitchMenu()
            cleanup()
            mouseClick(switchMenu, switchMenu.width / 2, switchMenu.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on SwitchMenu click");
            compare(signalSpyTriggered.signalArguments[0][0], false, "triggered signal argument non valid SwitchMenu click");
            compare(switchMenu.checked, false)
        }
    }
}
