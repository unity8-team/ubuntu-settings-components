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

ListItem {
    id: menu

    property string text
    property bool highlightWhenPressed: true
    property alias icon: itemLayoutIcon
    property alias title: layoutItem.title
    property alias component: componentLoader.sourceComponent
    property color foregroundColor: theme.palette.normal.baseText
    property alias backColor: menu.color

    // Can't create an alias for divider.visible here, see QTBUG-50407
    // Thus this hack is needed not to override the default divider.visible value
    property bool showDivider: false
    Component.onCompleted: {
        if (showDivider != divider.visible)
            showDivider = divider.visible;
    }
    onShowDividerChanged: divider.visible = showDivider
    divider.visible: false

    height: layoutItem.height + (divider.visible ? divider.height : 0)
    highlightColor: highlightWhenPressed ? theme.palette.highlighted.background : "transparent"

    // These fields are for retro-compatibility with ListItem.Empty
    signal triggered(var value)
    onClicked: triggered(null)

    property bool removable: false
    property bool confirmRemoval: true
    onConfirmRemovalChanged: console.error(menu+": confirmRemoval property is deprecated")
    signal itemRemoved()

    property bool selected: false
    onSelectedChanged: console.error(menu+": selected property is deprecated")


    ListItemLayout {
        id: layoutItem
        objectName: "layoutItem"
        title.text: menu.text
        title.color: menu.foregroundColor
        title.opacity: enabled ? 1 : 0.5

        Icon {
            id: itemLayoutIcon
            objectName: "itemLayoutIcon"
            color: theme.palette.normal.backgroundText
            SlotsLayout.position: SlotsLayout.Leading
        }

        Loader {
            id: componentLoader
            asynchronous: false
            visible: status == Loader.Ready
            SlotsLayout.position: SlotsLayout.Trailing
        }
    }

    ListItemActions {
        id: removeAction
        actions: [
            Action {
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
}
