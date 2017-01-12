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

#include <cups/ppd.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

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
    if (!isPrinterNameValid(name)) {
        setInternalStatus(QString("%1 is not a valid printer name.").arg(info));
        return false;
    }

    if (!isStringValid(info)) {
        setInternalStatus(QString("%1 is not a valid description.").arg(info));
        return false;
    }

    return sendNewPrinterClassRequest(name, IPP_TAG_PRINTER, IPP_TAG_TEXT,
                                      "printer-info", info);
}

bool CupsPkHelper::printerClassSetOption(const QString &name,
                                         const QString &option,
                                         const QStringList &values)
{
        bool isClass;
        int length;
        ipp_t *request;
        ipp_attribute_t *attr;
        char *newppdfile;
        bool retval;

        if (!isPrinterNameValid(name)) {
            setInternalStatus(QString("%1 is not a valid printer name.").arg(name));
            return false;
        }

        if (!isStringValid(option)) {
            setInternalStatus(QString("%1 is not a valid option.").arg(option));
            return false;
        }

        if (values.size() == 0) {
            setInternalStatus("Invalid values argument.");
            return false;
        }

        Q_FOREACH(const QString &val, values) {
            if (!isStringValid(val)) {
                setInternalStatus(QString("%1 is not a valid value.").arg(val));
                return false;
            }
        }


        isClass = printerIsClass(name);

        /* We permit only one value to change in PPD file because we are setting
         * default value in it. */
        if (!isClass && length == 1) {
            cups_option_t *options = NULL;
            int numOptions = 0;
            QString ppdfile;

            numOptions = cupsAddOption(option.toStdString().c_str(),
                                       values[0].toStdString().c_str(),
                                       numOptions, &options);

            ppdfile = QString(cupsGetPPD(name.toStdString().c_str()));

            newppdfile = preparePpdForOptions(ppdfile.toStdString().c_str(),
                                              options, numOptions).toLatin1().data();

            unlink(ppdfile.toStdString().c_str());
            cupsFreeOptions(numOptions, options);
        } else
            newppdfile = NULL;

        if (isClass) {
            request = ippNewRequest(CUPS_ADD_MODIFY_CLASS);
            addClassUri(request, name);
        } else {
            request = ippNewRequest(CUPS_ADD_MODIFY_PRINTER);
            addPrinterUri(request, name);
        }

        addRequestingUsername(request, NULL);

        if (length == 1) {
            ippAddString(request, IPP_TAG_PRINTER, IPP_TAG_NAME,
                         option.toStdString().c_str(),
                         NULL,
                         values[0].toStdString().c_str());
        } else {
            int i;

            attr = ippAddStrings(request, IPP_TAG_PRINTER, IPP_TAG_NAME,
                                 option.toStdString().c_str(), length, NULL, NULL);

            for (i = 0; i < length; i++)
                ippSetString(request, &attr, i, values[i].toStdString().c_str());
        }

        if (newppdfile) {
            retval = postRequest(request, newppdfile, CphResourceAdmin);
            unlink(newppdfile);
            // TODO: fix leak here.
        } else {
                retval = sendRequest(request, CphResourceAdmin);
        }

        return retval;
}


/* This function sets given options to specified values in file 'ppdfile'.
 * This needs to be done because of applications which use content of PPD files
 * instead of IPP attributes.
 * CUPS doesn't do this automatically (but hopefully will starting with 1.6) */
QString CupsPkHelper::preparePpdForOptions(const QString &ppdfile,
                                           cups_option_t *options,
                                           int numOptions)
{
    auto ppdfile_c = ppdfile.toStdString().c_str();
    ppd_file_t *ppd;
    bool ppdchanged = false;
    QString result;
    QString error;
    char newppdfile[PATH_MAX];
    cups_file_t  *in = NULL;
    cups_file_t  *out = NULL;
    QString line(CPH_STR_MAXLEN, QChar());
    QString keyword(CPH_STR_MAXLEN, QChar());
    QString keyptr;
    ppd_choice_t *choice;
    const QString value;
    QLatin1String defaultStr("*Default");

    ppd = ppdOpenFile(ppdfile_c);
    if (!ppd) {
        error = QString("Unable to open PPD file \"%1\": %2")
                .arg(ppdfile).arg(strerror(errno));
        setInternalStatus(error);
        goto out;
    }

    in = cupsFileOpen(ppdfile_c, "r");
    if (!in) {
        error = QString("Unable to open PPD file \"%1\": %2")
            .arg(ppdfile).arg(strerror(errno));
        setInternalStatus(error);
        goto out;
    }

    out = cupsTempFile2(newppdfile, sizeof(newppdfile));
    if (!out) {
        setInternalStatus("Unable to create temporary file");
        goto out;
    }

    /* Mark default values and values of options we are changing. */
    ppdMarkDefaults(ppd);
    cupsMarkOptions(ppd, numOptions, options);

    while (cupsFileGets(in, line.data()->toLatin1(), line.size())) {
        if (!line.startsWith(defaultStr)) {
            cupsFilePrintf(out, "%s\n", line.data());
        } else {
            /* This part parses lines with *Default on their
             * beginning. For instance:
             *   "*DefaultResolution: 1200dpi" becomes:
             *     - keyword: Resolution
             *     - keyptr: 1200dpi
             */
            keyword = line.mid(defaultStr.size());
            for (keyptr = keyword.data(); *keyptr; keyptr++)
                if (*keyptr == ':' || isspace(*keyptr & 255))
                    break;

            *keyptr++ = '\0';

            while (isspace(*keyptr & 255))
                keyptr++;

            /* We have to change PageSize if any of PageRegion,
             * PageSize, PaperDimension or ImageableArea changes.
             * We change PageRegion if PageSize is not available. */
            if (keyword == "PageRegion" ||
                keyword == "PageSize" ||
                keyword == "PaperDimension" ||
                keyword == "ImageableArea") {

                choice = ppdFindMarkedChoice(ppd, "PageSize");
                if (!choice)
                    choice = ppdFindMarkedChoice(ppd, "PageRegion");
            } else {
                choice = ppdFindMarkedChoice(ppd, keyword);
            }

            if (choice && !g_str_equal (choice->choice, keyptr)) {
                /* We have to set the value in PPD manually if
                 * a custom value was passed in:
                 * cupsMarkOptions() marks the choice as
                 * "Custom". We want to set this value with our
                 * input. */
                if (!(QString(choice->choice) == "Custom")) {
                    cupsFilePrintf(out,
                                   "*Default%s: %s\n",
                                   keyword.toStdString().c_str(),
                                   choice->choice);
                    ppdchanged = true;
                } else {
                    value = cupsGetOption(keyword, numOptions, options);
                    if (value) {
                        cupsFilePrintf(out,
                                       "*Default%s: %s\n",
                                       keyword.toStdString().c_str(),
                                       value.toStdString().c_str());
                        ppdchanged = true;
                    } else {
                        cupsFilePrintf(out, "%s\n", line.toStdString().c_str());
                    }
                }
            } else {
                cupsFilePrintf (out, "%s\n", line.toStdString().c_str());
            }
        }
    }

    if (ppdchanged)
        result = QString(newppdfile);
    else
        unlink(newppdfile);

out:
    if (in)
        cupsFileClose(in);
    if (out)
        cupsFileClose(out);
    if (ppd)
        ppdClose(ppd);

    return result;
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

    // TODO: implement class modification <here>.
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
        /* that's a fall back -- we don't use
         * g_assert_not_reached() to avoid crashing. */
        qCritical("Asking for a resource with no match.");
        return "/";
    }
}

bool CupsPkHelper::isPrinterNameValid(const QString &name)
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

bool CupsPkHelper::isStringValid(const QString &string, const bool checkNull,
                                 const int maxLength)
{
    if (isStringPrintable(string, checkNull, maxLength))
        return true;
    return false;
}

bool CupsPkHelper::isStringPrintable(const QString &string,
                                     const bool checkNull,
                                     const int maxLength)
{
    int i;
    int len;

    /* no null string */
    if (string.isNull())
        return !checkNull;

    len = string.size();
    if (maxLength > 0 && len > maxLength)
        return false;

    /* only printable characters */
    for (i = 0; i < len; i++) {
        const QChar c = string.at(i);
        if (!c.isPrint())
                return false;
    }
    return true;
}

void CupsPkHelper::setInternalStatus(const QString &status)
{
    if (!m_internalStatus.isNull()) {
        m_internalStatus = QString::null;
    }

    if (status.isNull()) {
        m_internalStatus = QString::null;
    } else {
        m_internalStatus = status;

        // Only used for errors for now.
        qCritical() << status;
    }
}

bool CupsPkHelper::postRequest(ipp_t *request, const QString &file,
                               const CphResource &resource)
{
    ipp_t *reply;
    QString resourceChar;

    resourceChar = getResource(resource);

    if (!file.isEmpty())
        reply = cupsDoFileRequest(m_connection, request, resourceChar,
                                  file.toStdString().c_str());
    else
        reply = cupsDoFileRequest(m_connection, request, resourceChar,
                                  NULL);

    return handleReply(reply);
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

bool CupsPkHelper::printerIsClass(const QString &name);
{
    const char * const attrs[1] = { "member-names" };
    ipp_t *request;
    QString resource;
    ipp_t *reply;
    bool retval;

    // Class/Printer name validation is equal.
    if (!isPrinterNameValid(name)) {
        setInternalStatus(QString("%1 is not a valid printer name.").arg(name));
        return false;
    }

    request = ippNewRequest(IPP_GET_PRINTER_ATTRIBUTES);
    addClassUri(request, name);
    addRequestingUsername(request, QString());
    ippAddStrings(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD,
                  "requested-attributes", 1, NULL, attrs);

    resource = getResource(CphResource::CphResourceRoot);
    reply = cupsDoRequest(cups->priv->connection,
                          request.toStdString().c_str(),
                          resource.toStdString().c_str());

    if (!isReplyOk(reply, true))
        return true;

    /* Note: we need to look if the attribute is there, since we get a
     * reply if the name is a printer name and not a class name. The
     * attribute is the only way to distinguish the two cases. */
    retval = ippFindAttribute(reply, attrs[0], IPP_TAG_NAME) != NULL;

    if (reply)
        ippDelete(reply);

    return retval;
}

void CupsPkHelper::addClassUri(ipp_t *request, const QString &name)
{
    QUrl uri(QString("ipp://localhost/printers/%1").arg(name));
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
                 "printer-uri", NULL, uri.toEncoded().data());
}

