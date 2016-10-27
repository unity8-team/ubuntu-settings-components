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
 * Author: Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Settings.Menus.Style 0.1

ListItem {
    id: menu

    property bool pointerMode: false
    property bool highlightWhenPressed: true
    property real menuHeight: -1
    property BaseStyle menuStyle: pointerMode ? PointerStyle : TouchStyle
    property alias backColor: menu.color
    property color foregroundColor: menuStyle.foregroundColor

    // This is for retro-compatibility with ListItem.Empty, adding support to override the callback
    signal triggered(var value)
    function onClickedCallback() { triggered(null) }
    onClicked: onClickedCallback()

    property bool removable: false
    property bool confirmRemoval: true
    onConfirmRemovalChanged: console.error(menu+": confirmRemoval property is deprecated")
    signal itemRemoved()

    divider.visible: false

    // FIXME: this is should use implicitHeight, but we can't yet due to lp:1630683
    Binding on height {
        when: menuHeight >= 0
        value: Math.max(menuStyle.minimumHeight, menuHeight) + (divider.visible ? divider.height : 0)
    }

    Binding on highlightColor {
        when: !highlightWhenPressed
        value: backColor
    }

    Binding on highlightColor {
        when: highlightWhenPressed && menuStyle
        value: menuStyle.highlightColor
    }

    ListItemActions {
        id: removeAction
        actions: [
            Action {
                objectName: "removeAction"
                iconName: "delete"
                onTriggered: removeItemAnimation.start();
            }
        ]

        SequentialAnimation {
            id: removeItemAnimation

            running: false
            UbuntuNumberAnimation {
                target: menu
                property: "height"
                to: 0
            }
            ScriptAction {
                script: {
                    itemRemoved()
                }
            }
        }
    }

    leadingActions: removable ? removeAction : null
    clip: leadingActions || trailingActions
}
