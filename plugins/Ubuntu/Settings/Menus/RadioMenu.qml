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

    property alias checked: bullet.visible

    function onClickedCallback() {
        checked = true
        menu.triggered(checked)
    }

    slots: Item {
        id: container
        width: menuStyle.iconSize
        height: width
        anchors.verticalCenter: parent.verticalCenter
        SlotsLayout.overrideVerticalPositioning: true
        SlotsLayout.position: SlotsLayout.Leading

        states: State {
           name: "pointer";
           when: menu.pointerMode

           PropertyChanges {
                target: menu.layout
                padding.leading: 0
           }
           PropertyChanges {
                target: container
                width: menuStyle.padding.leading + units.gu(1)
                SlotsLayout.padding.leading: 0
                SlotsLayout.padding.trailing: 0
           }
           PropertyChanges {
                target: menu.layout.mainSlot
                SlotsLayout.padding.leading: 0
           }
        }

        Rectangle {
            id: bullet
            objectName: "bullet"
            anchors.centerIn: parent
            visible: false
            width: visible ? units.gu(0.8) : 0
            height: width
            radius: width/2
            color: menu.foregroundColor
            opacity: visible ? 1.0 : 0.0
            Behavior on opacity { UbuntuNumberAnimation {} }
            Behavior on width { UbuntuNumberAnimation {} }
        }
    }
}
