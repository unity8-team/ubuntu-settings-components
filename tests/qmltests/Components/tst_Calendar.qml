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
    }
}
