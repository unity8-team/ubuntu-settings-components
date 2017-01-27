/*
 * Copyright 2016 Canonical Ltd.
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
import Ubuntu.Components 1.3
import Ubuntu.Settings.Components 0.1
import Ubuntu.Components.Popups 1.3

MainView {
    id: mainView
    // Note! applicationName needs to match the .desktop filename
    applicationName: "SettingsComponents"

    width: units.gu(42)
    height: units.gu(75)

    property var _diag
    property alias _accepted: acceptedLabel.acceptedPath

    Component {
        id: fp
        FilePicker {
            onReject: {
                if (_diag) {
                    PopupUtils.close(_diag)
                }
                _accepted = ""
            }
            onAccept: {
                _accepted = path
                if (_diag) {
                    PopupUtils.close(_diag)
                }
            }
        }
    }

    Page {
        header: PageHeader {
            id: pageHeader
            title: "FilePicker"
        }

        Column {
            spacing: units.gu(2)
            anchors {
                top: pageHeader.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: units.gu(2)
            }

            Button {
                anchors { left: parent.left; right: parent.right }
                text: i18n.tr("Select dark theme")
                onClicked: theme.name = "Ubuntu.Components.Themes.SuruDark"
            }

            Button {
                anchors { left: parent.left; right: parent.right }
                text: i18n.tr("Open file picker")
                onClicked: _diag = PopupUtils.open(fp)
            }

            Label {
                id: acceptedLabel
                property string acceptedPath
                visible: acceptedPath
                anchors { left: parent.left; right: parent.right }
                text: i18n.tr("Accepted path: %1").arg(acceptedPath)
                elide: Text.ElideMiddle
            }
        }
    }
}
