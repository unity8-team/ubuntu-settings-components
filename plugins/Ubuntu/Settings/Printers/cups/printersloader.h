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

#ifndef USC_PRINTERS_CUPS_PRINTERSLOADER_H
#define USC_PRINTERS_CUPS_PRINTERSLOADER_H

class PrintersLoader : public QObject
{
    Q_OBJECT
    PrinterBackend *m_backend;
    OrgCupsCupsdNotifierInterface *m_notifier;
    bool m_running = false;
public:
    explicit PrintersLoader(PrinterBackend *backend,
                            OrgCupsCupsdNotifierInterface* notifier,
                            QObject *parent = Q_NULLPTR);
    ~PrintersLoader();

public Q_SLOTS:
    void load();
    void cancel();

Q_SIGNALS:
    void finished();
    void loaded(QList<QSharedPointer<Printer>> printers);
};

#endif // USC_PRINTERS_CUPS_PRINTERSLOADER_H
