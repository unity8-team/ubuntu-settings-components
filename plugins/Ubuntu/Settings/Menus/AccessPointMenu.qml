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

StandardMenu {
    id: ap
    property bool active: false
    property bool secure: false
    property bool adHoc: false
    property int signalStrength: 0

    icon.height: Math.min(units.gu(3), parent.height - units.gu(1))
    icon.width: icon.height
    icon.color: ap.active ? theme.palette.normal.positive : theme.palette.normal.backgroundText
    icon.name: {
        if (adHoc) {
            return "nm-adhoc";
        } else if (signalStrength <= 0) {
            return "nm-signal-00";
        } else if (signalStrength <= 25) {
            return "nm-signal-25";
        } else if (signalStrength <= 50) {
            return "nm-signal-50";
        } else if (signalStrength <= 75) {
            return "nm-signal-75";
        }

        return "nm-signal-100";
    }

    component: Icon {
        id: iconSecure
        objectName: "iconSecure"
        visible: ap.secure
        name: "network-secure"
        color: ap.active ? theme.palette.normal.positive : theme.palette.normal.backgroundText
        width: height
        height: Math.min(units.gu(3), ap.parent.height - units.gu(1))
    }
}
