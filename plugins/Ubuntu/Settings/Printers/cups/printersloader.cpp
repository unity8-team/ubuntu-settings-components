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

PrintersLoader::PrintersLoader(PrinterBackend *backend,
                               OrgCupsCupsdNotifierInterface* notifier,
                               QObject *parent)
    : QObject(parent)
    , m_backend(backend)
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

        if (!info.isNull()) {
            auto p = QSharedPointer<Printer>(new Printer(new PrinterCupsBackend(m_backend, info, m_notifier)));
            list.append(p);
        } else {
            qWarning() << "Printer is null so skipping (" << name << ")";
        }
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
