/*
 * Copyright (C) 2017 Canonical, Ltd.
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
#include "cupspkhelpermechanism.h"

#include <QtQml/qqml.h>

static QObject* p_singletonprovider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new UbuntuSettingsPrinters;
}

void UbuntuSettingsPrintersPlugin::registerTypes(const char *uri)
{
    qmlRegisterSingletonType<UbuntuSettingsPrinters>(
        uri, 0, 1, "UbuntuSettingsPrinters", p_singletonprovider
    );
}
