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
                        enabled: values.length > 1
                        text: "Duplex"
                        values: printer.supportedDuplexModes
                        onSelectedIndexChanged: printer.duplexMode = selectedIndex
                        Component.onCompleted: {
                            if (enabled) {
                                selectedIndex = printer.duplexMode
                            }
                        }
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
                        visible: printer.supportedColorModels.length
                        text: "Color model"
                        values: printer.supportedColorModels
                        enabled: values.length > 1
                        onSelectedIndexChanged: printer.colorModel = selectedIndex
                        Component.onCompleted: {
                            if (enabled)
                                selectedIndex = printer.colorModel
                        }
                    }

                    ListItems.ValueSelector {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        visible: printer.supportedPrintQualities.length
                        text: "Quality"
                        values: printer.supportedPrintQualities
                        enabled: values.length > 1
                        onSelectedIndexChanged: printer.printQuality = selectedIndex
                        Component.onCompleted: {
                            if (enabled)
                                selectedIndex = printer.printQuality
                        }
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
                trailingActionBar {
                    actions: [
                        Action {
                            iconName: "add"
                            text: "Add printer"
                            onTriggered: pageStack.push(addPrinterPageComponent)
                        }
                    ]
                }
            }
            visible: false

            ListView {
                id: printerList
                anchors { fill: parent }
                model: Printers.allPrintersWithPdf
                delegate: ListItem {
                    height: modelLayout.height + (divider.visible ? divider.height : 0)
                    ListItemLayout {
                        id: modelLayout
                        title.text: displayName
                        title.font.bold: model.default
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

    Component {
        id: addPrinterPageComponent
        Page {
            id: addPrinterPage
            states: [
                State {
                    name: "success"
                    PropertyChanges {
                        target: okAction
                        enabled: false
                    }
                    PropertyChanges {
                        target: closeAction
                        enabled: false
                    }
                    PropertyChanges {
                        target: addPrinterCol
                        enabled: false
                    }
                    StateChangeScript {
                        script: okTimer.start()
                    }
                },
                State {
                    name: "failure"
                    PropertyChanges {
                        target: errorMessageContainer
                        visible: true
                    }
                }
            ]
            header: PageHeader {
                title: "Add printer"
                flickable: addPrinterFlickable
                leadingActionBar.actions: [
                    Action {
                        id: closeAction
                        iconName: "close"
                        text: "Abort"
                        onTriggered: pageStack.pop()
                    }
                ]
                trailingActionBar {
                    actions: [
                        Action {
                            id: okAction
                            iconName: "ok"
                            text: "Complete"
                            onTriggered: {
                                var ret = Printers.addPrinterWithPpdFile(
                                    printerName.text,
                                    printerPpd.text,
                                    printerUri.text,
                                    printerDescription.text,
                                    printerLocation.text
                                );
                                if (ret) {
                                    addPrinterPage.state = "success"
                                } else {
                                    errorMessage.text = Printers.lastMessage;
                                    addPrinterPage.state = "failure"
                                }
                            }
                        }
                    ]
                }
            }

            Timer {
                id: okTimer
                interval: 2000
                onTriggered: pageStack.pop();
            }

            Flickable {
                id: addPrinterFlickable
                anchors.fill: parent

                Column {
                    id: addPrinterCol
                    property bool enabled: true
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    spacing: units.gu(2)

                    Item {
                        id: errorMessageContainer
                        visible: false
                        anchors {
                            left: parent.left
                            right: parent.right
                            margins: units.gu(2)
                        }
                        height: units.gu(6)
                        Label {
                            id: errorMessage
                            anchors {
                                top: parent.top
                                topMargin: units.gu(2)
                                horizontalCenter: parent.horizontalCenter
                            }
                        }

                    }

                    ListItems.ValueSelector {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Choose driver"
                        values: [
                            "Provide PPD file",
                            "Select printer from database"
                        ]
                        enabled: false
                    }

                    ListItems.Standard {
                        text: "Device URI"
                        control: TextField {
                            id: printerUri
                            placeholderText: "e.g. ipp://server.local/my-queue"
                        }
                        enabled: parent.enabled
                    }

                    ListItems.Standard {
                        text: "PPD File"
                        control: TextField {
                            id: printerPpd
                            placeholderText: "e.g. /usr/share/cups/foo.ppd"
                        }
                        enabled: parent.enabled
                    }

                    ListItems.Standard {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Printer name"
                        control: TextField {
                            id: printerName
                            placeholderText: "e.g. laserjet"
                        }
                        enabled: parent.enabled
                    }

                    ListItems.Standard {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Description (optional)"
                        control: TextField {
                            id: printerDescription
                            placeholderText: "e.g. HP Laserjet with Duplexer"
                        }
                        enabled: parent.enabled
                    }

                    ListItems.Standard {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        text: "Location (optional)"
                        control: TextField {
                            id: printerLocation
                            placeholderText: "e.g. Lab 1"
                        }
                        enabled: parent.enabled
                    }
                }
            }
        }
    }
}
