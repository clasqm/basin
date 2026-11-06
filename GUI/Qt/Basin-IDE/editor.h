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

#ifndef EDITOR_H_INCLUDED
#define EDITOR_H_INCLUDED

#include "config.h"
#ifndef DISABLE_QTGUI

#include <QPlainTextEdit>
#include <QObject>

#include "types.h"
#include "highlighter.h"
/* -------------------------------------------------------------------------- */
  class QPaintEvent;
  class QResizeEvent;
  class QSize;
  class QWidget;

  class LineNumberArea;
/* -------------------------------------------------------------------------- */
  class CodeEditor : public QPlainTextEdit
  {
     Q_OBJECT

     public:
        CodeEditor(QWidget *parent = 0);

        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();

     protected:
        void resizeEvent(QResizeEvent *event);

     private slots:
        void updateLineNumberAreaWidth(int newBlockCount);
        void updateLineNumberArea(const QRect &, int);

     public slots:
        void highlightCurrentLine();
        void highlightCurrentErrorLine(const QString &string = QString());

     private:
        QWidget *lineNumberArea;
        Highlighter *highlighter;
  };
/* -------------------------------------------------------------------------- */
  class LineNumberArea : public QWidget
  {
     public:
        LineNumberArea(CodeEditor  *editor) : QWidget(editor)
        {
           codeEditor = editor;
        }

        QSize sizeHint() const
        {
           return QSize(codeEditor->lineNumberAreaWidth(), 0);
        }

     protected:
        void paintEvent(QPaintEvent *event)
        {
           codeEditor->lineNumberAreaPaintEvent(event);
        }

     private:
        CodeEditor  *codeEditor;
 };
/* -------------------------------------------------------------------------- */
#endif
#endif // EDITOR_H_INCLUDED
