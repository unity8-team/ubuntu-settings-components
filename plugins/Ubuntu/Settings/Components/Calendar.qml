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
 *      Andrea Cimitan <andrea.cimitan@canonical.com>
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Settings.Components 0.1
import "dateExt.js" as DateExt
import "Calendar.js" as Cal

ListView {
    id: calendar

    property var currentDate: new Date(priv.today.year, priv.today.month, 1)
    property int firstDayOfWeek: Qt.locale(i18n.language).firstDayOfWeek
    property var maximumDate
    property var minimumDate
    property var selectedDate: currentDate
    property var eventDays: new Array()
    property bool showWeekNumbers: false

    function reset() {
        if (!priv.ready) return;
        currentDate = new Date(priv.today.year, priv.today.month, 1)
    }

    Component.onCompleted: {
        priv.__populateModel();
    }

    onCurrentIndexChanged: {
        if (!priv.ready) return;

        currentDate = new Date(currentItem.month.year, currentItem.month.month, 1);
    }

    onSelectedDateChanged: {
        if (currentDate != selectedDate)
            currentDate = selectedDate
    }

    onCurrentDateChanged: {
        if (selectedDate.getFullYear() != currentDate.getFullYear() ||
            selectedDate.getMonth() != currentDate.getMonth()) {
            priv.userSelected = false
            selectedDate = new Date(currentDate.getFullYear(), currentDate.getMonth(), 1)
        }
    }

    ListModel {
        id: calendarModel
    }

    QtObject {
        id: priv

        property bool ready: false
        property bool userSelected: false
        property var today: new Cal.Day().fromDate((new Date()))
        property real weekDaysHeight: 0
        readonly property real squareUnit: units.gu(3)
        readonly property int days: 7
        readonly property int weeks: 6

        property var currentMonth: new Cal.Month().fromDate(currentDate)
        property var selectedDay: new Cal.Day().fromDate(selectedDate)
        property var minimumMonth: minimumDate ? new Cal.Month().fromDate(minimumDate) : undefined
        property var maximumMonth: maximumDate ? new Cal.Month().fromDate(maximumDate) : undefined

        property var minimumDay: minimumDate ? new Cal.Day().fromDate(minimumDate) : undefined
        property var maximumDay: maximumDate ? new Cal.Day().fromDate(maximumDate) : undefined

        onCurrentMonthChanged: {
            if (!ready) return
            __populateModel();
        }
        onMinimumMonthChanged: {
            if (!ready) return
            __populateModel();
        }
        onMaximumMonthChanged: {
            if (!ready) return
            __populateModel();
        }

        function __getRealMinimumMonth(month) {
            if (minimumMonth !== undefined && minimumMonth > month) {
                return minimumMonth;
            }
            return month;
        }

        function __getRealMaximumMonth(month) {
            if (maximumMonth !== undefined && maximumMonth < month) {
                return maximumMonth;
            }
            return month;
        }

        function __populateModel() {
            //  disable the onCurrentIndexChanged logic
            priv.ready = false;

            var minimumMonth = __getRealMinimumMonth(currentMonth.addMonths(-2));
            var maximumMonth = __getRealMaximumMonth(currentMonth.addMonths(2));

            // Remove old minimum months
            while (calendarModel.count > 0 && new Cal.Month(calendarModel.get(0).month) < minimumMonth) {
                calendarModel.remove(0);
            }
            // Remove old maximum months
            while (calendarModel.count > 0 && new Cal.Month(calendarModel.get(calendarModel.count - 1).month) > maximumMonth) {
                calendarModel.remove(calendarModel.count - 1);
            }

            if (calendarModel.count > 0) {
                // Add new months
                var firstMonth = new Cal.Month(calendarModel.get(0).month);
                while (firstMonth > minimumMonth) {
                    calendarModel.insert(0, { "month": firstMonth.addMonths(-1) });
                    firstMonth = new Cal.Month(calendarModel.get(0).month);
                }

                var lastMonth = new Cal.Month(calendarModel.get(calendarModel.count - 1).month);
                while (lastMonth < maximumMonth) {
                    calendarModel.append({ "month": lastMonth.addMonths(1) });
                    lastMonth = new Cal.Month(calendarModel.get(calendarModel.count - 1).month);
                }
            } else {
                var i = 0;
                do {
                    calendarModel.append({ "month": minimumMonth.addMonths(i) });
                    ++i;
                } while (minimumMonth.addMonths(i) <= maximumMonth)
            }

            currentIndex = currentMonth - minimumMonth;

            // Ok, we're all set up. enable the onCurrentIndexChanged logic
            priv.ready = true
        }
    }

    LiveTimer {
        frequency: calendar.visible ? LiveTimer.Minute : LiveTimer.Disabled
        onFrequencyChanged: trigger()
        onTrigger: {
            Date.timeZoneUpdated(); // FIXME remove when fixed in UITK
            var today = new Cal.Day().fromDate((new Date()));
            if (!priv.today.equals(today)) {
                priv.today = today;
                reset();
            }
        }
    }

    implicitWidth: priv.squareUnit * priv.days
    implicitHeight: priv.squareUnit * priv.weeks + priv.weekDaysHeight
    interactive: true
    clip: true
    cacheBuffer: Math.max((width+1) * 3, 0) // one page left, one page right
    highlightRangeMode: ListView.StrictlyEnforceRange
    preferredHighlightBegin: 0
    preferredHighlightEnd: width
    model: calendarModel
    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    focus: true
    highlightFollowsCurrentItem: true

    Keys.onLeftPressed: selectedDate.addDays(-1)
    Keys.onRightPressed: selectedDate.addDays(1)

    delegate: Row {
        property var month: new Cal.Month(model.month)
        property var monthStart: new Cal.Day(model.month.year, model.month.month, 1)
        property var monthEnd: monthStart.addMonths(1)
        property var gridStart: monthStart.weekStart(firstDayOfWeek)

        Column {
            id: weekColumn
            visible: calendar.showWeekNumbers
            spacing: monthGrid.rowSpacing

            Row {
                Column {
                    Label {
                        objectName: "weekDay" + modelData
                        text: i18n.tr("WEEK")
                        textSize: Label.XSmall
                        // FIXME: There's no good palette that covers both
                        //        Ambiance (Ash) and Suru (Silk)
                        color: theme.palette.disabled.base
                    }

                    Repeater {
                        id: weekNumbers
                        model: priv.weeks
                        delegate: Item {
                            property var rowDate: monthStart.addDays(modelData * priv.days)
                            property int weekNumber: QtDateFunctions.weekNumber(rowDate.toDate())
                            width: priv.squareUnit
                            height: priv.squareUnit

                            Label {
                                id: weekNumberLabel
                                anchors.centerIn: parent
                                text: weekNumber > 9 ? weekNumber : Qt.locale(i18n.language).zeroDigit + weekNumber
                                textSize: Label.Medium
                                color: theme.palette.normal.backgroundTertiaryText
                            }
                        }
                    }
                }

                Column {
                    Item {
                        width: units.gu(2)
                        height: weekColumn.height

                        Rectangle {
                            color: theme.palette.disabled.base
                            anchors.fill: parent
                            anchors.topMargin: units.gu(0.5)
                            anchors.bottomMargin: anchors.topMargin
                            anchors.leftMargin: units.gu(0.9)
                            anchors.rightMargin: anchors.leftMargin
                        }
                    }
                }
            }
        }

        Grid {
            id: monthGrid

            columns: priv.days
            columnSpacing: (calendar.width - calendar.implicitWidth - (weekColumn.visible ? weekColumn.width : 0)) / (columns - 1)

            rows: priv.weeks + 1 /* the weekDays header */
            rowSpacing: (calendar.height - calendar.implicitHeight) / (rows - 1)

            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignHCenter

            Repeater {
                id: daysHeader
                model: priv.days

                delegate: Label {
                    objectName: "weekDay" + modelData
                    text: Qt.locale(i18n.language).standaloneDayName((modelData + firstDayOfWeek) % priv.days, Locale.ShortFormat).toUpperCase()
                    textSize: Label.XSmall
                    // FIXME: There's no good palette that covers both
                    //        Ambiance (Ash) and Suru (Silk)
                    color: theme.palette.highlighted.base
                    onHeightChanged: priv.weekDaysHeight = Math.max(height, priv.weekDaysHeight)
                }
            }

            Repeater {
                model: priv.days * priv.weeks
                delegate: Item {
                    id: dayItem
                    objectName: "dayItem" + index

                    property int weekday: (index % priv.days + firstDayOfWeek) % priv.days
                    property var dayStart: gridStart.addDays(index)
                    property bool isSelected: priv.userSelected && dayStart.equals(priv.selectedDay)
                    property bool isCurrentMonth: (monthStart < dayStart || monthStart.equals(dayStart))  && dayStart < monthEnd
                    property bool isWeekend: weekday == 0 || weekday == 6
                    property bool isToday: dayStart.equals(priv.today)
                    property bool hasEvent: isCurrentMonth && eventDays.indexOf(dayStart.day) != -1
                    property bool isWithinBounds: (priv.minimumDay === undefined || dayStart >= priv.minimumDay) &&
                                                (priv.maximumDay === undefined || dayStart <= priv.maximumDay)

                    width: priv.squareUnit
                    height: priv.squareUnit

                    UbuntuShape {
                        anchors.fill: parent
                        visible: isToday
                        aspect: UbuntuShape.Flat
                        radius: "small"
                        color: dayNumber.color

                        UbuntuShape {
                            // XXX: since we can't just colorize the shape border
                            //      we need another one to fill the center with bg color
                            id: currentDayShape
                            radius: parent.radius
                            aspect: parent.aspect
                            backgroundColor: theme.palette.normal.background

                            anchors.fill: parent
                            anchors.margins: units.gu(0.1)
                        }
                    }

                    Label {
                        id: dayNumber
                        anchors.centerIn: parent
                        text: dayStart.toDate().toLocaleDateString(Qt.locale(), "dd")
                        textSize: Label.Medium
                        color: isSelected ? theme.palette.normal.positionText : theme.palette.normal.backgroundText

                        Binding on color {
                            when: isCurrentMonth && isWeekend && !isSelected
                            value: theme.palette.normal.backgroundTertiaryText
                        }

                        Binding on color {
                            when: !isCurrentMonth
                            // FIXME: There's no good palette that covers both
                            //        Ambiance (silk) and Suru (inkstone)
                            value: theme.palette.disabled.base
                        }
                    }

                    UbuntuShape {
                        objectName: "eventMarker"+index
                        aspect: UbuntuShape.Flat
                        radius: "small"
                        color: theme.palette.selected.baseText
                        width: units.gu(0.4)
                        height: width
                        visible: hasEvent
                        y: dayNumber.height + (parent.height - dayNumber.height - height) / 2
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    AbstractButton {
                        anchors.fill: parent

                        onClicked: {
                            if (isWithinBounds) {
                                if (!isSelected) {
                                    calendar.selectedDate = new Date(dayStart.year, dayStart.month, dayStart.day)
                                    priv.userSelected = true
                                } else if (priv.userSelected) {
                                    calendar.selectedDate = new Date(dayStart.year, dayStart.month)
                                    priv.userSelected = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
