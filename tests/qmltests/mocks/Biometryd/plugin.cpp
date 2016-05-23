/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "plugin.h"

#include "MockDevice.h"
#include "MockFingerprintReader.h"
#include "MockObserver.h"
#include "MockOperation.h"
#include "MockService.h"
#include "MockTemplateStore.h"
#include "MockUser.h"

#include <QtQml>

void BackendPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Biometryd"));

    qmlRegisterType<MockObserver>(uri, 0, 0, "Observer");
    qmlRegisterType<MockUser>(uri, 0, 0, "User");

    qmlRegisterUncreatableType<MockDevice>(uri, 0, 0, "Device", "Rely on Biometryd.instance");

    qmlRegisterUncreatableType<MockOperation>(uri, 0, 0, "Operation", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<MockSizeQuery>(uri, 0, 0, "SizeQuery", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<MockEnrollment>(uri, 0, 0, "Enrollment", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<MockClearance>(uri, 0, 0, "Clearance", "Rely on Biometryd.instance");
    qmlRegisterUncreatableType<MockTemplateStore>(uri, 0, 0, "TemplateStore", "Rely on Biometryd.instance");

    qmlRegisterSingletonType<MockService>(
        uri, 0, 0, "Biometryd",
        [](QQmlEngine*, QJSEngine*) -> QObject*
        {
            return new MockService;
        }
    );
    qmlRegisterSingletonType<MockFingerprintReader>(
        uri, 0, 0, "FingerprintReader",
        [](QQmlEngine*, QJSEngine*) -> QObject*
        {
            return new MockFingerprintReader;
        }
    );
}
