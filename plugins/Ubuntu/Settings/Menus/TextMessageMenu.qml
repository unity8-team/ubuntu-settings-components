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
import Ubuntu.Settings.Components 0.1 as USC

SimpleMessageMenu {
    id: menu

    property alias replyEnabled: actionTextField.activateEnabled
    property alias replyButtonText: actionTextField.buttonText
    property alias replyHintText: actionTextField.textHint

    signal replied(string value)

    footer: USC.ActionTextField {
        id: actionTextField
        activateEnabled: true
        buttonText: i18n.dtr("ubuntu-settings-components", "Send")

        onActivated: {
            menu.replied(value);
        }
    }
}
