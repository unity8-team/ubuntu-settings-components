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

#include "printer.h"
#include "printer_p.h"

#include <QDebug>

Printer::Printer(QObject *parent)
    : QObject(parent)
    , d_ptr(new PrinterPrivate(this))
{
}

Printer::Printer(PrinterInfo *info, CupsFacade *cups, QObject *parent)
    : QObject(parent)
    , d_ptr(new PrinterPrivate(this, info, cups))
{
}

Printer::~Printer()
{

}

PrinterPrivate::PrinterPrivate(Printer *q)
{

}

PrinterPrivate::PrinterPrivate(Printer *q, PrinterInfo *info, CupsFacade *cups)
{
    this->info = info;
    this->cups = cups;
}

PrinterPrivate::~PrinterPrivate()
{
    delete this->info;
}

ColorMode Printer::colorMode() const
{

}

ColorMode Printer::defaultColorMode() const
{

}

int Printer::copies() const
{

}

bool Printer::duplex() const
{
    Q_D(const Printer);
    return d->info->defaultDuplexMode() != DuplexMode::DuplexNone;
}

QList<DuplexMode> Printer::supportedDuplexModes() const
{
    Q_D(const Printer);
    return d->info->supportedDuplexModes();
}

QStringList Printer::supportedDuplexStrings() const
{
    Q_D(const Printer);
    QStringList list;
    Q_FOREACH(const DuplexMode &mode, supportedDuplexModes()) {
        list << Utils::duplexModeToPpdChoice(mode);
    }
    return list;
}

DuplexMode Printer::defaultDuplexMode() const
{
    Q_D(const Printer);
    return d->info->defaultDuplexMode();
}

QString Printer::name() const
{
    Q_D(const Printer);
    return d->info->printerName();
}

QString Printer::printRange() const
{

}

PrintRange Printer::printRangeMode() const
{

}

bool Printer::pdfMode() const
{

}

Quality Printer::quality() const
{

}

QString Printer::description() const
{
    Q_D(const Printer);
    return d->info->description();
}

QPageSize Printer::pageSize() const
{

}

QList<QPageSize> Printer::supportedPageSizes() const
{

}

AccessControl Printer::accessControl() const
{

}

ErrorPolicy Printer::errorPolicy() const
{

}

bool Printer::enabled() const
{

}

QStringList Printer::users() const
{

}

State Printer::state() const
{

}

QString Printer::lastStateMessage() const
{

}

void Printer::setAccessControl(const AccessControl &accessControl)
{

}

void Printer::setColorMode(const ColorMode &colorMode)
{

}

void Printer::setCopies(const int &copies)
{

}

void Printer::setDescription(const QString &description)
{
    Q_D(const Printer);
    QString answer = d->cups->printerSetInfo(d->info->printerName(), description);

}

void Printer::setDuplex(const bool duplex)
{
    // TODO: this seems useless, maybe drop this setter?
}

void Printer::setDefaultDuplexMode(const DuplexMode &duplexMode)
{
    Q_D(Printer);

    if (defaultDuplexMode() == duplexMode) {
        return;
    }

    if (!d->info->supportedDuplexModes().contains(duplexMode)) {
        qWarning() << __PRETTY_FUNCTION__ << "duplex mode not supported";
        return;
    }

    QStringList vals({Utils::duplexModeToPpdChoice(duplexMode)});
    QString reply = d->cups->printerAddOption(name(), "Duplex", vals);
}

void Printer::setEnabled(const bool enabled)
{

}

void Printer::setErrorPolicy(const ErrorPolicy &errorPolicy)
{

}

void Printer::setName(const QString &name)
{

}

void Printer::setPrintRange(const QString &printRange)
{

}

void Printer::setPrintRangeMode(const PrintRange &printRangeMode)
{

}

void Printer::setPdfMode(const bool pdfMode)
{

}

void Printer::setQuality(const Quality &quality)
{

}

void Printer::setPageSize(const QPageSize &pageSize)
{

}

void Printer::addUser(const QString &username)
{

}

void Printer::removeUser(const QString &username)
{

}

void Printer::requestInkLevels(const QString &name)
{

}
