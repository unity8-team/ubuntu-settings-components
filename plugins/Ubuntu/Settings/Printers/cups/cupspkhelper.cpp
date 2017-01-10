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

#include "cups/cupspkhelper.h"

#include <QUrl>
#include <QDebug>

CupsPkHelper::CupsPkHelper()
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

CupsPkHelper::~CupsPkHelper()
{
    if (m_connection)
        httpClose(m_connection);
}

bool CupsPkHelper::printerClassSetInfo(const QString &name,
                                       const QString &info)
{
    if (!isPrinterNameValidInternal(name))
        return false;

    return sendNewPrinterClassRequest(name, IPP_TAG_PRINTER, IPP_TAG_TEXT,
                                      "printer-info", info);
}

bool CupsPkHelper::sendNewPrinterClassRequest(const QString &printerName,
                                              ipp_tag_t group,
                                              ipp_tag_t type,
                                              const QString &name,
                                              const QString &value)
{
    ipp_t *request;

    request = ippNewRequest(CUPS_ADD_MODIFY_PRINTER);
    addPrinterUri(request, printerName);
    addRequestingUsername(request, QString());
    ippAddString(request, group, type, name.toStdString().c_str(), NULL,
                 value.toStdString().c_str());

    if (sendRequest(request, CphResource::CphResourceAdmin))
        return true;

    // it failed, maybe it was a class?
    if (m_lastStatus != IPP_NOT_POSSIBLE) {
        return false;
    }

    // TODO: implement class modification here.
    return false;
}

void CupsPkHelper::addPrinterUri(ipp_t *request,
                                 const QString &name)
{
    QUrl uri(QString("ipp://localhost/printers/%1").arg(name));
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
                 "printer-uri", NULL, uri.toEncoded().data());
}

void CupsPkHelper::addRequestingUsername(ipp_t *request,
                                         const QString &username)
{
    if (!username.isEmpty())
        ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                     "requesting-user-name", NULL,
                     username.toStdString().c_str());
    else
        ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                     "requesting-user-name", NULL, cupsUser());
}

const QString CupsPkHelper::getResource(
        const CupsPkHelper::CphResource &resource)
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

bool CupsPkHelper::isPrinterNameValidInternal(const QString &name)
{
    int i;
    int len;

    /* Quoting the lpadmin man page:
     *    CUPS allows printer names to contain any printable character
     *    except SPACE, TAB, "/", or  "#".
     * On top of that, validate_name() in lpadmin.c (from cups) checks that
     * the string is 127 characters long, or shorter. */

    /* no empty string */
    if (name.isEmpty())
        return false;

    len = name.size();
    /* no string that is too long; see comment at the beginning of the
     * validation code block */
    if (len > 127)
        return false;

    /* only printable characters, no space, no /, no # */
    for (i = 0; i < len; i++) {
        const QChar c = name.at(i);
        if (!c.isPrint())
                return false;
        if (c.isSpace())
                return false;
        if (c == '/' || c == '#')
                return false;
    }
    return true;
}


bool CupsPkHelper::sendRequest(ipp_t *request, const CphResource &resource)
{
    ipp_t *reply;
    const QString resourceChar = getResource(resource);
    reply = cupsDoRequest(m_connection, request,
                          resourceChar.toStdString().c_str());
    return handleReply(reply);
}

bool CupsPkHelper::handleReply(ipp_t *reply)
{
    bool retval;
    retval = isReplyOk(reply, false);
    if (reply)
        ippDelete(reply);

    return retval;
}

bool CupsPkHelper::isReplyOk(ipp_t *reply, bool deleteIfReplyNotOk)
{
    /* reset the internal status: we'll use the cups status */
    m_lastStatus = IPP_STATUS_CUPS_INVALID;

    if (reply && ippGetStatusCode(reply) <= IPP_OK_CONFLICT) {
        m_lastStatus = IPP_OK;
        return true;
    } else {
        setErrorFromReply(reply);
        qWarning() << __PRETTY_FUNCTION__ << cupsLastErrorString();

        if (deleteIfReplyNotOk && reply)
            ippDelete(reply);

        return false;
    }
}

void CupsPkHelper::setErrorFromReply(ipp_t *reply)
{
    if (reply)
        m_lastStatus = ippGetStatusCode(reply);
    else
        m_lastStatus = cupsLastError();
}
