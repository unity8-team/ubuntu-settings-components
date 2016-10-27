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
    id: userSessionMenu

    property alias name: userSessionMenu.text
    property alias active: activeIcon.visible

    slots: [
        Rectangle {
            id: activeIcon
            objectName: "activeIcon"
            implicitWidth: checkMark.width + units.gu(1)
            implicitHeight: checkMark.height + units.gu(1)
            radius: width / 2
            antialiasing: true
            color: menuStyle.iconColor
            visible: false

            Icon {
                id: checkMark
                name: "tick"
                height: menuStyle.iconSize
                width: height
                anchors.centerIn: parent
            }
        }
    ]
}
