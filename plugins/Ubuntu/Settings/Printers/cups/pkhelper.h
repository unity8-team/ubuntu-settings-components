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

class PkHelper
{
public:
    explicit PkHelper();
    ~PkHelper();

    bool sendNewPrinterClassRequest(const QString &printerName,
                                    ipp_tag_t group,
                                    ipp_tag_t type,
                                    const QString &name,
                                    const QString &value);

private:
    enum CphResource
    {
        CphResourceRoot = 0,
        CphResourceAdmin,
        CphResourceJobs,
    };

    static void _cph_cups_add_printer_uri(ipp_t *request,
                                          const QString &name);
    static void _cph_cups_add_requesting_user_name(ipp_t *request,
                                                   const QString &username);
    static const QString _cph_cups_get_resource (const CphResource &resource);

    bool sendRequest(ipp_t *request, const CphResource &resource);
    bool handleReply(ipp_t *reply);
    bool isReplyOk(ipp_t *reply,
                   bool deleteIfReplyNotOk);
    void setErrorFromReply(ipp_t *reply);

    http_t *m_connection;
    ipp_status_t m_lastStatus = IPP_OK;
    QString m_internalStatus = QString::null;
};
