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

#include <cups/cups.h>
#include <cups/http.h>
#include <cups/ipp.h>
// #include <cups/ppd.h>

#include <QString>

// Class that wraps code from cups-pk-helper as well as cups state.
class CupsPkHelper
{
public:
    explicit CupsPkHelper();
    ~CupsPkHelper();

    bool printerClassSetInfo(const QString &name, const QString &info);

private:
    enum CphResource
    {
        CphResourceRoot = 0,
        CphResourceAdmin,
        CphResourceJobs,
    };

    bool sendNewPrinterClassRequest(const QString &printerName,
                                    ipp_tag_t group,
                                    ipp_tag_t type,
                                    const QString &name,
                                    const QString &value);
    static void addPrinterUri(ipp_t *request, const QString &name);
    static void addRequestingUsername(ipp_t *request, const QString &username);
    static const QString getResource(const CphResource &resource);
    static bool isPrinterNameValidInternal(const QString &name);

    bool sendRequest(ipp_t *request, const CphResource &resource);
    bool handleReply(ipp_t *reply);
    bool isReplyOk(ipp_t *reply, bool deleteIfReplyNotOk);
    void setErrorFromReply(ipp_t *reply);

    http_t *m_connection;
    ipp_status_t m_lastStatus = IPP_OK;
    QString m_internalStatus = QString::null;
};
