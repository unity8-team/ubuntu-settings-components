/*
 * Copyright (C) 2015 Canonical, Ltd.
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

// local
#include "plugin.h"
#include "serverpropertysynchroniser.h"
#include "filepickerhelper.h"

// Qt
#include <QtQml/qqml.h>

static QObject* filepickerhelperProvider(QQmlEngine*, QJSEngine*)
{
    return new FilePickerHelper;
}

void UbuntuSettingsComponentsPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<ServerPropertySynchroniser>(uri, 0, 1, "ServerPropertySynchroniser");
    qmlRegisterSingletonType<FilePickerHelper>(uri, 0, 1, "FilePickerHelper", filepickerhelperProvider);
}
