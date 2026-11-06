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

#include "config.h"
#ifndef DISABLE_QTGUI
#include "terminal.h"
/* -------------------------------------------------------------------------- */
  QTerminal::QTerminal () :QPlainTextEdit()
  {
     setGeometry( 20, 20, 485, 316 );
     setWordWrapMode( QTextOption::WrapAnywhere );
     setPalette ( QPalette( QBrush(QColor(0x00,0x00,0x00)), // windowText
                            QBrush(QColor(0xff,0xff,0xff)), // button
                            QBrush(QColor(0xff,0xff,0xff)), // light
                            QBrush(QColor(0xff,0xff,0xff)), // dark
                            QBrush(QColor(0xff,0xff,0xff)), // mid
                            QBrush(QColor(0xe5,0xe5,0xe5)), // text
                            QBrush(QColor(0xff,0xff,0xff)), // bright_text
                            QBrush(QColor(0x00,0x00,0x00)), // base
                            QBrush(QColor(0x00,0x00,0x00))  // window
                          ));
     setVerticalScrollBarPolicy   ( Qt::ScrollBarAlwaysOff );
     setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
     setReadOnly ( true );
     setFocusPolicy ( Qt::StrongFocus );

     QFont font;
     font.setFixedPitch(true);
     font.setPointSize(10);
     font.setStyleHint(QFont::System,QFont::NoAntialias);
     setFont ( font );

     isFinished = false;
  }
/* -------------------------------------------------------------------------- */
  void QTerminal::keyPressEvent ( QKeyEvent * )
  {  // Closing terminal if any key pressed
     if (isFinished ) close();
  }
/* -------------------------------------------------------------------------- */
#endif
