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

#include <QtGui>
#include <cassert>

#include "editor.h"
/* -------------------------------------------------------------------------- */
  CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
  {
     lineNumberArea = new LineNumberArea(this);

     connect(this, SIGNAL(blockCountChanged(int)), this,
                                          SLOT(updateLineNumberAreaWidth(int)));
     connect(this, SIGNAL(updateRequest(const QRect &, int)), this,
                                SLOT(updateLineNumberArea(const QRect &, int)));

     connect(this, SIGNAL(cursorPositionChanged()), this,
                                                  SLOT(highlightCurrentLine()));

     updateLineNumberAreaWidth(0);

     QFont font;
     font.setFixedPitch(true);
     font.setPointSize(10);
     font.setStyleHint(QFont::System,QFont::PreferAntialias);

     setFont(font);
     setLineWidth (80);

     highlighter = new Highlighter(document());
  }
/* -------------------------------------------------------------------------- */
  _int_t CodeEditor::lineNumberAreaWidth()
  {
     _int_t digits = 1;
     _int_t max = qMax(1, blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     _int_t space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

     return space;
  }
/* -------------------------------------------------------------------------- */
  void CodeEditor::updateLineNumberAreaWidth(_int_t /* newBlockCount */)
  {
     setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
  }
/* -------------------------------------------------------------------------- */
  void CodeEditor::updateLineNumberArea(const QRect &rect, _int_t dy)
  {
     if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(),
                                        lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
  }
/* -------------------------------------------------------------------------- */
  void CodeEditor::resizeEvent(QResizeEvent *e)
  {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                           lineNumberAreaWidth(), cr.height()));
  }
/* -------------------------------------------------------------------------- */
  void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
  {
     QPainter painter(lineNumberArea);
     painter.fillRect(event->rect(), Qt::lightGray);

     QTextBlock block = firstVisibleBlock();
     _int_t blockNumber = block.blockNumber();
     _int_t top =
        (_int_t) blockBoundingGeometry(block).translated(contentOffset()).top();

     _int_t bottom = top + (_int_t) blockBoundingRect(block).height();

     while (block.isValid() && top <= event->rect().bottom())
     {
        if (block.isVisible() && bottom >= event->rect().top())
        {
           QString number = QString::number(blockNumber + 1);
           painter.setPen(Qt::black);
           painter.drawText(0, top, lineNumberArea->width(),
                            fontMetrics().height(),Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (_int_t) blockBoundingRect(block).height();
        ++blockNumber;
     }
  }
/* -------------------------------------------------------------------------- */
  void  CodeEditor::highlightCurrentLine()
  {
     QList<QTextEdit::ExtraSelection> extraSelections;
     QTextEdit::ExtraSelection selection;
     QColor lineColor = QColor(Qt::yellow).lighter(170);

     selection.format.setBackground(lineColor);
     selection.format.setProperty(QTextFormat::FullWidthSelection, true);
     selection.cursor = textCursor();
     selection.cursor.clearSelection();
     extraSelections.append(selection);

     setExtraSelections(extraSelections);
  }
/* -------------------------------------------------------------------------- */
  void  CodeEditor::highlightCurrentErrorLine(const QString &string)
  {
     QList<QTextEdit::ExtraSelection> extraSelections;
     QTextEdit::ExtraSelection selection;
     QColor lineColor = QColor(Qt::red).lighter(170);

     selection.format.setBackground(lineColor);
     selection.format.setProperty(QTextFormat::FullWidthSelection, true);
     selection.cursor = textCursor();
     selection.cursor.clearSelection();

     // This dirty code is need for select error line with red color
     // Need to parse string from stderr,sent by interpreter,and find
     // line number to highlight.
     QString str(string);
     std::string temp = str.toStdString();
     int i = temp.find(" line: ") + 7;
     int j = temp.find(" error: ");
     int line = atoi(temp.substr(i,j - i).c_str());
     assert (line >=0);
     selection.cursor.movePosition (QTextCursor::Start);
     while (line > 1)
     {
        selection.cursor.movePosition (QTextCursor::Down);
        --line;
     }
     extraSelections.append(selection);

     setTextCursor(selection.cursor);
     setExtraSelections(extraSelections);
     ensureCursorVisible ();
  }
/* -------------------------------------------------------------------------- */
#endif
