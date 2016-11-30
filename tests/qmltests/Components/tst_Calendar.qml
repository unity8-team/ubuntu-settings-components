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
import Ubuntu.Settings.Components 0.1
import Ubuntu.Components 1.3

Rectangle {
    width: units.gu(42)
    height: units.gu(75)

    color: theme.palette.normal.background

    Label {
        id: label
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: units.gu(2)
        }
        height: units.gu(5)
        text: Qt.formatDate(calendar.currentDate, "MMMM") + " " + calendar.currentDate.getFullYear()
    }

    Calendar {
        id: calendar
        anchors {
            left: parent.left
            right: parent.right
            top: label.bottom
        }
    }

    UbuntuTestCase {
        name: "Calendar"
        when: windowShown

        function init() {
            calendar.selectedDate = new Date(2013, 4, 10);
        }

        function test_daysInMonth_data() {
            var tests = []

            for (var year = 2000; year <= 2016; ++year) {
                for (var month = 1; month <= 12; ++month) {
                    tests.push({tag: year+"/"+month, year: year, month: month })
                }
            }

            return tests
        }

        function test_daysInMonth(data) {
            switch (data.month) {
                case 2:
                    var expected = Date.leapYear(data.year) ? 29 : 28
                    break
                case 4:
                case 6:
                case 9:
                case 11:
                    var expected = 30
                    break
                default:
                    var expected = 31
            }

            compare(Date.daysInMonth(data.year, data.month-1), expected)
        }

        function test_addMonthsFromLastDay_data() {
            var tests = []
            for (var i = 0; i <= 25; ++i) {
                tests.push({tag: "previous "+i, delta: -i })
                tests.push({tag: "next "+i, delta: i })
            }
            return tests
        }

        function test_addMonthsFromLastDay(data) {
            var date = new Date(2016, 4, 31)
            var newDate = date.addMonths(data.delta)
            var monthDelta = date.getMonth() + data.delta
            var monthNumber = monthDelta % 12

            compare(newDate.getFullYear(), date.getFullYear() + monthDelta / 12 | 0)
            compare(newDate.getMonth(), monthNumber < 0 ? 12 + monthNumber : monthNumber)
            compare(newDate.getDate(), Date.daysInMonth(newDate.getFullYear(), newDate.getMonth()))
        }

        function test_selectedDate_data() {
            return [
                { date: new Date(2010, 4, 10) },
                { date: new Date() },
                { date: new Date(2020, 10, 31)},
            ];
        }

        function test_selectedDate(data) {
            calendar.selectedDate = data.date;

            compare(calendar.currentItem.month.firstDay().year, data.date.getFullYear(), "Current year does no correspond to set date");
            compare(calendar.currentItem.month.firstDay().month, data.date.getMonth(), "Current month does no correspond to set date");
        }

        function test_firstDayOfWeek_data() {
            return [
                {tag: 'Thursday', firstDayOfWeek: 5},
                {tag: 'Sunday', firstDayOfWeek: 0},
            ];
        }

        function test_firstDayOfWeek(data) {
            calendar.firstDayOfWeek = data.firstDayOfWeek;

            for (var i = 0; i < (6*7); i++) {
                var dayColumn = findChild(calendar, "dayItem" + i);
                verify(dayColumn);

                var dayStart =  new Date(dayColumn.dayStart.year, dayColumn.dayStart.month, dayColumn.dayStart.day);
                compare(dayStart.getDay(), (data.firstDayOfWeek + i)%7, "Day column does not match expected for firstDayOfWeek");
            }
        }

        function test_selectedDateUpdatesCurrent_data() {
            return [
                { date: new Date(2010, 4, 10) },
                { date: new Date() },
                { date: new Date(2020, 10, 31)},
            ];
        }

        function test_selectedDateUpdatesCurrent(data) {
            calendar.selectedDate = data.date
            compare(calendar.currentDate, data.date)
        }

        function test_eventsMarker_data()
        {
            var values = []
            for (var i = 0; i < 20; ++i) {
                values.push({tag: "eventDay "+i+": "+(i % 2 != 0), idx: i, dayEvent: (i % 2 != 0)})
            }

            return values
        }

        function test_eventsMarker(data) {
            var eventMarkerLoader = findChild(calendar, "eventMarkerLoader"+data.idx)
            verify(eventMarkerLoader)

            var expected = data.dayEvent

            if (data.dayEvent) {
                var dayItem = findChild(calendar, "dayItem"+data.idx)
                verify(dayItem)

                calendar.eventDays = [dayItem.dayStart.day]
                expected = dayItem.isCurrentMonth
            }

            compare(eventMarkerLoader.visible, expected)
            var eventMarker = findChild(eventMarkerLoader, "eventMarker"+data.idx)
            verify(expected ? eventMarker : eventMarker === undefined)
        }

        function test_showWeeksNumber_data() {
            return [{tag: "visible", visible: true}, {tag: "not visible", visible: false}]
        }

        function test_showWeeksNumber(data) {
            var weekNumbersLoader = findChild(calendar, "weekNumbersLoader"+calendar.currentIndex)
            verify(weekNumbersLoader)

            calendar.showWeekNumbers = data.visible
            var weekNumbersColumn = findChild(weekNumbersLoader, "weekNumbersColumn"+calendar.currentIndex)
            verify(data.visible ? weekNumbersColumn !== undefined : weekNumbersColumn === undefined)
        }

        function test_isToday() {
            for (var i = 0; i < 7*6; ++i) {
                var dayItem = findChild(calendar, "dayItem"+i)
                verify(dayItem)
                var todayMarkerLoader = findChild(dayItem, "todayMarkerLoader"+i)
                verify(todayMarkerLoader)

                compare(todayMarkerLoader.visible, dayItem.isToday)
                compare(todayMarkerLoader.active, dayItem.isToday)
            }
        }

        function test_moveToMonth_data() {
            var tests = []

            for (var i = 1; i <= 15; ++i) {
                tests.push({tag: "previous "+i, delta: -i })
                tests.push({tag: "next "+i, delta: i })
            }

            return tests
        }

        function test_moveToMonth(data) {
            var expected = calendar.currentDate.addMonths(data.delta)
            var now = new Date()

            if (expected.getFullYear() != now.getFullYear() || expected.getMonth() != now.getMonth()) {
                expected.setDate(1)
            } else {
                expected.setDate(now.getDate())
            }

            calendar.moveToMonth(data.delta)
            compare(calendar.currentDate, expected)
        }
    }
}
