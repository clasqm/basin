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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
  using std::vector;
#include <QMainWindow>
#include <QStringList>
#include <QAction>
#include <QProcess>

#include "types.h"
#include "editor.h"
#include "terminal.h"
/* -------------------------------------------------------------------------- */
  typedef char _char_t;
/* -------------------------------------------------------------------------- */
  class QTextEdit;
  class QListWidget;
  class QProcess;
  class QWidget;
/* -------------------------------------------------------------------------- */
  class MainWindow : public QMainWindow
  {
     Q_OBJECT
//{  public
     public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow ();
        void StoreArgs (_char_t* argv[]);
//}
//{  public slots
     public slots:
        void about();
        void newFile();
        void interp();
        void run();
        void save();
        void print();
        void undo();
        void toggleInitVars();
        void toggleTraceActions();
        void toggleTraceTree();
        void toggleTraceSilent();
        void configure();
        void readErr();
        void readOut();
        void closeInter();
//}
//{  signals
     signals:
        void error_found(const QString& = QString());
//}
//{  private slots
     private slots:
        void openFile(const QString& = QString());
//}
//{  private
     private:
        void setupEditor();
        void setupFileMenu();
        void setupHelpMenu();
        void createActions();
        void createConnections();
        void createMenus();
        void createToolBars();
        void createDockWindow();
        void createStatusBar();

        CodeEditor *editor;
        QMenu *fileMenu;
        QMenu *helpMenu;
        QMenu *settingsMenu;
        QTextEdit *buildLog;
        QToolBar *fileToolBar;
        QToolBar *editToolBar;
        QAction *newFileAct;
        QAction *interpAct;
        QAction *runAct;
        QAction *saveAct;
        QAction *printAct;
        QAction *undoAct;
        QAction *aboutAct;
        QAction *aboutQtAct;
        QAction *quitAct;
        QAction *configureAct;
        QAction *controlInitVars;
        QAction *traceActions;
        QAction *traceTree;
        QAction *traceSilent;
        QAction *readErrAct;
        QProcess *interpreter;
        QStringList *args;
        QTerminal *terminal;
        QDockWidget *dock;
        QListWidget *configWidget;

        _char_t *filename;
//}
  };
/* -------------------------------------------------------------------------- */
#endif
