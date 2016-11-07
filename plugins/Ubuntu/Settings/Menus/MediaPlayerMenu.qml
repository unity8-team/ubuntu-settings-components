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
 * Authored by Andrea Cimitan <andrea.cimitan@canonical.com>
 *             Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3
import Ubuntu.Thumbnailer 0.1

BaseLayoutMenu {
    id: menu

    property bool showTrack: false
    property alias playerName: menu.text
    property alias playerIcon: playerIcon.source

    property alias albumArt: albumArtImage.source
    property string song
    property string artist
    property string album

    highlightWhenPressed: false
    menuHeight: albumLayout.visible ? albumLayout.height : layout.height
    Behavior on menuHeight { UbuntuNumberAnimation {} }

    layout.visible: !albumLayout.visible
    layout.objectName: "player"

    slots: Image {
        id: playerIcon
        height: menuStyle.avatarSize
        width: height
        SlotsLayout.position: SlotsLayout.Leading
    }

    ListItemLayout {
        id: albumLayout
        objectName: "albumArt"
        visible: showTrack

        padding {
            top: menuStyle.padding.top
            bottom: menuStyle.padding.bottom
            leading: menuStyle.padding.leading
            trailing: menuStyle.padding.trailing
        }

        title.text: menu.song
        title.font.pixelSize: menuStyle.fontSize
        title.color: menu.foregroundColor
        subtitle.text: menu.artist
        subtitle.color: menu.foregroundColor
        subtitle.font.pixelSize: menuStyle.fontSize
        summary.text: menu.album
        summary.font.pixelSize: menuStyle.subtitleFontSize

        UbuntuShape {
            width: units.gu(7)
            height: width
            SlotsLayout.position: SlotsLayout.Leading

            image: Image {
                id: albumArtImage
                fillMode: Image.PreserveAspectFit
                sourceSize: Qt.size(width, height)
                anchors.fill: parent
            }
        }
    }
}
