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
import Ubuntu.Settings.Components 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    Flickable {
        anchors.fill: parent

        ActionTextField {
            id: actionField
            text: "Message"
            buttonText: "Send!"
        }
    }

    SignalSpy {
        id: activatedSignalSpy
        target: actionField
        signalName: "activated"
    }

    UbuntuTestCase {
        name: "ActionTextField"
        when: windowShown

        property QtObject replyText
        property QtObject sendButton

        function init() {
            replyText = findChild(actionField, "replyText");
            verify(replyText !== undefined)

            sendButton = findChild(actionField, "sendButton");
            verify(sendButton !== undefined)
        }

        function cleanup() {
            activatedSignalSpy.clear()
        }

        function test_replyText() {
            actionField.text = "FoooBar"
            compare(replyText.text, "FoooBar")
        }

        function test_replyTextLosesFocusOnDisabled() {
            mouseClick(replyText, replyText.width/2, replyText.height/2)
            compare(replyText.focus, true)

            replyText.enabled = false
            compare(replyText.focus, false)
        }

        function test_buttonText() {
            actionField.buttonText = "BarBar"
            compare(sendButton.text, "BarBar")
        }

        function test_buttonState() {
            actionField.activateEnabled = false
            actionField.text = "Send me"
            compare(sendButton.enabled, false)

            actionField.activateEnabled = true
            compare(sendButton.enabled, true)

            actionField.text = ""
            compare(sendButton.enabled, false)

            actionField.text = "Enable again"
            compare(sendButton.enabled, true)
        }

        function test_buttonClickActivatesTextArea() {
            actionField.activateEnabled = true
            mouseClick(sendButton, sendButton.width/2, sendButton.height/2)
            compare(activatedSignalSpy.count, 1)
            compare(activatedSignalSpy.signalArguments[0][0], replyText.text)
        }
    }
}
