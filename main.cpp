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

#ifdef USED_QT5
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

#include "mainwindow.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("lcd-image-converter");
    QCoreApplication::setOrganizationName("riuson");
    QApplication a(argc, argv);
    a.addLibraryPath(QApplication::applicationDirPath());
    a.addLibraryPath(QApplication::applicationDirPath() + "/plugins");
    MainWindow w;
    w.show();
    return a.exec();
}
//-----------------------------------------------------------------------------
