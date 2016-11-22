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

import QtQuick 2.4
import QtTest 1.0
import Ubuntu.Test 0.1
import Ubuntu.Settings.Menus 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    property var date1: new Date(2012, 2, 10)
    property var date2: new Date(2013, 5, 10)
    property var date3: new Date(2014, 6, 10)
    property var date4: new Date(2016, 9, 30)
    property var date5: new Date(2016, 9, 31)

    Flickable {
        id: flickable

        anchors.fill: parent
        contentWidth: column.width
        contentHeight: column.height

        Item {
            id: column

            width: flickable.width
            height: childrenRect.height

            CalendarMenu {
                id: calendarMenu
            }
        }
    }

    UbuntuTestCase {
        name: "CalendarMenu"
        when: windowShown

        property var calendar: findChild(calendarMenu, "calendar")

        function test_interactive_data() {
            return [
                        {tag: "pointer", pointer: true, expected: false },
                        {tag: "touch", pointer: false, expected: true }
            ]
        }

        function test_interactive(data) {
            calendarMenu.pointerMode = data.pointer
            compare(calendar.interactive, data.expected, "Cannot use interactive")
        }

        function test_currentDate() {
            calendarMenu.currentDate = date2
            compare(calendar.currentDate, date2, "Cannot set currendDate")
        }

        function test_firstDayOfWeek() {
            calendarMenu.firstDayOfWeek = 5
            compare(calendar.firstDayOfWeek, 5, "Cannot set firstDayOfWeek")
        }

        function test_selectedDate() {
            calendar.selectedDate = date2
            compare(calendar.selectedDate, date2, "Cannot set selectedDate")
        }

        // regression test for https://bugs.launchpad.net/ubuntu/+source/ubuntu-settings-components/+bug/1620496
        function test_double30thOctober() {
            calendar.selectedDate = date4;
            calendar.selectedDate = calendar.selectedDate.addDays(1);
            compare(calendar.selectedDate, date5, "The next day after 2016-10-30 is not 2016-10-31");
        }

        function test_SwitchMonth_data() {
            var tests = []

            for (var i = 1; i <= 15; ++i) {
                tests.push({tag: "previous "+i, buttonName: "goPreviousMonth", delta: -i })
                tests.push({tag: "next "+i, buttonName: "goNextMonth", delta: i })
            }

            return tests
        }

        function test_SwitchMonth(data) {
            var button = findChild(calendarMenu, data.buttonName);
            var monthLayout = findChild(calendarMenu, "monthLayout")
            verify(button)
            verify(monthLayout)

            var expected = calendar.currentDate.addMonths(data.delta)
            var now = new Date()

            if (expected.getFullYear() != now.getFullYear() || expected.getMonth() != now.getMonth()) {
                expected.setDate(1)
            } else {
                expected.setDate(now.getDate())
            }

            for (var i = 0; i < Math.abs(data.delta); ++i)
                mouseClick(button, button.width / 2, button.height / 2)

            compare(calendar.currentDate, expected)
            compare(monthLayout.title.text, i18n.ctr("%1=month name, %2=4-digit year", "%1 %2")
                                                .arg(Qt.locale().standaloneMonthName(expected.getMonth(), Locale.LongFormat))
                                                .arg(expected.getFullYear()))
        }
    }
}
