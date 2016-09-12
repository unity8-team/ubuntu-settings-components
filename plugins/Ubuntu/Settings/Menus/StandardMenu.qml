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
    property alias icon: itemLayoutIcon
    property alias title: layoutItem.title
    property alias component: componentLoader.sourceComponent

    property alias iconSource: itemLayoutIcon.source
    property alias iconName: itemLayoutIcon.name
    property alias iconColor: itemLayoutIcon.color

    height: layoutItem.height + (divider.visible ? divider.height : 0)

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
            width: units.gu(2)
            SlotsLayout.position: SlotsLayout.Leading
        }

        Loader {
            id: componentLoader
            asynchronous: false
            visible: status == Loader.Ready
            SlotsLayout.position: SlotsLayout.Trailing
        }
    }
}

