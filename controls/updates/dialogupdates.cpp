/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "dialogupdates.h"
#include "ui_dialogupdates.h"

#include <QTextStream>
#include <QFile>
#include <QXmlQuery>
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include "revisioninfo.h"
//-----------------------------------------------------------------------------
DialogUpdates::DialogUpdates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUpdates)
{
    ui->setupUi(this);
    this->ui->labelStatus->setVisible(false);

    // hide ? button from title
    this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));

    // update icon
    QIcon icon;
    icon.addFile(":/images/icon64", QSize(64, 64));
    this->setWindowIcon(icon);

    // show local history by default
    this->showHistory();

    // focus on Close button
    this->ui->buttonBox->setFocus();

    // start download history.xml from network
    this->showUpdates();
}
//-----------------------------------------------------------------------------
DialogUpdates::~DialogUpdates()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogUpdates::showHistory()
{
    // XML file
    QString xml;
    {
        QFile file(":/history/changes");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            xml = stream.readAll();
            file.close();
        }
    }

    // XSL file
    QString xsl;
    {
        QFile file(":/history/xsl");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            xsl = stream.readAll();
            file.close();
        }
    }

    // CSS file
    QString style;
    {
        QFile file(":/history/style");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            style = stream.readAll();
            file.close();
        }
    }

    // transform
    QString html;
    if (this->transformHistory(xml, xsl, &html))
    {
        this->ui->textBrowser->document()->setDefaultStyleSheet(style);
        this->ui->textBrowser->setHtml(html);
    }
}
//-----------------------------------------------------------------------------
void DialogUpdates::showUpdates()
{
    QNetworkAccessManager* mNetworkManager = new QNetworkAccessManager(this);
    this->connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), SLOT(networkReply(QNetworkReply*)));

    QUrl url("http://wiki.lcd-image-converter.googlecode.com/git/history.xml");
    QNetworkReply* reply = mNetworkManager->get(QNetworkRequest(url));
    Q_UNUSED(reply);
}
//-----------------------------------------------------------------------------
void DialogUpdates::showUpdates(const QString &xml)
{
    // XSL file
    QString xsl;
    {
        QFile file(":/history/xsl");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            xsl = stream.readAll();
            file.close();
        }
    }

    // CSS file
    QString style;
    {
        QFile file(":/history/style");
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            style = stream.readAll();
            file.close();
        }
    }

    // transform
    QString html;
    if (this->transformHistory(xml, xsl, &html))
    {
        this->ui->textBrowser->document()->setDefaultStyleSheet(style);
        this->ui->textBrowser->setHtml(html);
    }
}
//-----------------------------------------------------------------------------
void DialogUpdates::showError(const QString &message)
{
    this->ui->labelStatus->setText(message);
    this->ui->labelStatus->setVisible(true);
}
//-----------------------------------------------------------------------------
bool DialogUpdates::transformHistory(const QString &xml, const QString &xsl, QString *html)
{
    bool isSuccessfully = false;

    // XML file
    QBuffer history;
    {
        QString value = xml;

        value.replace("<sha1>current</sha1>", QString("<sha1>%1</sha1>").arg(RevisionInfo::hash()));
        value.replace("<date>current</date>", QString("<date>%1</date>").arg(RevisionInfo::date()));

        QByteArray array = value.toUtf8();

        history.setData(array);
    }

    // XSL file
    QBuffer transform;
    {
        QString value = xsl;

        value.replace("$current_date", QString("%1").arg(RevisionInfo::date()));

        QByteArray array = value.toUtf8();

        transform.setData(array);
    }

    // transform
    if (history.open(QIODevice::ReadOnly))
    {
        if (transform.open(QIODevice::ReadOnly))
        {
            QXmlQuery query(QXmlQuery::XSLT20);

            query.setFocus(&history);
            query.setQuery(&transform);

            QString resultHtml = "";
            if (query.evaluateTo(&resultHtml))
            {
                *html = resultHtml;
                isSuccessfully = true;
            }

            transform.close();
        }
        history.close();
    }

    return isSuccessfully;
}
//-----------------------------------------------------------------------------
void DialogUpdates::networkReply(QNetworkReply* reply)
{
    const int RESPONSE_OK = 200;

    QString replyString;
    if(reply->error() == QNetworkReply::NoError)
    {
        int httpStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt();
        switch(httpStatusCode)
        {
        case RESPONSE_OK:
        {
            if (reply->isReadable())
            {
                //Assuming this is a human readable file replyString now contains the file
                replyString = QString::fromUtf8(reply->readAll().data());
                this->showUpdates(replyString);
            }
            break;
        }
        default:
        {
            QString message = QString("Check updates failed with HTTP code: %1").arg(httpStatusCode);
            this->showError(message);
            break;
        }
        }
    }
    else
    {
        QString message = QString("Check updates failed with QNetworkReply message:<br/>%1").arg(reply->errorString());
        this->showError(message);
    }
    reply->deleteLater();
}
