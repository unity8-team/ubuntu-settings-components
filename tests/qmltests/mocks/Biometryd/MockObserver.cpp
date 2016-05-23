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

#include "MockObserver.h"

class Result : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int uid READ uid CONSTANT)

public:
    explicit Result(unsigned int uid, QObject *parent = 0)
        : QObject(parent), m_uid(uid)
    {}

    unsigned int uid() const { return m_uid; }

private:
    unsigned int m_uid;
};

MockObserver::MockObserver(QObject *parent)
    : QObject(parent)
{
}

void MockObserver::mockSize(int size, const QString &error)
{
    if (error.isEmpty())
        Q_EMIT succeeded(QVariant::fromValue(size));
    else
        Q_EMIT failed(error);
}

void MockObserver::mockEnroll(const QString &error)
{
    if (error.isEmpty())
        Q_EMIT succeeded(QVariant());
    else
        Q_EMIT failed(error);
}

void MockObserver::mockEnrollProgress(double percent,
                                      const QVariantMap& details)
{
    Q_EMIT progressed(percent, details);
}

void MockObserver::mockClearance(const QString &error)
{
    if (error.isEmpty())
        Q_EMIT succeeded(QVariant());
    else
        Q_EMIT failed(error);
}

#include "MockObserver.moc"
