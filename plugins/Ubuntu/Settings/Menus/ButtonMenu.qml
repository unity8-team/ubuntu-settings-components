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
 * Authored by Andrea Cimitan <andrea.cimitan@canonical.com>
 *             Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

StandardMenu {
    id: menu
    property alias buttonText: buttonLabel.text
    signal buttonClicked()

    slots: [
        Button {
            objectName: "button"
            color: menuStyle.buttonColor
            width: Math.max(units.gu(menu.pointerMode ? 5 : 8), buttonLabel.paintedWidth + units.gu(4))
            height: menuStyle.buttonHeight

            Label {
                id: buttonLabel
                font.pixelSize: menuStyle.buttonFontSize
                color: theme.palette.normal.foregroundText
                anchors.centerIn: parent
                scale: parent.pressed ? 0.98 : 1
            }

            onClicked: {
                menu.buttonClicked()
                menu.clicked()
            }
        }
    ]
}
