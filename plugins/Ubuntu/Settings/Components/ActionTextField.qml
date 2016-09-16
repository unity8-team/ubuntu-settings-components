/*
 * Copyright 2013-2016 Canonical Ltd.
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
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

Item {
    id: textField
    property alias text: replyField.text
    property alias buttonText: sendButton.text
    property bool activateEnabled: false
    property alias textHint: replyField.placeholderText

    signal activated(var value)

    height: layout.height
    anchors { right: parent.right; left: parent.left }

    SlotsLayout {
        id: layout
        padding.top: 0

        mainSlot: TextArea {
            id: replyField
            objectName: "replyText"
            autoSize: true

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
            enabled: (replyField.text.length > 0 || replyField.inputMethodComposing) && textField.activateEnabled
            color: enabled ? theme.palette.normal.positive : theme.palette.disabled.positive

            onClicked: {
                Qt.inputMethod.commit();
                textField.activated(replyField.text);
            }

            SlotsLayout.position: SlotsLayout.Trailing
        }
    }
}
