/*
   Basin-IDE,frontend to Basin - QBASIC Interpreter.
   Copyright (C) 2009-2011  #pragma, Evg

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

#ifndef TERMINAL_H_INCLUDED
#define TERMINAL_H_INCLUDED

#include "config.h"
#ifndef DISABLE_QTGUI
#include <QtGui>
#include <QPlainTextEdit>
/* -------------------------------------------------------------------------- */
  class QTerminal :public QPlainTextEdit
  {
     Q_OBJECT

     public:
        QTerminal ();
        bool isFinished;
     signals:
        void is_active ( QKeyEvent * event );
     private:
        void keyPressEvent ( QKeyEvent * event );
  };
/* -------------------------------------------------------------------------- */
#endif // DISABLE_QTGUI
/* -------------------------------------------------------------------------- */
#endif // TERMINAL_H_INCLUDED
