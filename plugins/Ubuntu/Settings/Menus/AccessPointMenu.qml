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
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

ListItem
{
    id: menu
    property string text
    property bool active: false
    property bool secure: false
    property bool adHoc: false
    property int signalStrength: 0
    divider.visible: false

    signal triggered(var value)
    onClicked: triggered(null)
    height: layoutItem.height + (divider.visible ? divider.height : 0)

    ListItemLayout {
        id: layoutItem
        title.text: menu.text

        Icon {
            id: iconSignal
            objectName: "iconSignal"
            width: height
            height: Math.min(units.gu(3), parent.height - units.gu(1))
            color: menu.active ? theme.palette.normal.positive : theme.palette.normal.backgroundText
            name: {
                var imageName = "nm-signal-100"

                if (adHoc) {
                    imageName = "nm-adhoc";
                } else if (signalStrength <= 0) {
                    imageName = "nm-signal-00";
                } else if (signalStrength <= 25) {
                    imageName = "nm-signal-25";
                } else if (signalStrength <= 50) {
                    imageName = "nm-signal-50";
                } else if (signalStrength <= 75) {
                    imageName = "nm-signal-75";
                }
                return imageName;
            }

            SlotsLayout.position: SlotsLayout.Leading
        }

        Icon {
            id: iconSecure
            objectName: "iconSecure"
            visible: menu.secure
            name: "network-secure"
            color: menu.active ? theme.palette.normal.positive : theme.palette.normal.backgroundText
            width: height
            height: Math.min(units.gu(3), parent.height - units.gu(1))

            SlotsLayout.position: SlotsLayout.Trailing
        }
    }
}
