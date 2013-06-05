/*
 * Copyright 2013 Canonical Ltd.
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
 * Authored by Andrea Cimitan <andrea.cimitan@canonical.com>
 */

import QtQuick 2.0
import QtTest 1.0
import Ubuntu.Components 0.1
import "../../SystemComponents"
import "utils.js" as UtilsJS

Item {
    width: units.gu(42)
    height: units.gu(75)

    Flickable {
        id: flickable

        anchors.fill: parent
        contentWidth: column.width
        contentHeight: column.height

        Column {
            id: column

            width: flickable.width
            height: childrenRect.height

            CalendarMenu {
                id: calendarMenu
            }
        }
    }

    TestCase {
        name: "CalendarMenu"
        when: windowShown

        property var calendar: UtilsJS.findChild(calendarMenu, "calendar")

        function test_compressed() {
            calendarMenu.compressed = true
            compare(calendar.compressed, true, "Cannot set compressed")
        }

        function test_currentDate() {
            calendarMenu.currentDate = new Date(2013, 5, 10)
            compare(calendar.currentDate, new Date(2013, 5, 10), "Cannot set currendDate")
        }

        function test_firstDayOfWeek() {
            calendarMenu.firstDayOfWeek = 5
            compare(calendar.firstDayOfWeek, 5, "Cannot set firstDayOfWeek")
        }

        function test_maximumDate() {
            calendarMenu.maximumDate = new Date(2014, 6, 10)
            compare(calendar.maximumDate, new Date(2014, 6, 10), "Cannot set maximumDate")
        }

        function test_minimumDate() {
            calendar.minimumDate = new Date(2012, 2, 10)
            compare(calendar.minimumDate, new Date(2012, 2, 10), "Cannot set minimumDate")
        }

        function test_selectedDate() {
            calendar.selectedDate = new Date(2013, 5, 10)
            compare(calendar.selectedDate, new Date(2013, 5, 10), "Cannot set selectedDate")
        }
    }
}