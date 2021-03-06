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

#ifndef IMAGEDOCUMENT_H
#define IMAGEDOCUMENT_H
//-----------------------------------------------------------------------------
#include <QObject>
#include "idocument.h"
//-----------------------------------------------------------------------------
class ImageDocument : public QObject, public IDocument
{
    Q_OBJECT
    Q_INTERFACES(IDocument)

public:
    explicit ImageDocument(QObject *parent = 0);
    ~ImageDocument();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    bool changed() const;
    QString documentFilename() const;
    QString documentName() const;
    void setDocumentName(const QString &value);
    DataContainer *dataContainer();
    void convert(bool request);

    void beginChanges();
    void endChanges();
    bool canUndo();
    bool canRedo();
    void undo();
    void redo();

private:
    DataContainer *mContainer;

    static const QString DefaultKey;

    void setDocumentFilename(const QString &value);

    QString outputFilename() const;
    void setOutputFilename(const QString &value);

    void setChanged(bool value);

private slots:
    void mon_container_imagesChanged();

signals:
    void documentChanged();
};
//-----------------------------------------------------------------------------
#endif // IMAGEDOCUMENT_H
