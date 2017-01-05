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

#ifndef USC_JOB_H
#define USC_JOB_H

#include "cancelobservable.h"

#include <QtCore/QObject>

class JobPrivate;
class Job : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Job)
    Q_PROPERTY(QString jobName READ jobName CONSTANT)
    Q_PROPERTY(QString printerName READ printerName CONSTANT)
    Q_PROPERTY(QString startedByUser READ startedByUser CONSTANT)
    Q_PROPERTY(QDateTime timeAtCreation READ timeAtCreation CONSTANT)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool authRequired READ authRequired CONSTANT)

public:
    explicit Job(QObject *parent = 0);
    ~Job();

    enum class State
    {
        JobPending,
        JobProcessing,
        JobStopped,
        JobCanceled,
        JobAborted,
        JobCompleted,
    };
    Q_ENUM(State)

    QString jobName() const;
    QString printerName() const;
    QString startedByUser() const;
    QDateTime timeAtCreation() const;
    State state() const;
    bool authRequired() const;

public slots:
    CancelObservable* cancel();

Q_SIGNALS:
    void stateChanged();
}

#endif // USC_JOB_H
