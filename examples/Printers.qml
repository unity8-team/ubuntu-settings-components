/*
 * Copyright 2017 Canonical Ltd.
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
 * Authored by Jonas G. Drange <jonas.drange@canonical.com>
 */

import QtQuick 2.4
import QtQuick.Layouts 1.1
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Components.ListItems 1.3 as ListItems
import Ubuntu.Settings.Components 0.1
import Ubuntu.Settings.Printers 0.1

MainView {
    width: units.gu(50)
    height: units.gu(90)

    PrinterModel {
        id: printerModel
    }

    Component {
        id: printerPage

        Page {
            visible: false
            property var printer
            header: PageHeader {
               title: printer.name
               flickable: printerFlickable
            }

            Flickable {
                id: printerFlickable
                anchors.fill: parent

                Column {
                    spacing: units.gu(2)
                    anchors {
                        top: parent.top
                        topMargin: units.gu(2)
                        left: parent.left
                        right: parent.right
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: units.gu(2)
                        }
                        text: "Description"
                    }

                    ListItems.SingleControl {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        control: TextField {
                           anchors {
                                margins: units.gu(1)
                                left: parent.left
                                right: parent.right

                            }
                            text: printer.description
                            onTextChanged: printer.description = text
                        }
                    }


                    ListItems.ValueSelector {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Duplex"
                        values: printer.supportedDuplexModes
                        onSelectedIndexChanged: printer.duplex = values[selectedIndex]
                        Component.onCompleted: selectedIndex =
                            printer.supportedDuplexModes.indexOf(printer.duplex)
                    }

                    ListItems.ValueSelector {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Page size"
                        values: printer.supportedPageSizes
                        onSelectedIndexChanged: printer.pageSize = selectedIndex
                        Component.onCompleted: selectedIndex = printer.supportedPageSizes.indexOf(printer.pageSize)
                    }

                    ListItems.ValueSelector {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Color model"
                        values: printer.supportedColorModels
                        enabled: values.length > 1
                        onSelectedIndexChanged: printer.colorModel = selectedIndex
                        Component.onCompleted: selectedIndex = printer.supportedColorModels.indexOf(printer.colorModel)
                    }
                }
            }

        }
    }

    PageStack {
        id: pageStack

        Component.onCompleted: push(printersPage)

        Page {
            id: printersPage
            header: PageHeader {
                title: "Printers"
                flickable: printerList
            }
            visible: false

            ListView {
                id: printerList
                anchors { fill: parent }
                model: printerModel
                delegate: ListItem {
                    height: modelLayout.height + (divider.visible ? divider.height : 0)
                    ListItemLayout {
                        id: modelLayout
                        title.text: displayName
                        subtitle.text: description

                        Icon {
                            id: icon
                            width: height
                            height: units.gu(2.5)
                            name: "printer-symbolic"
                            SlotsLayout.position: SlotsLayout.First
                        }

                        ProgressionSlot {}
                    }
                    onClicked: pageStack.push(printerPage, { printer: model })
                    Component.onCompleted: console.log("printer", model.name)
                }
            }
        }
    }
}
