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

            CheckableMenu {
                id: checkMenu
                text: i18n.tr("Check")
            }
        }
    }

    SignalSpy {
        id: signalSpyTriggered
        target: checkMenu
        signalName: "triggered"
    }

    UbuntuTestCase {
        name: "CheckableMenu"
        when: windowShown

        function init() {
            checkMenu.checked = false;
        }

        function cleanup() {
            signalSpyTriggered.clear()
        }

        function test_checkChanged() {
            var checkbox = findChild(checkMenu, "checkbox");
            verify(checkbox !== undefined);

            compare(checkbox.checked, false, "Checkbox should initially be unchecked");
            checkMenu.checked = true;
            compare(checkbox.checked, true, "Checkbox should be checked");
        }

        function test_clickCheckBox() {
            var checkbox = findChild(checkMenu, "checkbox");
            verify(checkbox !== undefined);
            mouseClick(checkMenu, checkbox.width / 2, checkbox.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on checkbox click");
            compare(signalSpyTriggered.signalArguments[0][0], true, "triggered signal argument non valid checkMenu click");
            compare(checkMenu.checked, true)
        }

        function test_clickCheckedCheckBox() {
            test_clickCheckBox()
            cleanup()
            var checkbox = findChild(checkMenu, "checkbox");
            verify(checkbox !== undefined);
            mouseClick(checkMenu, checkbox.width / 2, checkbox.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on checkbox click");
            compare(signalSpyTriggered.signalArguments[0][0], false, "triggered signal argument non valid checkMenu click");
            compare(checkMenu.checked, false)
        }

        function test_clickCheckMenu() {
            mouseClick(checkMenu, checkMenu.width / 2, checkMenu.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on checkMenu click");
            compare(signalSpyTriggered.signalArguments[0][0], true, "triggered signal argument non valid checkMenu click");
            compare(checkMenu.checked, true)
        }

        function test_clickCheckedCheckMenu() {
            test_clickCheckMenu()
            cleanup()
            mouseClick(checkMenu, checkMenu.width / 2, checkMenu.height / 2);
            compare(signalSpyTriggered.count, 1, "signal checked not triggered on checkMenu click");
            compare(signalSpyTriggered.signalArguments[0][0], false, "triggered signal argument non valid checkMenu click");
            compare(checkMenu.checked, false)
        }
    }
}
