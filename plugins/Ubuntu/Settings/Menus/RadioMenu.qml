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
 * Authors:
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

BaseLayoutMenu {
    id: menu

    property alias checked: radio.visible

    function onClickedCallback() {
        checked = true
        menu.triggered(checked)
    }

    slots: Item {
        width: menuStyle.iconSize
        height: width
        SlotsLayout.position: SlotsLayout.Leading

        Rectangle {
            id: radio
            objectName: "radio"
            anchors.centerIn: parent
            visible: false
            width: units.gu(0.8)
            height: width
            radius: width/2
            color: menuStyle.foregroundColor
        }
    }
}
