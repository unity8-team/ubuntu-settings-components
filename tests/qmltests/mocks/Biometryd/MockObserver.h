/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MOCK_OBSERVER_H
#define MOCK_OBSERVER_H

#include <QObject>
#include <QString>
#include <QVariant>

class MockObserver : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MockObserver)

public:
    explicit MockObserver(QObject *parent = 0);

    Q_INVOKABLE void mockSize(int size, const QString &error); // only in mock
    Q_INVOKABLE void mockEnroll(const QString &error); // only in mock
    Q_INVOKABLE void mockEnrollProgress(double percent,
                                        const QVariantMap& details); // only in mock
    Q_INVOKABLE void mockClearance(const QString &error); // only in mock

Q_SIGNALS:
    void progressed(double percent, const QVariantMap& details);
    void succeeded(const QVariant &result);
    void failed(const QString &reason);
    void canceled(const QString& reason);
    void started();
};

#endif // MOCK_OBSERVER_H
