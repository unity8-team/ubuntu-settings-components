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
 *      Renato Araujo Oliveira Filho <renato@canonical.com>
 *      Olivier Tilloy <olivier.tilloy@canonical.com>
 *      Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

Item {
    id: messageHeader

    property alias avatar: avatarImage.source
    property alias icon: iconImage.source
    property alias time: timeText.text
    property alias title: itemLayout.title
    property alias body: itemLayout.summary

    signal iconClicked()

    implicitHeight: itemLayout.height
    anchors { right: parent.right; left: parent.left }

    function shakeIcon() {
        shake.restart();
    }

    ListItemLayout {
        id: itemLayout

        UbuntuShape {
            width: units.gu(6)
            height: width

            SlotsLayout.position: SlotsLayout.Leading

            source: ShaderEffectSource {
                sourceItem: avatarImage
                hideSource: true
            }
            Icon {
                id: avatarImage
                objectName: "avatar"
                color: Qt.rgba(0.0, 0.0, 0.0, 0.0)

                Binding on color {
                    when: String(avatarImage.source).match(/^image:\/\/theme/)
                    value: theme.palette.normal.backgroundText
                }
            }
        }

        title.objectName: "title"
        title.font.weight: Font.DemiBold
        title.anchors.rightMargin: timeText.width > iconImage.width ? timeText.width - iconImage.width : 0

        summary.objectName: "body"
        summary.maximumLineCount: 3
        summary.wrapMode: Text.WordWrap
        summary.elide: Text.ElideRight

        Icon {
            id: iconImage
            objectName: "icon"
            color: theme.palette.normal.backgroundText
            width: units.gu(3)
            height: width
            SlotsLayout.position: SlotsLayout.Trailing
            SlotsLayout.overrideVerticalPositioning: true

            Binding on y {
                when: timeText.text.length
                value: itemLayout.title.y + itemLayout.title.baselineOffset + itemLayout.title.height
            }

            Binding on anchors.verticalCenter {
                when: !timeText.text.length
                value: itemLayout.verticalCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: messageHeader.iconClicked()
            }

            SequentialAnimation {
                id: shake
                PropertyAnimation { target: iconImage; property: "rotation"; duration: 50; to: -20 }
                SpringAnimation { target: iconImage; property: "rotation"; from: -20; to: 0; mass: 0.5; spring: 15; damping: 0.1 }
            }
        }
    }

    Label {
        id: timeText
        objectName: "time"
        anchors {
            top: parent.top
            right: parent.right
            topMargin: itemLayout.padding.top
            rightMargin: itemLayout.padding.trailing
        }

        fontSize: "x-small"
        maximumLineCount: 1
    }
}
