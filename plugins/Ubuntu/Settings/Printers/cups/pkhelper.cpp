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

#include "cups/pkhelper.h"

#include <QUrl>
#include <QDebug>

PkHelper::PkHelper()
    : m_connection(httpConnectEncrypt(cupsServer(),
                                      ippPort(),
                                      cupsEncryption()))
{
    if (!m_connection) {
        qCritical("Failed to connect to cupsd");
    } else {
        qDebug("Successfully connected to cupsd.");
    }
}

PkHelper::~PkHelper()
{
    if (m_connection)
        httpClose(m_connection);
}

bool PkHelper::sendNewPrinterClassRequest(const QString &printerName,
                                          ipp_tag_t group,
                                          ipp_tag_t type,
                                          const QString &name,
                                          const QString &value)
{
    qWarning() << "sendNewPrinterClassRequest";
    ipp_t *request;

    request = ippNewRequest(CUPS_ADD_MODIFY_PRINTER);
    _cph_cups_add_printer_uri(request, printerName);
    _cph_cups_add_requesting_user_name(request, QString());
    ippAddString(request, group, type, name.toStdString().c_str(), NULL, value.toStdString().c_str());

    if (sendRequest(request, CphResource::CphResourceAdmin))
        return true;

    // it failed, maybe it was a class?
    if (m_lastStatus != IPP_NOT_POSSIBLE) {
        qWarning() << "nah";
        return false;
    }

    // request = ippNewRequest (CUPS_ADD_MODIFY_CLASS);
    // _cph_cups_add_class_uri (request, printer_name);
    // _cph_cups_add_requesting_user_name (request, NULL);
    // ippAddString (request, group, type, name, NULL, value);

    // return _cph_cups_send_request (cups, request, CPH_RESOURCE_ADMIN);

}

void PkHelper::_cph_cups_add_printer_uri(ipp_t *request,
                                         const QString &name)
{
    QUrl uri(QString("ipp://localhost/printers/%1").arg(name));

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
                 "printer-uri", NULL, uri.toEncoded().data());
}

void PkHelper::_cph_cups_add_requesting_user_name(ipp_t *request,
                                                  const QString &username)
{
    if (!username.isEmpty())
        ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                     "requesting-user-name", NULL, username.toLatin1());
    else
        ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                     "requesting-user-name", NULL, cupsUser());
}

const QString PkHelper::_cph_cups_get_resource(const PkHelper::CphResource &resource)
{
    switch (resource) {
    case CphResourceRoot:
            return "/";
    case CphResourceAdmin:
            return "/admin/";
    case CphResourceJobs:
            return "/jobs/";
    default:
        /* that's a fallback -- we don't use
         * g_assert_not_reached() to avoir crashing. */
        qCritical("Asking for a resource with no match.");
        return "/";
    }
}

bool PkHelper::sendRequest(ipp_t *request, const CphResource &resource)
{
    ipp_t      *reply;
    const QString resourceChar = _cph_cups_get_resource(resource);

    reply = cupsDoRequest(m_connection, request, resourceChar.toLatin1());

    return handleReply(reply);
}

bool PkHelper::handleReply(ipp_t *reply)
{
    bool retval;

    retval = isReplyOk(reply, false);

    if (reply)
        ippDelete(reply);

    return retval;
}

bool PkHelper::isReplyOk(ipp_t *reply, bool deleteIfReplyNotOk)
{
        /* reset the internal status: we'll use the cups status */
        m_lastStatus = IPP_STATUS_CUPS_INVALID ;

        if (reply && ippGetStatusCode (reply) <= IPP_OK_CONFLICT) {
            m_lastStatus = IPP_OK;
            return true;
        } else {
            setErrorFromReply(reply);
            qWarning() << "reply not ok" << cupsLastErrorString();

            if (deleteIfReplyNotOk && reply)
                ippDelete (reply);

            return false;
        }

}

void PkHelper::setErrorFromReply(ipp_t *reply)
{
    if (reply)
        m_lastStatus = ippGetStatusCode(reply);
    else
        m_lastStatus = cupsLastError();
}
