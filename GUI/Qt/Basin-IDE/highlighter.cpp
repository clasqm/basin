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

#include "highlighter.h"
/* -------------------------------------------------------------------------- */
    Highlighter::Highlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
    {
        HighlightingRule rule;

        keywordFormat.setForeground(Qt::darkBlue);
        keywordFormat.setFontWeight(QFont::Bold);
        QStringList keywordPatterns;
        keywordPatterns << "\\bLET\\b"   << "\\bPRINT\\b"  << "\\bFOR\\b"
                        << "\\bNEXT\\b"  << "\\bDO\\b"     << "\\bLOOP\\b"
                        << "\\bUNTIL\\b" << "\\bWHILE\\b"  << "\\bWEND\\b"
                        << "\\bIF\\b"    << "\\bELSE\\b"   << "\\bELSEIF\\b"
                        << "\\bEND\\b"   << "\\bINPUT\\b"  << "\\bDIM\\b"
                        << "\\bTO\\b"    << "\\bOPTION\\b" << "\\bTHEN\\b"
                        << "\\bSTEP\\b"  << "\\bGOTO\\b"   << "\\bBASE\\b"
                        << "\\bTRACE\\b" << "\\bSCREEN\\b" << "\\bPRESET\\b"
                        << "\\bLINE\\b"  << "\\bCIRCLE\\b" << "\\bCLS\\b"
                        << "\\bBEEP\\b"  << "\\bCLEAR\\b"  << "\\bACTION\\b"
                        << "\\bTREE\\b";
        foreach (QString pattern, keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }

        delimetersFormat.setForeground(Qt::red);
        rule.pattern = QRegExp("[*+-=/<>:)(]");
        rule.format = delimetersFormat;
        highlightingRules.append(rule);

        functionFormat.setForeground(Qt::darkCyan);
        rule.pattern = QRegExp("\\b[A-Za-z0-9_$]+(?=\\()");
        rule.format = functionFormat;
        highlightingRules.append(rule);

        numbersFormat.setForeground(Qt::magenta);
        rule.pattern = QRegExp("[0-9]");
        rule.format = numbersFormat;
        highlightingRules.append(rule);

        quotationFormat.setForeground(Qt::blue);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setFontItalic(true);
        singleLineCommentFormat.setForeground(Qt::gray);
        rule.pattern = QRegExp("\'[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);
    }
/* -------------------------------------------------------------------------- */
    void Highlighter::highlightBlock(const QString &text)
    {
        foreach (HighlightingRule rule, highlightingRules) {
            QRegExp expression(rule.pattern);
            int index = text.indexOf(expression);
            while (index >= 0) {
                int length = expression.matchedLength();
                setFormat(index, length, rule.format);
                index = text.indexOf(expression, index + length);
            }
        }
        setCurrentBlockState(0);
    }
/* -------------------------------------------------------------------------- */
#endif
