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

import Qt.labs.folderlistmodel 2.1
import QtQuick 2.4
import QtTest 1.0
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import Ubuntu.Settings.Components 0.1
import Ubuntu.Test 0.1

MainView {
    id: testRoot
    width: units.gu(42)
    height: units.gu(75)

    Component {
        id: filePickerComp

        FilePicker {}
    }

    Component {
        id: fsModelComp

        FolderListModel {}
    }

    SignalSpy {
        id: spy
    }

    UbuntuTestCase {
        name: "FilePicker"
        when: windowShown

        property var instance: null
        property var fsModel: null

        function init() {
            fsModel = fsModelComp.createObject(testRoot);
        }

        function cleanup() {
            PopupUtils.close(instance);
            tryCompareFunction(function () {
                return !!findChild(testRoot, "filePicker");
            }, false);
            fsModel.destroy();
            spy.clear();
            spy.target = null;
            spy.signalName = "";
        }

        function test_breadCrumb_data() {
            return [
                {
                    folder: "file:///home/user/",
                    parentFolder: "file:///home",
                    count: 0,
                    targetText: i18n.dtr("ubuntu-settings-components", "%1 (%2 files)")
                        .arg(i18n.tr("Device") + "/home/user")
                        .arg(0)
                },
                {
                    folder: "file:///",
                    parentFolder: "file:///",
                    count: 10,
                    targetText: i18n.dtr("ubuntu-settings-components", "%1 (%2 files)")
                        .arg(i18n.tr("Device"))
                        .arg(10)
                },
                {
                    folder: "file:///bar/baz",
                    parentFolder: "file:///bar",
                    count: 1,
                    targetText: i18n.dtr("ubuntu-settings-components", "%1 (%2 file)")
                        .arg(i18n.tr("Device") + "/bar/baz")
                        .arg(1)
                },
            ]
        }

        function test_breadCrumb(data) {
            fsModel.folder = data.folder;
            fsModel.parentFolder = data.parentFolder;
            fsModel.count = data.count;
            instance = PopupUtils.open(filePickerComp, null, { "fsModel": fsModel });
            waitForRendering(instance);

            var crumbs = findChild(instance, "filePickerBreadcrumb");
            compare(crumbs.text, data.targetText)
        }

        function test_backButtonEnabled_data() {
            return [
                { folder: "file:///", parentFolder: "file:///", targetEnabled: false },
                { folder: "file:///bar", parentFolder: "file:///", targetEnabled: true },
                { folder: "file:///bar/baz", parentFolder: "file:///bar", targetEnabled: true },
            ]
        }

        function test_backButtonEnabled(data) {
            fsModel.folder = data.folder;
            fsModel.parentFolder = data.parentFolder;
            instance = PopupUtils.open(filePickerComp, null, { "fsModel": fsModel });
            waitForRendering(instance);

            var button = findChild(instance, "filePickerBackButton");
            compare(button.enabled, data.targetEnabled);
        }

        function test_backButtonAction_data() {
            return [
                { folder: "file:///", parentFolder: "file:///" },
                { folder: "file:///bar", parentFolder: "file:///" },
                { folder: "file:///bar/baz", parentFolder: "file:///bar" }
            ]
        }

        function test_backButtonAction(data) {
            fsModel.folder = data.folder;
            fsModel.parentFolder = data.parentFolder;
            instance = PopupUtils.open(filePickerComp, null, { "fsModel": fsModel });
            waitForRendering(instance);

            var button = findChild(instance, "filePickerBackButton");
            mouseClick(button, button.width / 2, button.height / 2);
            compare(fsModel.folder, fsModel.parentFolder);
        }

        function test_renderFiles_data() {
            return [
                { files: [ ["fileA.txt", "/home/user/fileA.txt", "", 5000, "txt", false, true] ] },
                { files: [
                    ["fileA.txt", "/home/user/fileA.txt", "", 5000, "txt", false, true],
                    ["fileB.txt", "/fileB.txt", "", 5000, "txt", false, true],
                    ["somedir", "/somedir", "somedir", 4001, "", true, false]
                ]}
            ]
        }

        function test_renderFiles(data) {
            for (var i = 0; i < data.files.length; i++) {
                fsModel.mockAddFile.apply(fsModel, data.files[i]);
            }
            instance = PopupUtils.open(filePickerComp, null, { "fsModel": fsModel });
            waitForRendering(instance);

            // Make sure each file was rendered.
            for (var i = 0; i < data.files.length; i++) {
                verify(findChild(instance, "filePickerFileItem_" + i));
            }
        }

        function test_accept_data() {
            var files = [
                ["fileA.txt", "file:///home/user/fileA.txt", "", 5000, "txt", false, true],
                ["fileB.txt", "file:///fileB.txt", "", 5000, "txt", false, true]
            ]
            return [
                { "files": files, accept: 0, targetAcceptPath: "file:///home/user/fileA.txt" },
                { "files": files, accept: 1, targetAcceptPath: "file:///fileB.txt" },
            ]
        }

        function test_accept(data) {
            for (var i = 0; i < data.files.length; i++) {
                fsModel.mockAddFile.apply(fsModel, data.files[i]);
            }
            instance = PopupUtils.open(filePickerComp, null, { "fsModel": fsModel });
            waitForRendering(instance);

            var item = findChild(instance, "filePickerFileItem_" + data.accept);
            mouseClick(item, item.width / 2, item.height / 2);

            spy.target = instance;
            spy.signalName = "accept";

            var btn = findChild(instance, "filePickerAccept");
            mouseClick(btn, btn.width / 2, btn.height / 2);
            spy.wait();
            compare(spy.count, 1);
            compare(spy.signalArguments[0][0], data.targetAcceptPath);
        }

        function test_reject() {
            instance = PopupUtils.open(filePickerComp, null, { "fsModel": fsModel });
            waitForRendering(instance);

            spy.target = instance;
            spy.signalName = "reject";

            var btn = findChild(instance, "filePickerCancel");
            mouseClick(btn, btn.width / 2, btn.height / 2);
            spy.wait();
            compare(spy.count, 1);
        }
    }
}
