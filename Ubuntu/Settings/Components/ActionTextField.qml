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
 * Authors:
 *      Renato Araujo Oliveira Filho <renato@canonical.com>
 *      Olivier Tilloy <olivier.tilloy@canonical.com>
 */

import QtQuick 2.0
import Ubuntu.Components 0.1

Item {
    id: actionTextField
    property alias text: textField.text
    property alias buttonText: sendButton.text
    property alias placeholderText: textField.placeholderText
    property alias activateEnabled: sendButton.enabled

    property alias inputMethodHints: textField.inputMethodHints

    signal activated(var value)

    TextField {
        id: textField
        objectName: "textField"

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: sendButton.left
            rightMargin: units.gu(1)
        }
        hasClearButton: false

        onEnabledChanged: {
            //Make sure that the component lost focus when enabled = false,
            //otherwise it will get focus again when enable = true
            if (!enabled) {
                focus = false;
            }
        }
    }

    Button {
        id: sendButton
        objectName: "sendButton"
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        width: units.gu(9)
        color: enabled ? "#c94212" : "#bababa"

        onClicked: {
            actionTextField.activated(textField.text);
        }
    }
}
