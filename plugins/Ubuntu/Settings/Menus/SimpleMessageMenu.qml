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
import Ubuntu.Settings.Components 0.1 as USC

BaseMenu {
    id: menu

    property string title
    property string body
    property alias time: messageHeader.time

    property url avatar
    property url icon

    signal iconActivated
    signal dismissed

    property alias footer: footerContainer.children
    property real _animationDuration: UbuntuAnimation.FastDuration

    menuHeight: layout.height
    clip: leadingActions || trailingActions || heightAnimation.running

    Column {
        id: layout
        anchors { right: parent.right; left: parent.left }

        USC.MessageHeader {
            id: messageHeader

            avatar: menu.avatar != "" ? menu.avatar : "image://theme/contact"
            icon: menu.icon != "" ? menu.icon : "image://theme/message"
            state: menu.state
            menuStyle: menu.menuStyle
            title.text: menu.title
            body.text: menu.body

            onIconClicked:  {
                menu.iconActivated();
            }
        }

        Item {
            id: footerContainer
            anchors { right: parent.right; left: parent.left }
            width: childrenRect.width
            height: childrenRect.height
            visible: menu.state === "expanded"
            opacity: 0

            Behavior on opacity {
                NumberAnimation {
                    duration:  _animationDuration
                }
            }
        }
    }

    Behavior on menuHeight {
        NumberAnimation {
            id: heightAnimation
            duration: _animationDuration
            easing.type: Easing.OutQuad
        }
    }

    onTriggered: if (!selected) messageHeader.shakeIcon();

    states: State {
        name: "expanded"
        when: selected

        PropertyChanges {
            target: footerContainer
            opacity: 1.0
        }

        PropertyChanges {
            target: menu
            color: highlightColor
        }
    }

    onItemRemoved: {
        menu.dismissed();
    }
}
