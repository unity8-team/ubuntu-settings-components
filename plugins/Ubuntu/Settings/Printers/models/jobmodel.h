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

#include "printers_global.h"

#include "printer/printer.h"

#include <QAbstractListModel>
#include <QByteArray>
#include <QModelIndex>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QVariant>

class PRINTERS_DECL_EXPORT JobModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit JobModel(const int updateIntervalMSecs=5000, QObject *parent = Q_NULLPTR);
    explicit JobModel(const QString &printerName, PrinterBackend *backend, const int updateIntervalMSecs=5000,
                      QObject *parent = Q_NULLPTR);
    ~JobModel();

    enum Roles
    {
        // Qt::DisplayRole holds job title
        IdRole = Qt::UserRole,
        OwnerRole,
        StateRole,
        TitleRole,
        LastStateMessageRole,
        LastRole = LastStateMessageRole,
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int count() const;

    Q_INVOKABLE QVariantMap get(const int row) const;
private:
    QTimer m_update_timer;
    PrinterBackend *m_backend;
    QString m_printer_name;

    QList<QSharedPointer<PrinterJob>> m_jobs;
private Q_SLOTS:
    void startUpdateTimer(const int &msecs);
    void update();

Q_SIGNALS:
    void countChanged();
};

#endif // USC_JOB_MODEL_H
