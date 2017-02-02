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

#include "utils.h"

#include "backend/backend_cups.h"
#include "cups/cupsfacade.h"

#include "models/jobmodel.h"

#include <QDebug>

JobModel::JobModel(const int updateIntervalMSecs, QObject *parent)
    : JobModel(QStringLiteral(""), new PrinterCupsBackend, updateIntervalMSecs, parent)
{
}

JobModel::JobModel(const QString &printerName, PrinterBackend *backend,
                   const int updateIntervalMSecs,
                   QObject *parent)
    : QAbstractListModel(parent)
    , m_backend(backend)
    , m_printer_name(printerName)
{
    update();
    startUpdateTimer(updateIntervalMSecs);
}

JobModel::~JobModel()
{
}

void JobModel::startUpdateTimer(const int &msecs)
{
    // Start a timer to poll for changes in the printers
    m_update_timer.setParent(this);
    connect(&m_update_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_update_timer.start(msecs);
}

void JobModel::update()
{
    qDebug() << "Updating!!!" << m_printer_name;

    // Store the old count and get the new printers
    int oldCount = m_jobs.size();
    QList<PrinterJob *> newJobs = m_backend->printerGetJobs(m_printer_name);

    qDebug() << "newJobs" << newJobs.size();

    /* If any printers returned from the backend are irrelevant, we delete
    them. This a list of indices that corresponds to printers scheduled for
    deletion in newPrinters. */
    QList<uint> forDeletion;

    // Go through the old model
    for (int i=0; i < m_jobs.count(); i++) {
        // Determine if the old printer exists in the new model
        bool exists = false;

        Q_FOREACH(PrinterJob *p, newJobs) {
            // TODO: update status here
            if (p->jobId() == m_jobs.at(i)->jobId()) {
                exists = true;
                break;
            }
        }

        // If it doesn't exist then remove it from the old model
        if (!exists) {
            beginRemoveRows(QModelIndex(), i, i);
            PrinterJob *p = m_jobs.takeAt(i);
            p->deleteLater();
            endRemoveRows();

            i--;  // as we have removed an item decrement
        }
    }

    // Go through the new model
    for (int i=0; i < newJobs.count(); i++) {
        // Determine if the new printer exists in the old model
        bool exists = false;
        int j;

        for (j=0; j < m_jobs.count(); j++) {
            if (m_jobs.at(j)->jobId() == newJobs.at(i)->jobId()) {
                exists = true;
                forDeletion << i;
                break;
            }
        }

        if (exists) {
            if (j == i) {  // New printer exists and in correct position
                continue;
            } else {
                // New printer does exist but needs to be moved in old model
                beginMoveRows(QModelIndex(), j, 1, QModelIndex(), i);
                m_jobs.move(j, i);
                endMoveRows();
            }

            // We can safely delete the newPrinter as it already exists.
            forDeletion << i;
        } else {
            // New printer does not exist insert into model
            beginInsertRows(QModelIndex(), i, i);
            m_jobs.insert(i, newJobs.at(i));
            endInsertRows();
        }
    }

    Q_FOREACH(const int &index, forDeletion) {
        newJobs.at(index)->deleteLater();
    }

    if (oldCount != m_jobs.size()) {
        Q_EMIT countChanged();
    }
}

int JobModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_jobs.size();
}

int JobModel::count() const
{
    return rowCount();
}

QVariant JobModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;

    if ((0<=index.row()) && (index.row()<m_jobs.size())) {

        auto job = m_jobs[index.row()];

        switch (role) {
        case IdRole:
            ret = job->jobId();
            break;
        case OwnerRole:
            ret = m_printer_name;
            break;
        case StateRole:
            // TODO: improve, for now have a switch
            switch (job->state()) {
            case PrinterEnum::JobState::Aborted:
                ret = "Aborted";
                break;
            case PrinterEnum::JobState::Canceled:
                ret = "Canceled";
                break;
            case PrinterEnum::JobState::Complete:
                ret = "Compelete";
                break;
            case PrinterEnum::JobState::Held:
                ret = "Held";
                break;
            case PrinterEnum::JobState::Pending:
                ret = "Pending";
                break;
            case PrinterEnum::JobState::Processing:
                ret = "Processing";
                break;
            case PrinterEnum::JobState::Stopped:
                ret = "Stopped";
                break;
            }
            break;
        case Qt::DisplayRole:
        case TitleRole:
            ret = job->title();
            break;
        }
    }

    return ret;
}

bool JobModel::setData(const QModelIndex &index,
                        const QVariant &value, int role)
{
    if ((0<=index.row()) && (index.row()<m_jobs.size())) {

        auto job = m_jobs[index.row()];

        switch (role) {

        }
    }

    return true;
}

QHash<int, QByteArray> JobModel::roleNames() const
{
    static QHash<int,QByteArray> names;

    if (Q_UNLIKELY(names.empty())) {
        names[Qt::DisplayRole] = "displayName";
        names[IdRole] = "id";
        names[OwnerRole] = "owner";
        names[StateRole] = "state";
        names[TitleRole] = "title";
        names[LastStateMessageRole] = "lastStateMessage";
    }

    return names;
}

QVariantMap JobModel::get(const int row) const
{
    QHashIterator<int, QByteArray> iterator(roleNames());
    QVariantMap result;
    QModelIndex modelIndex = index(row, 0);

    while (iterator.hasNext()) {
        iterator.next();
        result[iterator.value()] = modelIndex.data(iterator.key());
    }

    return result;
}
