/*
 * Copyright (C) 2016 Canonical, Ltd.
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
 */

#include "plugin.h"
#include "ubuntusettingsfingerprint.h"
#include "fingerprintvisualprovider.h"

#include <QtQml/qqml.h>

void UbuntuSettingsFingerprintPlugin::registerTypes(const char *uri)
{

    qmlRegisterType<UbuntuSettingsFingerprint>(uri, 0, 1, "UbuntuSettingsFingerprint");
}

void UbuntuSettingsFingerprintPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
    engine->addImageProvider(QLatin1String("fingerprintvisual"),
                             new FingerprintVisualProvider);
}
