/*
 * Copyright (C) 2015 Canonical, Ltd.
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

// local
#include "plugin.h"
#include "serverpropertysynchroniser.h"

// Qt
#include <QtQml/qqml.h>
#include <QDate>

class QtDateFunctions : public QObject
{
    Q_OBJECT
public:
    QtDateFunctions(QObject * parent = nullptr) : QObject(parent) {}
    ~QtDateFunctions() = default;

    Q_INVOKABLE int weekNumber(const QDate &date) const { return date.weekNumber(); }
    Q_INVOKABLE QString formattedWeekNumber(const QDate &date) const { return QString("%1").arg(date.weekNumber(), 2, 10, QChar('0')); }
};

void UbuntuSettingsComponentsPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<ServerPropertySynchroniser>(uri, 0, 1, "ServerPropertySynchroniser");
    qmlRegisterSingletonType<QtDateFunctions>(uri, 0, 1, "QtDateFunctions",
                                              [](QQmlEngine*, QJSEngine*) -> QObject* { return new QtDateFunctions; });
}

#include "plugin.moc"
