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
 * Authored by Nick Dedekind <nick.dedekind@gmail.com>
 *             Marco Trevisan <marco.trevisan@canonical.com>
 */

import QtQuick 2.4
import Ubuntu.Components 1.3

BaseMenu {
    id: menu

    property bool playing: false
    property bool canPlay: false
    property bool canGoNext: false
    property bool canGoPrevious: false

    signal next()
    signal play(bool play)
    signal previous()

    highlightWhenPressed: false
    implicitHeight: layout.height + (divider.visible ? divider.height : 0)

    SlotsLayout {
        id: layout
        objectName: "playbackMenuLayout"

        Row {
            anchors.centerIn: parent
            spacing: units.gu(3)

            PlaybackButton {
                objectName: "previousButton"
                iconName: "media-skip-backward"
                enabled: canGoPrevious
                onClicked: menu.previous()
            }

            PlaybackButton {
                objectName: "playButton"
                iconName: playing ? "media-playback-pause" : "media-playback-start"
                enabled: canPlay
                onClicked: menu.play(!playing)
            }

            PlaybackButton {
                objectName: "nextButton"
                iconName: "media-skip-forward"
                enabled: canGoNext
                onClicked: menu.next()
            }
        }
    }
}
