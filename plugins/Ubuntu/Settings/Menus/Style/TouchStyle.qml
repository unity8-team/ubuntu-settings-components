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

pragma Singleton
import QtQuick 2.4
import Ubuntu.Components 1.3

BaseStyle {
    id: style

    readonly property SlotsLayout __reference_layout: SlotsLayout {}

    padding {
        top: __reference_layout.padding.top
        bottom: __reference_layout.padding.bottom
        leading: __reference_layout.padding.leading
        trailing: __reference_layout.padding.trailing
    }

    minimumHeight: units.gu(5.9)
    fontSize: units.gu(2)
    iconSize: units.gu(2.5)
    buttonFontSize: units.gu(1.7)
    buttonHeight: units.gu(3)
}
