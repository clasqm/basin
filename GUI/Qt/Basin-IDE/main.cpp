/*
   basin ,simple QBASIC interpreter.
   Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
   All rights reserved.
   Contact: Nokia Corporation (qt-info@nokia.com)

   This program is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation,  either  version  3  of the License, or(at your option) any later
   version.

   This  program  is  distributed  in  the  hope  that  it  will  be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "config.h"
#ifndef DISABLE_QTGUI
#include <QApplication>
#include <QTextCodec>

// This define shold be defined in "config.h"
#ifndef PATH_TO_INTERPRETER
#define PATH_TO_INTERPRETER "./basin"
#endif

#include "types.h"
#include "mainwindow.h"

  int main(_int_t argc, _char_t* argv[])
  {
     QApplication app(argc, argv);
     Q_INIT_RESOURCE(dockwidgets);
     MainWindow window;
     window.resize(800, 600);
     window.show();
     return app.exec();
  }
#endif  
