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
    property var selectedDate: currentDate
    property var eventDays: new Array()
    property bool showWeekNumbers: false

    function reset() {
        if (!priv.ready) return;
        currentDate = new Date(priv.today.year, priv.today.month, 1)
    }

    function firstDayOfCurrentMonth() {
        return new Date(currentItem.month.year, currentItem.month.month)
    }

    function moveToMonth(delta) {
        if (!priv.ready) return;
        currentDate = priv.currentMonth.addMonths(delta).firstDay().toDate()
    }

    function resetSelectionForMonth() {
        if (!priv.ready) return;
        priv.userSelected = false
        selectedDate = firstDayOfCurrentMonth()
    }

    Component.onCompleted: {
        priv.__populateModel();
    }

    onCurrentIndexChanged: {
        if (!priv.ready) return;

        currentDate = firstDayOfCurrentMonth()
    }

    onSelectedDateChanged: {
        if (!priv.ready) return;

        if (currentDate != selectedDate)
            currentDate = selectedDate
    }

    onCurrentDateChanged: {
        if (!priv.ready) return;

        if (selectedDate != currentDate) {
            priv.userSelected = false
            selectedDate = currentDate
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
        readonly property real todayRingThickness: units.gu(.1)
        readonly property int days: 7
        readonly property int weeks: 6

        property var currentMonth: new Cal.Month().fromDate(currentDate)
        property var selectedDay: new Cal.Day().fromDate(selectedDate)

        onCurrentMonthChanged: {
            if (!ready) return
            __populateModel();
        }

        function __populateModel() {
            //  disable the onCurrentIndexChanged logic
            priv.ready = false;

            var minimumMonth = currentMonth.addMonths(-2);
            var maximumMonth = currentMonth.addMonths(2);

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
    highlightMoveDuration: UbuntuAnimation.FastDuration

    Keys.onLeftPressed: selectedDate.addDays(-1)
    Keys.onRightPressed: selectedDate.addDays(1)

    // This is a workaround for bug https://bugreports.qt.io/browse/QTBUG-49224
    delegate: Loader {
        id: monthDelegateLoader
        sourceComponent: monthComponent

        readonly property var month: new Cal.Month(model.month)

        Binding {
            target: monthDelegateLoader.item
            property: "monthIndex"
            value: index
            when: monthDelegateLoader.status == Loader.Ready
        }

        Binding {
            target: monthDelegateLoader.item
            property: "month"
            value: month
            when: monthDelegateLoader.status == Loader.Ready
        }
    }

Component {
    id: monthComponent
    Row {
        objectName: "monthRow" + monthIndex
        property int monthIndex: 0
        property var month: new Cal.Month()
        readonly property var monthStart: month.firstDay()
        readonly property var monthEnd: monthStart.addMonths(1)
        readonly property var gridStart: monthStart.weekStart(firstDayOfWeek)

        Loader {
            id: weekNumbersLoader
            objectName: "weekNumbersLoader" + monthIndex
            active: calendar.showWeekNumbers
            visible: active

            sourceComponent: Column {
                id: weekNumbersColumn
                objectName: "weekNumbersColumn" + monthIndex
                spacing: monthGrid.rowSpacing

                Row {
                    Column {
                        Label {
                            objectName: "weekDay" + monthIndex
                            text: i18n.ctr("Header text: keep it short and upper case", "WEEK")
                            textSize: Label.XSmall
                            // FIXME: There's no good palette that covers both
                            //        Ambiance (Ash) and Suru (Silk)
                            color: theme.palette.disabled.base
                        }

                        Repeater {
                            id: weekNumbers
                            model: priv.weeks
                            delegate: Item {
                                readonly property var rowDate: monthStart.addDays(index * priv.days).toDate()
                                width: priv.squareUnit
                                height: priv.squareUnit

                                Label {
                                    id: weekNumberLabel
                                    anchors.centerIn: parent
                                    text: QtDateFunctions.formattedWeekNumber(rowDate)
                                    textSize: Label.Medium
                                    color: theme.palette.normal.backgroundTertiaryText
                                }
                            }
                        }
                    }

                    Column {
                        Item {
                            width: units.gu(2)
                            height: weekNumbersColumn.height

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
        }

        Grid {
            id: monthGrid

            columns: priv.days
            columnSpacing: (calendar.width - calendar.implicitWidth - (weekNumbersLoader.visible ? weekNumbersLoader.width : 0)) / (columns - 1)

            rows: priv.weeks + 1 /* the weekDays header */
            rowSpacing: (calendar.height - calendar.implicitHeight) / (rows - 1)

            verticalItemAlignment: Grid.AlignVCenter
            horizontalItemAlignment: Grid.AlignHCenter

            Repeater {
                id: daysHeader
                model: priv.days

                delegate: Label {
                    objectName: "weekDay" + index
                    text: Qt.locale(i18n.language).standaloneDayName((index + firstDayOfWeek) % priv.days, Locale.ShortFormat).toUpperCase()
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

                    readonly property int weekday: (index % priv.days + firstDayOfWeek) % priv.days
                    readonly property var dayStart: gridStart.addDays(index)
                    readonly property bool isSelected: priv.userSelected && dayStart.equals(priv.selectedDay)
                    readonly property bool isCurrentMonth: (monthStart < dayStart || monthStart.equals(dayStart))  && dayStart < monthEnd
                    readonly property bool isWeekend: weekday == 0 || weekday == 6
                    readonly property bool isToday: dayStart.equals(priv.today)
                    readonly property bool hasEvent: isCurrentMonth && eventDays.indexOf(dayStart.day) != -1

                    width: priv.squareUnit
                    height: priv.squareUnit

                    Loader {
                        id: todayMarkerLoader
                        objectName: "todayMarkerLoader" + index
                        active: isToday
                        visible: active
                        anchors.fill: parent
                        sourceComponent: UbuntuShape {
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
                                anchors.margins: priv.todayRingThickness
                            }
                        }
                    }

                    Label {
                        id: dayNumber
                        anchors.centerIn: parent
                        text: dayStart.toDate().toLocaleDateString(Qt.locale(), "dd")
                        textSize: Label.Medium
                        color: {
                            if (!isCurrentMonth) {
                                // FIXME: There's no good palette that covers both
                                //        Ambiance (silk) and Suru (inkstone)
                                return theme.palette.disabled.base
                            }
                            if (isSelected) {
                                return theme.palette.normal.positionText
                            }
                            if (isWeekend) {
                                return theme.palette.normal.backgroundTertiaryText
                            }

                            return theme.palette.normal.backgroundText
                        }
                    }

                    Loader{
                        objectName: "eventMarkerLoader"+index
                        active: hasEvent
                        visible: active
                        width: units.gu(0.4)
                        height: width
                        y: dayNumber.height + (parent.height - dayNumber.height) / 2 - priv.todayRingThickness
                        anchors.horizontalCenter: parent.horizontalCenter

                        sourceComponent: UbuntuShape {
                            objectName: "eventMarker"+index
                            aspect: UbuntuShape.Flat
                            radius: "small"
                            color: isSelected ? dayNumber.color : theme.palette.selected.baseText
                        }
                    }

                    AbstractButton {
                        anchors.fill: dayNumber
                        visible: (currentIndex == monthIndex)

                        onClicked: {
                            if (!isSelected) {
                                priv.userSelected = true
                                calendar.selectedDate = new Date(dayStart.year, dayStart.month, dayStart.day)
                            } else if (priv.userSelected) {
                                if (priv.today.getMonth().equals(month)) {
                                    calendar.selectedDate = priv.today.toDate()
                                } else {
                                    calendar.selectedDate = new Date(dayStart.year, dayStart.month)
                                }
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
