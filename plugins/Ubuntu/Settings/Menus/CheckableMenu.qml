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
 *      Nick Dedekind <nick.dedekind@canonical.com>
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

BaseLayoutMenu {
    id: menu

    property alias checked: checkbox.checked

    function onClickedCallback() {
        checked = !checked
        menu.triggered(checked)
    }

    slots: CheckBox {
        id: checkbox
        objectName: "checkbox"
        onClicked: menu.triggered(checked)
        width: menuStyle.iconSize
        height: width
        SlotsLayout.position: SlotsLayout.Leading
    }
}
