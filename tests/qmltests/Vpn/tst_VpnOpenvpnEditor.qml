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
import QtTest 1.0
import Ubuntu.Test 0.1
import Ubuntu.Settings.Vpn 0.1

Item {
    width: units.gu(42)
    height: units.gu(75)

    VpnEditor {
        anchors.fill: parent
        id: vpnEditor
    }

    UbuntuTestCase {
        name: "VpnOpenvpnEditor"
        when: windowShown

        function getLoader() {
            return findChild(vpnEditor, "editorLoader");
        }

        function init () {
            waitForRendering(vpnEditor);
            vpnEditor.connection = {
                id: "openvpn connection",
                type: 0, // openvpn
                remote: "",
                portSet: false,
                port: 1194,
                neverDefault: false,
                protoTcp: true,
                cert: "cert.pem",
                ca: "cert.ca",
                key: "cert.key",
                certPass: "foo",
                ta: "cert.ta",
                taSet: true,
                taDir: 1,
                remoteCertTlsSet: true,
                remoteCertTls: 0,
                cipher: 8,
                compLzo: true,
                updateSecrets: function () {}
            }
            vpnEditor.render();

            // Wait until we've loaded
            tryCompareFunction(function () { return typeof getLoader() }, "object");
            tryCompareFunction(function () { return getLoader().status }, Loader.Ready);
        }

        function test_fields() {
            var e                 = getLoader().item;
            var c                 = vpnEditor.connection;
            var serverField       = findChild(e, "vpnOpenvpnServerField");
            var portField         = findChild(e, "vpnOpenvpnPortField");
            var portToggle        = findChild(e, "vpnOpenvpnCustomPortToggle");
            var routesField       = findChild(e, "vpnOpenvpnRoutesField");
            var tcpToggle         = findChild(e, "vpnOpenvpnTcpToggle");
            var udpToggle         = findChild(e, "vpnOpenvpnUdpToggle");
            var certField         = findChild(e, "vpnOpenvpnCertField");
            var caField           = findChild(e, "vpnOpenvpnCaField");
            var keyField          = findChild(e, "vpnOpenvpnKeyField");
            var certPassField     = findChild(e, "vpnOpenvpnCertPassField");
            var taSetToggle       = findChild(e, "vpnOpenvpnTaSetToggle");
            var taField           = findChild(e, "vpnOpenvpnTaField");
            var taDirSelector     = findChild(e, "vpnOpenvpnTaDirSelector");
            var certSetToggle     = findChild(e, "vpnOpenvpnRemoteCertSetToggle");
            var certTlsSelector   = findChild(e, "vpnOpenvpnRemoteCertTlsSelector");
            var cipherSelector    = findChild(e, "vpnOpenvpnCipherSelector");
            var compressionToggle = findChild(e, "vpnOpenvpnCompressionToggle");

            compare(serverField.text,              c.remote);
            compare(parseInt(portField.text, 10),  c.port);
            compare(portToggle.checked,            c.portSet);
            compare(routesField.neverDefault,      c.neverDefault);
            compare(tcpToggle.checked,             c.protoTcp);
            compare(udpToggle.checked,             !c.protoTcp);
            compare(certField.path,                c.cert);
            compare(caField.path,                  c.ca);
            compare(keyField.path,                 c.key);
            compare(certPassField.text,            c.certPass);
            compare(taSetToggle.checked,           c.taSet);
            compare(taField.path,                  c.ta);
            compare(taDirSelector.selectedIndex,   c.taDir);
            compare(certSetToggle.checked,         c.remoteCertTlsSet);
            compare(certTlsSelector.selectedIndex, c.remoteCertTls);
            compare(cipherSelector.selectedIndex,  c.cipher);
            compare(compressionToggle.checked,     c.compLzo);
        }

        // Test that the two checkboxes function as radio button
        function test_tcp_udp_toggle() {
            var e         = getLoader().item;
            var c         = vpnEditor.connection;
            var tcpToggle = findChild(e, "vpnOpenvpnTcpToggle");
            var udpToggle = findChild(e, "vpnOpenvpnUdpToggle");
            compare(tcpToggle.checked, c.protoTcp);
            compare(udpToggle.checked, !c.protoTcp);

            mouseClick(udpToggle, udpToggle.width / 2, udpToggle.height / 2);
            compare(udpToggle.checked, true);
            compare(tcpToggle.checked, false);
        }

        function test_changes() {
            var e           = getLoader().item;
            var c           = vpnEditor.connection;
            var serverField = findChild(e, "vpnOpenvpnServerField");
            var portField   = findChild(e, "vpnOpenvpnPortField");
            var portToggle  = findChild(e, "vpnOpenvpnCustomPortToggle");

            compare(e.getChanges(), []);

            // make some changes
            serverField.text = "new remote";
            portToggle.checked = !c.portSet;
            portField.text = "50000";

            compare(e.getChanges(), [
                ["remote", serverField.text],
                ["portSet", portToggle.checked],
                ["port", parseInt(portField.text, 10)],
            ]);
        }

        function test_validity() {
            compare(getLoader().item.valid, true);
        }

        function test_route_toggle() {
            var all = findChild(vpnEditor, "vpnAllNetworksToggle");
            var own = findChild(vpnEditor, "vpnOwnNetworksToggle");

            verify(all.checked);
            verify(!own.checked);

            mouseClick(own, own.width / 2, own.height / 2);

            verify(!all.checked);
            verify(own.checked);
        }

        function test_file_selector() {
            var selector = findChild(vpnEditor, "vpnOpenvpnCaField");
            var noop = function() {}
            selector.__dialog = {
                accept: { connect: noop, disconnect: noop },
                reject: { connect: noop, disconnect: noop },
                hide: noop
            };

            compare(selector.model.length, 3);
            compare(selector.model[0], i18n.tr("None"));
            compare(selector.model[1], "cert.ca");
            compare(selector.model[2], i18n.tr("Choose Certificate…"));

            selector.pathAccepted("");
            compare(selector.model.length, 2);
            compare(selector.model[0], i18n.tr("None"));
            compare(selector.model[1], i18n.tr("Choose Certificate…"));
        }
    }
}
