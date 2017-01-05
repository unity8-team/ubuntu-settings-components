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

#ifndef USC_CANCELOBSERVABLE_H
#define USC_CANCELOBSERVABLE_H

#include <QtCore/QObject>

class CancelObservablePrivate;
class CancelObservable : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CancelObservable)
public:
    explicit CancelObservable(QObject* parent = 0);

    // started is emitted when the state changes to started.
    Q_SIGNAL void started();
    // progressed is emitted when the overall operation progresses towards completion.
    // percent Overall completion status of the operation, in [0,1], -1 indicates indeterminate.
    // details Additional details about the operation.
    Q_SIGNAL void progressed(double percent, const QVariantMap& details);
    // canceled is emitted when the operation has been canceled.
    // reason The human readable reason for cancelling the operation.
    Q_SIGNAL void canceled(const QString& reason);
    // failed is emitted when the operation fails to complete.
    // reason The human readable reason for the failure.
    Q_SIGNAL void failed(const QString& reason);
    // succeeded is emitted when the operation completes successfully.
    // result The result of the operation, might be empty.
    Q_SIGNAL void succeeded(const QVariant& result);
};

#endif // USC_CANCELOBSERVABLE_H
