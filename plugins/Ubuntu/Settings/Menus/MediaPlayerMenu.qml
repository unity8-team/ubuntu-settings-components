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
import QtQuick.Layouts 1.1
import Ubuntu.Thumbnailer 0.1

BaseLayoutMenu {
    id: menu

    property bool showTrack: false
    property alias playerName: menu.text
    property alias playerIcon: playerIcon.source

    property alias albumArt: albumArtImage.source
    property alias song: songLabel.text
    property alias artist: artistLabel.text
    property alias album: albumLabel.text

    highlightWhenPressed: false
    height: (albumLayout.visible ? albumLayout.height : layout.height) + (divider.visible ? divider.height : 0)

    layout.visible: !albumLayout.visible
    slots: Image {
        id: playerIcon
        height: units.gu(5)
        width: height
        SlotsLayout.position: SlotsLayout.Leading
    }

    SlotsLayout {
        id: albumLayout
        visible: showTrack
        mainSlot: Column {
            Label {
                id: songLabel
                elide: Text.ElideRight
                maximumLineCount: 1
                visible: text !== ""
                anchors { left: parent.left; right: parent.right }
            }

            Label {
                id: artistLabel
                elide: Text.ElideRight
                maximumLineCount: 1
                visible: text !== ""
                anchors { left: parent.left; right: parent.right }
            }

            Label {
                id: albumLabel
                elide: Text.ElideRight
                maximumLineCount: 1
                fontSize: "small"
                visible: text !== ""
                anchors { left: parent.left; right: parent.right }
            }
        }

        UbuntuShape {
            width: units.gu(8)
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
