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
import Ubuntu.Settings.Components 0.1 as USC

SimpleTextMessageMenu {
    id: menu

    property bool replyEnabled: true
    property string replyButtonText: "Send"

    signal replied(string value)

    footer: USC.ActionTextField {
        id: actionTextField
        anchors.fill:  parent
        placeholderText: "Reply"

        activateEnabled: text != "" && replyEnabled
        buttonText: replyButtonText

        onActivated: {
            menu.replied(value);
        }
    }
}
