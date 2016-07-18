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

#include "filepickerhelper.h"

#include <glib.h>

#include <QDir>
#include <QDebug>

FilePickerHelper::FilePickerHelper(QObject* parent)
    : QObject(parent)
{
}

QUrl FilePickerHelper::homeDirUrl()
{
    return QUrl::fromLocalFile(QDir::homePath());
}

QString FilePickerHelper::pathFromUrl(const QUrl &url)
{
    return url.path();
}

QString FilePickerHelper::rootPath()
{
    return QDir::rootPath();
}

QStringList FilePickerHelper::pathsFromUrl(const QUrl &url)
{
    QString path = url.path();
    return path.split(QDir::separator(), QString::SkipEmptyParts);
}

QString FilePickerHelper::formatSize(const quint64 &size)
{
    guint64 g_size = size;

    gchar * formatted_size = g_format_size (g_size);
    QString q_formatted_size = QString::fromLocal8Bit(formatted_size);
    g_free (formatted_size);

    return q_formatted_size;
}
