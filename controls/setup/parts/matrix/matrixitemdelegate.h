/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#ifndef MATRIXITEMDELEGATE_H
#define MATRIXITEMDELEGATE_H
//-----------------------------------------------------------------------------
#include <QtGui>

#ifdef USED_QT5
#include <QtWidgets>
#endif
//-----------------------------------------------------------------------------
class MatrixItemDelegate : public QItemDelegate
{
public:
    MatrixItemDelegate(QObject *pobj);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    QColor color() const;
    void setColor(const QColor &value);
private:
    QColor mColor;
};
//-----------------------------------------------------------------------------
#endif // MATRIXITEMDELEGATE_H
