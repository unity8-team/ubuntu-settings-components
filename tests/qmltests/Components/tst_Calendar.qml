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
            calendar.maximumDate = undefined;
            calendar.minimumDate = undefined;
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

            compare(calendar.currentItem.monthStart.year, data.date.getFullYear(), "Current year does no correspond to set date");
            compare(calendar.currentItem.monthStart.month, data.date.getMonth(), "Current month does no correspond to set date");
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

        function test_minMaxDate_data() {
            return [
                {tag: "Min=-0", date: new Date(), minDate: new Date(), maxDate: undefined, count: 3},
                {tag: "Min=-1", date: new Date(), minDate: new Date().addMonths(-1), maxDate: undefined, count: 4},
                {tag: "Min=-22", date: new Date(), minDate: new Date().addMonths(-22), maxDate: undefined, count: 5}, // max out at +-2

                {tag: "Max=+0", date: new Date(), minDate: undefined, maxDate: new Date(), count: 3},
                {tag: "Max=+1", date: new Date(), minDate: undefined, maxDate: new Date().addMonths(1), count: 4},
                {tag: "Max=+22", date: new Date(), minDate: undefined, maxDate: new Date().addMonths(22), count: 5}, // max out at +-2

                {tag: "Min=-0,Max=+0", date: new Date(), minDate: new Date(), maxDate: new Date(), count: 1},
                {tag: "Min=-1,Max=+1", date: new Date(), minDate: new Date().addMonths(-1), maxDate: new Date().addMonths(1), count: 3},
                {tag: "Min=-22,Max=+1", date: new Date(), minDate: new Date().addMonths(-22), maxDate: new Date().addMonths(1), count: 4}, // max out at +-2
                {tag: "Min=-1,Max=+22", date: new Date(), minDate: new Date().addMonths(-1), maxDate: new Date().addMonths(22), count: 4}, // max out at +-2
                {tag: "Min=-22,Max=+22", date: new Date(), minDate: new Date().addMonths(-22), maxDate: new Date().addMonths(22), count: 5}, // max out at +-2
            ];
        }

        function test_minMaxDate(data) {
            calendar.selectedDate = data.date;
            calendar.minimumDate = data.minDate;
            calendar.maximumDate = data.maxDate;
            compare(calendar.count, data.count, "The number of months should have changed");
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
            var eventMarker = findChild(calendar, "eventMarker"+data.idx)
            verify(eventMarker)

            var expected = data.dayEvent

            if (data.dayEvent) {
                var dayItem = findChild(calendar, "dayItem"+data.idx)
                verify(dayItem)

                calendar.eventDays = [dayItem.dayStart.day]
                expected = dayItem.isCurrentMonth
            }

            compare(eventMarker.visible, expected)
        }

        function test_showWeeksNumber() {
            
        }
    }
}
