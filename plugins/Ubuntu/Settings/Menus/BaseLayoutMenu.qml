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
 * Author: Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

BaseMenu {
    id: menu

    property string text
    property alias layout: layoutItem
    property alias title: layoutItem.title
    property alias slots: layoutItem.children

    // These fields are for retro-compatibility with ListItem.Empty based MenuItems
    property alias trailingComponent: trailingComponentLoader.sourceComponent
    property alias trailingComponentItem: trailingComponentLoader.item
    property alias component: menu.trailingComponent

    height: layoutItem.height + (divider.visible ? divider.height : 0)

    ListItemLayout {
        id: layoutItem
        objectName: "menuLayoutItem"
        title.text: menu.text
        title.color: menu.foregroundColor
        title.font.pixelSize: menuStyle.fontSize

        padding {
            top: menuStyle.padding.top
            bottom: menuStyle.padding.bottom
            leading: menuStyle.padding.leading
            trailing: menuStyle.padding.trailing
        }

        Loader {
            id: trailingComponentLoader
            asynchronous: false
            visible: status == Loader.Ready
            SlotsLayout.position: SlotsLayout.Trailing
        }
    }
}
