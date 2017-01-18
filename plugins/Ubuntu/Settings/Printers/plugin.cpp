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

#include "enums.h"
// #include "models/printermodel.h"
#include "printer/printer.h"
#include "printer/printerjob.h"
#include "printers/printers.h"

#include <QtQml/qqml.h>
#include <QSharedPointer>

static QObject* p_singletonprovider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new Printers;
}

void UbuntuSettingsComponentsPrintersPlugin::registerTypes(const char *uri)
{
    qmlRegisterSingletonType<Printers>(
        uri, 0, 1, "Printers", p_singletonprovider
    );

    // FIXME: do not expose this model, since QAbstractItemModel will suffice.
    qmlRegisterType<PrinterModel>(uri, 0, 1, "PrinterModel");
    // qmlRegisterUncreatableType<QAbstractItemModel>();
    qmlRegisterUncreatableType<Printer>(
        uri, 0, 1, "Printer", "use Printers to get a list of Printers."
    );
    qmlRegisterType<PrinterJob>(uri, 0, 1, "PrinterJob");

    qmlRegisterUncreatableType<PrinterEnum>(uri, 0, 1, "PrinterEnum", "Is an enum");
}
