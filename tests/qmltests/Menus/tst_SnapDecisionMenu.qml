/*
 * Copyright 2013 Canonical Ltd.
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
 * Authored by Nick Dedekind <nick.dedekind@canonical.com>
 */

import QtQuick 2.4
import QtTest 1.0
import Ubuntu.Test 0.1
import Ubuntu.Settings.Menus 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    Flickable {
        id: flickable

        anchors.fill: parent
        contentWidth: column.width
        contentHeight: column.height

        Item {
            id: column

            width: flickable.width
            height: childrenRect.height

            SnapDecisionMenu {
                id: messageMenu
                removable: false

                title: "Text Message"
                body: "I am a little teapot"
                time: "11:08am"
                _animationDuration: 0
            }

            SnapDecisionMenu {
                id: messageMenuRemovable
                removable: true
                anchors.top: messageMenu.bottom
                _animationDuration: 0
            }

            SnapDecisionMenu {
                id: messageMenuSelected
                removable: true
                selected: true
                anchors.top: messageMenuRemovable.bottom
                _animationDuration: 0

                onReplied: {
                    textMessageReply = value;
                }
            }
        }
    }

    property string textMessageReply: ""

    SignalSpy {
        id: signalSpyIconActivated
        signalName: "iconActivated"
        target: messageMenuSelected
    }

    SignalSpy {
        id: signalSpyDismiss
        signalName: "dismissed"
        target: messageMenuRemovable
    }

    SignalSpy {
        id: signalSpyActionActivated
        signalName: "actionActivated"
        target: messageMenuSelected
    }

    SignalSpy {
        id: signalSpyReply
        signalName: "replied"
        target: messageMenuSelected
    }

    SignalSpy {
        id: signalSpyTriggered
        signalName: "triggered"
        target: messageMenuSelected
    }

    UbuntuTestCase {
        name: "SnapDecisionMenu"
        when: windowShown

        function cleanup() {
            textMessageReply = "";
            messageMenu.replyEnabled = true;
            messageMenuSelected.selected = false;

            signalSpyIconActivated.clear();
            signalSpyDismiss.clear();
            signalSpyActionActivated.clear();
            signalSpyReply.clear();
            signalSpyTriggered.clear();

            messageMenu.replyExpanded = false;
            var replyText = findChild(messageMenu, "replyText");
            verify(replyText !== undefined, "Reply text not found");
            replyText.text = "";

            messageMenuRemovable.replyExpanded = false;
            replyText = findChild(messageMenuRemovable, "replyText");
            verify(replyText !== undefined, "Reply text not found");
            replyText.text = "";

            messageMenuSelected.replyExpanded = false;
            replyText = findChild(messageMenuSelected, "replyText");
            verify(replyText !== undefined, "Reply text not found");
            replyText.text = "";
        }

        function test_title_data() {
            return [
                { title: "title1" },
                { title: "title2" },
            ];
        }

        function test_title(data) {
            messageMenu.title = data.title;

            var title = findChild(messageMenu, "title");
            verify(title, "No title");
            compare(title.text, data.title, "Title does not match set title.");
        }

        function test_time_data() {
            return [
                { time: "11:09am" },
                { time: "4pm" },
            ];
        }

        function test_time(data) {
            messageMenu.time = data.time;

            var subtitle = findChild(messageMenu, "time");
            verify(subtitle !== undefined, "No time");
            compare(subtitle.text, data.time, "Time does not match set time.");
        }

        function test_avatar_data() {
            return [
                { avatar: Qt.resolvedUrl("../../artwork/avatar.png") },
                { avatar: Qt.resolvedUrl("../../artwork/rhythmbox.png") },
            ];
        }

        function test_avatar(data) {
            messageMenu.avatar = data.avatar;

            var avatar = findChild(messageMenu, "avatar");
            verify(avatar !== undefined, "No avatar");
            compare(avatar.source, data.avatar, "Avatar does not match set avatar.");
        }

        function test_icon_data() {
            return [
                { icon: Qt.resolvedUrl("../../artwork/avatar.png") },
                { icon: Qt.resolvedUrl("../../artwork/rhythmbox.png") },
            ];
        }

        function test_icon(data) {
            messageMenu.icon = data.icon;

            var icon = findChild(messageMenu, "icon");
            verify(icon !== undefined, "No icon");
            compare(icon.source, data.icon, "Icon does not match set icon.");
        }

        function test_body_data() {
            return [
                { body: "This is a test." },
                { body: "Test is also a test." },
            ];
        }

        function test_body(data) {
            messageMenu.body = data.body;

            var body = findChild(messageMenu, "body");
            verify(body !== undefined, "No body");
            compare(body.text, data.body, "Message does not match set message.");
        }

        function test_iconActivated() {
            var icon = findChild(messageMenuSelected, "icon");

            mouseClick(icon, icon.width / 2, icon.height / 2);
            compare(signalSpyIconActivated.count > 0, true, "activate icon should have been triggered");
        }

        function test_dismiss() {
            verify(findChild(messageMenuRemovable, "removeAction") === undefined);
            mouseFlick(messageMenuRemovable,
                       messageMenuRemovable.width / 2,
                       messageMenuRemovable.height / 2,
                       messageMenuRemovable.width,
                       messageMenuRemovable.height / 2,
                       true, true, units.gu(1), 10);
            var removeAction = findChild(messageMenuRemovable, "removeAction");
            verify(removeAction !== undefined);
            tryCompare(removeAction, "visible", true)
            mouseClick(removeAction, removeAction.width/2, removeAction.height/2)
            tryCompareFunction(function() { return signalSpyDismiss.count > 0; }, true);
        }

        function test_replyButtonText_data() {
            return [
                { buttonText: "Send" },
                { buttonText: "reply" },
            ];
        }

        function test_replyButtonText(data) {
            messageMenu.replyButtonText = data.buttonText;

            var button = findChild(messageMenu, "sendButton");
            verify(button !== undefined, "No send button");
            compare(button.text, data.buttonText, "Button text does not match set text.");
        }

        function test_activateEnabled() {
            messageMenuSelected.selected = true;
            messageMenuSelected.actionEnabled = false;

            var actionButton = findChild(messageMenuSelected, "actionButton");
            verify(actionButton !== undefined, "Action button not found");

            compare(actionButton.enabled, false, "Action button should not be enabled when activateEnabled=false");
            messageMenuSelected.actionEnabled = true
            compare(actionButton.enabled, true, "Action button should be enabled when activateEnabled=true");
        }

        function test_actionActivated() {
            messageMenuSelected.selected = true;
            messageMenuSelected.actionEnabled = true;

            var actionButton = findChild(messageMenuSelected, "actionButton");
            verify(actionButton !== undefined, "Action button not found");

            mouseClick(actionButton, actionButton.width / 2, actionButton.height / 2);
            compare(signalSpyActionActivated.count > 0, true);
        }

        function test_replyEnabled_data() {
            return [
                { tag: 'disabledNoReply', enabled: false, reply: "", expected: false},
                { tag: 'enabledNoReply', enabled: true, reply: "", expected: false},
                { tag: 'disabledWithReply', enabled: false, reply: "test", expected: false},
                { tag: 'enabledWithReply', enabled: true, reply: "test", expected: true},
            ];
        }

        function test_replyEnabled(data) {
            messageMenuSelected.selected = true;
            messageMenuSelected.replyEnabled = data.enabled

            var replyText = findChild(messageMenuSelected, "replyText");
            verify(replyText !== undefined, "Reply text not found");
            replyText.text = data.reply;

            var messageButton = findChild(messageMenuSelected, "messageButton");
            verify(messageButton !== undefined, "Message button not found");
            mouseClick(messageButton, messageButton.width / 2, messageButton.height / 2);

            var sendButton = findChild(messageMenuSelected, "sendButton");
            verify(sendButton !== undefined, "Send button not found");

            compare(sendButton.enabled, data.expected, "Reply button is not in correct state");
        }

        function test_reply() {
            messageMenuSelected.selected = true;
            messageMenuSelected.replyEnabled = true;

            var replyText = findChild(messageMenuSelected, "replyText");
            verify(replyText !== undefined, "Reply text not found");
            replyText.text = "reply1";

            var messageButton = findChild(messageMenuSelected, "messageButton");
            verify(messageButton !== undefined, "Message button not found");
            waitForRendering(messageButton)
            mouseClick(messageButton, messageButton.width / 2, messageButton.height / 2);

            var sendButton = findChild(messageMenuSelected, "sendButton");
            verify(sendButton !== undefined, "Send button not found");
            waitForRendering(sendButton)

            mouseClick(sendButton, messageButton.width / 2, messageButton.height / 2);
            compare(signalSpyReply.count > 0, true);
            compare(textMessageReply, "reply1", "Text message did not reply with correct text.");
        }

        function test_reply_with_message_data() {
            return [
                { tag: 'reply1', index: 0, expected: "reply1" },
                { tag: 'reply2', index: 1, expected: "reply2" },
                { tag: 'reply3', index: 2, expected: "reply3" }
            ]
        }

        function test_multipleLineReply_lp1396058() {
            messageMenuSelected.selected = true;
            messageMenuSelected.replyEnabled = true;

            var replyText = findChild(messageMenuSelected, "replyText");
            verify(replyText !== undefined, "Reply text not found");

            var messageButton = findChild(messageMenuSelected, "messageButton");
            verify(messageButton !== undefined, "Message button not found");
            waitForRendering(messageButton)
            mouseClick(messageButton, messageButton.width / 2, messageButton.height / 2);

            mouseClick(replyText, replyText.width / 2, replyText.height / 2);
            compare(replyText.focus, true, "Reply text should have focus after mouse click");

            keyClick("a"); keyClick("b");
            keyClick(Qt.Key_Return);
            keyClick("c"); keyClick("d");

            compare(signalSpyTriggered.count, 0, "Item should not have triggered on 'return'")

            compare(replyText.focus, true, "Reply text should still have focus after return pressed");
            compare(replyText.text, "ab\ncd");
        }
    }
}
