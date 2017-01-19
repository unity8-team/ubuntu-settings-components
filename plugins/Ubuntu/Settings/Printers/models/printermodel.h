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

#ifndef USC_PRINTER_MODEL_H
#define USC_PRINTER_MODEL_H

#include "printers_global.h"

#include "printer/printer.h"

#include <QAbstractListModel>
#include <QByteArray>
#include <QModelIndex>
#include <QObject>
#include <QSortFilterProxyModel>
#include <QTimer>
#include <QVariant>

class PrinterModelPrivate;
class PRINTERS_DECL_EXPORT PrinterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PrinterModel)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit PrinterModel(int timerMsec=5000, QObject *parent = 0);
    explicit PrinterModel(PrinterInfo *info, CupsFacade *cups, int timerMsec=5000, QObject *parent = 0);
    ~PrinterModel();

    enum Roles
    {
        // Qt::DisplayRole holds device name
        ColorModelRole = Qt::UserRole,
        SupportedColorModelsRole,
        CopiesRole,
        DefaultPrinterRole,
        DuplexRole,
        SupportedDuplexModesRole,
        NameRole,
        PrintRangeRole,
        PrintRangeModeRole,
        PdfModeRole,
        PrintQualityRole,
        SupportedPrintQualitiesRole,
        DescriptionRole,
        PageSizeRole,
        SupportedPageSizesRole,
        AccessControlRole,
        ErrorPolicyRole,
        UsersRole,
        StateRole,
        PrinterRole,
        LastStateMessageRole,
        IsPdfRole,
        LastRole = LastStateMessageRole,
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int count() const;

    QSharedPointer<Printer> getPrinterFromName(const QString &name);

    Q_INVOKABLE QVariantMap get(const int row) const;
private:
    QScopedPointer<PrinterModelPrivate> const d_ptr;
    QTimer m_update_timer;

private Q_SLOTS:
    void startUpdateTimer(int msecs);
    void update();

Q_SIGNALS:
    void countChanged();
};

class PRINTERS_DECL_EXPORT PrinterFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit PrinterFilter(QObject *parent = Q_NULLPTR);
    ~PrinterFilter();

    Q_INVOKABLE QVariantMap get(const int row) const;

    void filterOnState(const PrinterEnum::State &state);
    void filterOnRecent(const bool recent);
    void filterOnPdf(const bool pdf);

    int count() const;
protected:
    virtual bool filterAcceptsRow(
        int sourceRow, const QModelIndex &sourceParent) const override;
    virtual bool lessThan(const QModelIndex &left,
                          const QModelIndex &right) const override;

Q_SIGNALS:
    void countChanged();

private Q_SLOTS:
    void onSourceModelChanged();
    void onSourceModelCountChanged();

private:
    PrinterEnum::State m_state = PrinterEnum::State::IdleState;
    bool m_stateEnabled = false;
    bool m_recent = false;
    bool m_recentEnabled = false;
    bool m_pdfEnabled = false;
    bool m_pdf = false;
};

#endif // USC_PRINTER_MODEL_H
