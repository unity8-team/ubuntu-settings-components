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

#include "backend/backend_pdf.h"
#include "backend/backend_cups.h"
#include "printersloader.h"

#include <QPrinterInfo>

class PrinterCupsBackend;
PrintersLoader::PrintersLoader(IppClient *client,
                               OrgCupsCupsdNotifierInterface* notifier,
                               QObject *parent)
    : QObject(parent)
    , m_client(client)
    , m_notifier(notifier)
{
}

PrintersLoader::~PrintersLoader()
{
}

void PrintersLoader::load()
{
    QList<QSharedPointer<Printer>> list;
    m_running = true;

    // Use availablePrinterNames as this gives us a name for even null printers
    Q_FOREACH(QString name, QPrinterInfo::availablePrinterNames()) {
        if (!m_running)
            break;

        QPrinterInfo info = QPrinterInfo::printerInfo(name);
        auto backend = new PrinterCupsBackend(m_client, info, m_notifier);

        // Dest or PPD was null, but we know it's name so we will use it.
        if (info.printerName().isEmpty()) {
            backend->setPrinterNameInternal(name);
        }

        auto p = QSharedPointer<Printer>(new Printer(backend));
        list.append(p);
    }

    // Cups allows a faux PDF printer.
    auto faux = QSharedPointer<Printer>(new Printer(new PrinterPdfBackend(__("Create PDF"))));
    list.append(faux);

    Q_EMIT loaded(list);
    Q_EMIT finished();
}

void PrintersLoader::cancel()
{
    m_running = false;
}
