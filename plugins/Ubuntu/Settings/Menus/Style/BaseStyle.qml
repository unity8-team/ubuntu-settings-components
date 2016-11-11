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

QtObject {
    readonly property MenuPaddings padding: MenuPaddings {}
    readonly property real subtitleFontSize: units.gu(1.4)
    readonly property real avatarSize: units.gu(4)
    readonly property color foregroundColor: theme.palette.normal.backgroundText
    readonly property color highlightColor: theme.palette.highlighted.background
    readonly property color iconColor: foregroundColor
    readonly property color buttonColor: theme.palette.normal.foreground

    property real minimumHeight
    property real fontSize
    property real iconSize
    property real buttonFontSize
    property real buttonHeight
}
