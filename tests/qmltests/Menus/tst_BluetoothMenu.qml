/*
 * Copyright 2014 Canonical Ltd.
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

import QtQuick 2.0
import QtTest 1.0
import Ubuntu.Components 0.1
import Ubuntu.Settings.Menus 0.1
import "../utils.js" as UtilsJS

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

            BluetoothMenu {
                id: bluetoothMenu
                text: i18n.tr("Bluetooth Device")
                iconSource: Qt.resolvedUrl("../../artwork/avatar.png")
            }
        }
    }

    SignalSpy {
        id: signalSpy
        signalName: "connect"
        target: bluetoothMenu
    }

    TestCase {
        name: "BluetoothMenu"
        when: windowShown

        function test_text_data() {
            return [
                { text: "text1" },
                { text: "text2" },
            ];
        }

        function test_text(data) {
            bluetoothMenu.text = data.text;

            var text = UtilsJS.findChild(bluetoothMenu, "text");
            verify(text, "No text");
            compare(text.text, data.text, "Text does not match set text.");
        }

        function test_iconSource_data() {
            return [
                { icon: Qt.resolvedUrl("../../artwork/avatar.png") },
                { icon: Qt.resolvedUrl("../../artwork/rhythmbox.png") },
            ];
        }

        function test_iconSource(data) {
            bluetoothMenu.iconSource = data.icon;

            var icon = UtilsJS.findChild(bluetoothMenu, "icon");
            verify(icon, "No icon");
            compare(icon.source, data.icon, "Icon does not match set icon.");
        }

        function test_connectEnabled_data() {
            return [
                { tag: "false, unselected", enabled: false, selected: false, expected: false },
                { tag: "true, unselected", enabled: true, selected: false, expected: false },
                { tag: "false, selected", enabled: false, selected: true, expected: false },
                { tag: "true, selected", enabled: true, selected: true, expected: true }
            ];
        }

        function test_connectEnabled(data) {
            bluetoothMenu.selected = data.selected;
            bluetoothMenu.connectEnabled = data.enabled;

            var connect = UtilsJS.findChild(bluetoothMenu, "connect");
            verify(connect, "No connect");

            compare(connect.activateEnabled, data.expected, "Activate enabled does not match expected.");
        }
    }
}
