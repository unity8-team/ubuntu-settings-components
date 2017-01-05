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

#ifndef USC_JOB_MODEL_H
#define USC_JOB_MODEL_H

#include "job/job.h"

#include <QAbstractListModel>
#include <QByteArray>
#include <QDBusConnection>
#include <QModelIndex>
#include <QObject>
#include <QVariant>
#include <QVector>

class JobModelPrivate;
class JobModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(JobModel)
public:
    explicit JobModel(QObject *parent = 0);
    explicit JobModel(QDBusConnection &dbus, QObject *parent = 0);
    ~JobModel();

    enum class Roles
    {
        // Qt::DisplayRole holds job name
        PrinterNameRole = Qt::UserRole,
        StartedByUserNameRole,
        TimeAtCreationRole,
        JobStateRole,
        AuthRequiredRole,
        JobRole,
        PrinterRole,
        LastRole = PrinterRole,
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    QSharedPointer<Printer> getPrinterFromName(const QString &name);
};

class JobFilterPrivate;
class JobFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(JobFilter)
public:
    explicit JobFilter() {}
    virtual ~JobFilter() {}
    void filterOnState(const Job::State &state);

protected:
    virtual bool filterAcceptsRow(int, const QModelIndex&) const;
    virtual bool lessThan(const QModelIndex&, const QModelIndex&) const;
};

#endif // USC_JOB_MODEL_H
