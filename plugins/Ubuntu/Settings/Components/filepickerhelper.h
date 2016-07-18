/*
 * Copyright (C) 2016 Canonical, Ltd.
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

#ifndef FILEPICKERHELPER_H
#define FILEPICKERHELPER_H

#include <QObject>
#include <QUrl>
#include <QStringList>

class FilePickerHelper : public QObject
{
    Q_OBJECT
public:
    FilePickerHelper(QObject* parent = nullptr);
    Q_INVOKABLE static QUrl homeDirUrl();
    Q_INVOKABLE static QString rootPath();
    Q_INVOKABLE static QString pathFromUrl(const QUrl &url);
    Q_INVOKABLE static QStringList pathsFromUrl(const QUrl &url);
    Q_INVOKABLE static QString formatSize(const quint64 &size);
};

#endif // FILEPICKERHELPER_H
