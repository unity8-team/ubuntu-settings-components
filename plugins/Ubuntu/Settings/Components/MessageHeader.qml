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
    property alias title: titleText.text
    property alias time: timeText.text
    property alias body: bodyText.text
    property QtObject menuStyle

    signal iconClicked()

    height: slotsLayout.height
    anchors { right: parent.right; left: parent.left }

    function shakeIcon() {
        shake.restart();
    }

    SlotsLayout {
        id: slotsLayout

        padding {
            top: menuStyle.padding.top
            bottom: menuStyle.padding.bottom
            leading: menuStyle.padding.leading
            trailing: menuStyle.padding.trailing
        }

        UbuntuShape {
            width: menuStyle.avatarSize
            height: width

            SlotsLayout.position: SlotsLayout.Leading

            source: ShaderEffectSource {
                sourceItem: avatarImage
                hideSource: true
            }
            Icon {
                id: avatarImage
                objectName: "avatar"

                color: {
                    if (String(source).match(/^image:\/\/theme/)) {
                        return theme.palette.normal.backgroundText;
                    }
                    return Qt.rgba(0.0, 0.0, 0.0, 0.0);
                }
            }
        }

        mainSlot: Column {
            spacing: units.gu(0.5)

            Label {
                id: titleText
                objectName: "title"

                maximumLineCount: 1
                elide: Text.ElideRight
                font.weight: Font.DemiBold
                font.pixelSize: menuStyle.fontSize
                anchors { left: parent.left; }

                // XXX: We need to resize the title not to cover the time
                width: {
                    if (parent.width && timeText.width > iconImage.width)
                        return parent.width + iconImage.width - timeText.width;
                    return parent.width
                }
            }

            Label {
                id: bodyText
                objectName: "body"

                maximumLineCount: 3
                wrapMode: Text.WordWrap
                elide: Text.ElideRight
                font.pixelSize: menuStyle.subtitleFontSize
                anchors { left: parent.left; right: parent.right }
            }
        }

        Icon {
            id: iconImage
            objectName: "icon"
            color: theme.palette.normal.backgroundText
            width: units.gu(3)
            height: width
            SlotsLayout.position: SlotsLayout.Trailing
            SlotsLayout.overrideVerticalPositioning: true
            anchors.verticalCenter: parent.verticalCenter

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
            topMargin: slotsLayout.padding.top
            rightMargin: slotsLayout.padding.trailing
        }

        fontSize: "x-small"
        maximumLineCount: 1
    }
}
