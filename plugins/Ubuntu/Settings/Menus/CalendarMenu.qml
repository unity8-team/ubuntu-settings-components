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

BaseMenu {
    id: menu

    property alias currentDate: calendar.currentDate
    property alias firstDayOfWeek: calendar.firstDayOfWeek
    property alias maximumDate: calendar.maximumDate
    property alias minimumDate: calendar.minimumDate
    property alias selectedDate: calendar.selectedDate
    property alias showWeekNumbers: calendar.showWeekNumbers
    property alias eventDays: calendar.eventDays

    menuHeight: layout.height
    highlightWhenPressed: false

    Column {
        id: layout

        anchors {
            left: parent.left
            right: parent.right
        }

        ListItemLayout {
            id: monthLayout
            objectName: "monthLayout"

            padding {
                top: menuStyle.padding.top
                bottom: menuStyle.padding.bottom
                leading: menuStyle.padding.leading
                trailing: menuStyle.padding.trailing
            }

            title.horizontalAlignment: Text.AlignHCenter
            title.font.pixelSize: menuStyle.fontSize
            title.color: menuStyle.foregroundColor
            title.text: i18n.ctr("%1=month name, %2=4-digit year", "%1 %2")
                            .arg(Qt.locale().standaloneMonthName(calendar.currentDate.getMonth(), Locale.LongFormat))
                            .arg(calendar.currentDate.getFullYear())

            Icon {
                objectName: "goPreviousMonth"
                name: "go-previous"
                width: menuStyle.iconSize
                height: menuStyle.iconSize
                color: menuStyle.iconColor
                SlotsLayout.position: SlotsLayout.Leading

                AbstractButton {
                    anchors.fill: parent
                    onClicked: {
                        calendar.currentDate = calendar.currentDate.addMonths(-1)
                    }
                }
            }

            Icon {
                objectName: "goNextMonth"
                name: "go-next"
                width: menuStyle.iconSize
                height: menuStyle.iconSize
                color: menuStyle.iconColor
                SlotsLayout.position: SlotsLayout.Trailing

                AbstractButton {
                    anchors.fill: parent
                    onClicked: {
                        calendar.currentDate = calendar.currentDate.addMonths(1)
                    }
                }
            }
        }

        StyledSlotsLayout {
            id: slotsLayout
            objectName: "calenderMenuSlotsLayout"
            style: menuStyle

            mainSlot: Column {
                Calendar {
                    id: calendar
                    objectName: "calendar"
                    interactive: !pointerMode
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                }
            }
        }
    }
}
