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

#include "backend/backend_cups.h"
#include "cups/cupsfacade.h"
#include "models/drivermodel.h"

#include <QDebug>
#include <QtConcurrent>

DriverModel::DriverModel(QObject *parent)
    : DriverModel(new PrinterCupsBackend, parent)
{
}

DriverModel::DriverModel(PrinterBackend *backend, QObject *parent)
    : QAbstractListModel(parent)
    , m_backend(backend)
{
    connect(m_backend, SIGNAL(printerDriversLoaded(const QList<PrinterDriver>&)),
            this, SLOT(printerDriversLoaded(const QList<PrinterDriver>&)));

    QObject::connect(&m_watcher,
                     &QFutureWatcher<PrinterDriver>::finished,
                     this,
                     &DriverModel::filterFinished);

}

DriverModel::~DriverModel()
{
}

int DriverModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_drivers.size();
}

int DriverModel::count() const
{
    return rowCount();
}

QVariant DriverModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if ((0 <= index.row()) && (index.row() < m_drivers.size())) {

        auto driver = m_drivers[index.row()];

        switch (role) {
        case Qt::DisplayRole:
            ret = driver.toString();
            break;
        case NameRole:
            ret = driver.name;
            break;
        case DeviceIdRole:
            ret = driver.deviceId;
            break;
        case LanguageRole:
            ret = driver.language;
            break;
        case MakeModelRole:
            ret = driver.makeModel;
            break;
        }
    }

    return ret;
}

QHash<int, QByteArray> DriverModel::roleNames() const
{
    static QHash<int,QByteArray> names;

    if (Q_UNLIKELY(names.empty())) {
        names[Qt::DisplayRole] = "displayName";
        names[NameRole] = "name";
        names[DeviceIdRole] = "deviceId";
        names[LanguageRole] = "language";
        names[MakeModelRole] = "makeModel";
    }

    return names;
}

void DriverModel::setFilter(const QString& pattern)
{
    QString filter = pattern.toLower();
    // QStringList filter = pattern.toLower().split(" ");
    QList<PrinterDriver> list;

    if (m_watcher.isRunning())
        m_watcher.cancel();

    if (filter.isEmpty() || filter.isNull()) {
        setModel(m_originalDrivers);
        m_filter = pattern;
        return;
    }

    if (!m_filter.isEmpty() && !m_drivers.isEmpty() &&
            pattern.startsWith(m_filter))
        list = m_drivers; // search in the smaller list
    else
        list = m_originalDrivers; //search in the whole list

    m_filter = pattern;

    QFuture<PrinterDriver> future(QtConcurrent::filtered(list,
            [filter] (const PrinterDriver &driver) {
                QByteArray search = driver.makeModel.toLower();
                return search.contains(filter.toUtf8());
            }
        )
    );

    Q_EMIT filterBegin();

    m_watcher.setFuture(future);
}

QString DriverModel::filter() const
{
    return m_filter;
}

void DriverModel::filterFinished()
{
    setModel(m_watcher.future().results());
}

void DriverModel::load()
{
    m_backend->requestAvailablePrinterDrivers();
}

void DriverModel::cancel()
{

}

void DriverModel::printerDriversLoaded(const QList<PrinterDriver> &drivers)
{
    qWarning() << Q_FUNC_INFO << drivers.size();
    m_originalDrivers = drivers;
    setModel(m_originalDrivers);
}

void DriverModel::setModel(const QList<PrinterDriver> &drivers)
{
    beginResetModel();
    m_drivers = drivers;
    endResetModel();

    Q_EMIT filterComplete();
}

// DriverMatches::DriverMatches(const QString &string)
//     : m_query(string)
// {
// }

// bool DriverMatches::operator()(const DriverModel &driver)
// {
//     return true;
// }
