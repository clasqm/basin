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
#include <cstring>
#include <iostream>
  using std::cerr;
#include <string>
#include <cassert>
#include <cstdlib>
  // using exit()

#include "platform.h"
#include "mainwindow.h"
//#include "config.h"
// This define shold be defined in "config.h"
#ifndef PATH_TO_INTERPRETER
#define PATH_TO_INTERPRETER "./basin"
#endif
/* -------------------------------------------------------------------------- */
  MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent)
  {
     QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ("UTF-8") );
     QTextCodec::setCodecForTr       ( QTextCodec::codecForName ("UTF-8") );
     QTextCodec::setCodecForLocale   ( QTextCodec::codecForName ("UTF-8") );

     args         = new QStringList;
     interpreter  = new QProcess;
     terminal     = new QTerminal;
     editor       = new CodeEditor;
     fileMenu     = new QMenu(tr("&File"), this);
     settingsMenu = new QMenu(tr("&Settings"), this);
     helpMenu     = new QMenu(tr("&Help"), this);
     dock         = new QDockWidget(tr("Build log"), this);
     dock->setHidden(true);
     buildLog     = new QTextEdit(dock);
     filename     = new _char_t[255];
     configWidget = new QListWidget;
     configWidget->setHidden(true);

     createActions();
     createConnections();
     createToolBars();
     createStatusBar();

     setupFileMenu();
     setupHelpMenu();
     createDockWindow();
     setCentralWidget(editor);
     setWindowTitle(tr("Basin mini-IDE"));
     setWindowIcon(QIcon(":/icons/64x64.png"));

     strcpy(filename,"source2.bas");
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::createConnections()
  {
     connect(interpreter, SIGNAL(readyReadStandardError()),this,
                                                               SLOT(readErr()));

     connect(interpreter, SIGNAL(readyReadStandardOutput()),this,
                                                               SLOT(readOut()));

     connect(interpreter, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                                                            SLOT(closeInter()));

     connect(this, SIGNAL(error_found(const QString &)), editor,
                              SLOT(highlightCurrentErrorLine(const QString &)));
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::about()
  {
     QMessageBox aboutBox;
     aboutBox.about(this, tr("About Basin-IDE"),
                 tr("<p><b>Basin-IDE</b> is simple frontend to " \
                    "<b>basin</b> interpreter,simple interpreter for" \
                    " QBASIC language.</p>" \
                    "<p>Authors - <b>#pragma</b>,<b>Evg</b>.</p> "
                    "<p>Program was created on <b>www.cyberforum.ru</b></p>"));
     aboutBox.setIconPixmap(QPixmap(":icons/192x192.png","PNG"));
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::newFile()
  {
    editor->clear();
    buildLog->clear();
    dock->setHidden(true);
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::readErr()
  {
     buildLog->insertPlainText( interpreter->readAllStandardError().data() );
     dock->setHidden(false);
     emit error_found(buildLog->toPlainText());
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::readOut()
  {
     terminal->insertPlainText ( interpreter->readAllStandardOutput().data() );
     terminal->ensureCursorVisible ();
     terminal->show();
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::closeInter()
  {
     terminal->isFinished = true;
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::run()
  {
     QFile file(filename);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Dock Widgets"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(filename)
                              .arg(file.errorString()));
         return;
     }
     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << editor->toPlainText();
     QApplication::restoreOverrideCursor();

     statusBar()->showMessage(tr("Saved '%1'").arg(filename), 2000);

     buildLog->clear();
     dock->setHidden(true);
     interpreter->start(PATH_TO_INTERPRETER,QStringList() << *args << filename);
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::interp()
  {
     QFile file(filename);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Dock Widgets"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(filename)
                              .arg(file.errorString()));
         return;
     }
     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << editor->toPlainText();
     QApplication::restoreOverrideCursor();

     statusBar()->showMessage(tr("Saved '%1'").arg(filename), 2000);

     buildLog->clear();
     dock->setHidden(true);
     interpreter->start(PATH_TO_INTERPRETER,QStringList() << *args
                                                          << "--syntax-only"
                                                          << filename         );
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::openFile(const QString &path)
  {
     QString fileName = path;

     if (fileName.isNull())
         fileName = QFileDialog::getOpenFileName(this,
             tr("Open File"), "", "QBasic files(*.bas)");

     if (!fileName.isEmpty()) {
         QFile file(fileName);
         std::string str = fileName.toStdString();
         strcpy(filename,str.c_str());
         if (file.open(QFile::ReadWrite | QFile::Text))
            editor->setPlainText(file.readAll());
     }
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::configure()
  {
     configWidget->show();
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::setupFileMenu()
  {
     menuBar()->addMenu(fileMenu);

     fileMenu->addAction(tr("&New"), this, SLOT(newFile()),
                         QKeySequence(tr("Ctrl+N",
                                         "File|New")));
     fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                         QKeySequence(tr("Ctrl+O",
                                         "File|Open")));
     fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()),
                         QKeySequence(tr("Ctrl+Q",
                                         "File|Exit")));

     fileMenu = menuBar()->addMenu(tr("&Project"));
     fileMenu->addAction(controlInitVars);
     fileMenu->addAction(traceActions);
     fileMenu->addAction(traceTree);
     fileMenu->addAction(traceSilent);
     fileMenu = menuBar()->addMenu(tr("&View"));

     menuBar()->addMenu(settingsMenu);
     settingsMenu->addAction(tr("&Configure..."), this, SLOT(configure()),
                             QKeySequence(tr("Ctrl+1",
                                                  "Settings|Configure...")));
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::setupHelpMenu()
  {
     menuBar()->addMenu(helpMenu);
     helpMenu->addAction(tr("&About"), this, SLOT(about()));
     helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::toggleInitVars()
  {
    if (args->contains("--noinit-errors"))
    {
       if (!args->removeOne("--noinit-errors"))
       {
          cerr << "Cannot remove parameter in args list,"
               << "mainwindow.cpp,in void MainWindow::toggleInitVars()";
          exit(EXIT_FAILURE);
       }
       return;
    }
    args->append("--noinit-errors");
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::toggleTraceActions()
  {
    if (args->contains("--trace-act"))
    {
       if (!args->removeOne("--trace-act"))
       {
          cerr << "Cannot remove parameter in args list,"
               << "mainwindow.cpp,in void MainWindow::toggleTraceActions()";
          exit(EXIT_FAILURE);
       }
       return;
    }
    args->append("--trace-act");
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::toggleTraceTree()
  {
    if (args->contains("--trace-tree"))
    {
       if (!args->removeOne("--trace-tree"))
       {
          cerr << "Cannot remove parameter in args list,"
               << "mainwindow.cpp,in void MainWindow::toggleTraceTree()";
          exit(EXIT_FAILURE);
       }
       return;
    }
    args->append("--trace-tree");
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::toggleTraceSilent()
  {
    if (args->contains("--trace-silent"))
    {
       if (!args->removeOne("--trace-silent"))
       {
          cerr << "Cannot remove parameter in args list,"
               << "mainwindow.cpp,in void MainWindow::toggleTraceSilent()";
          exit(EXIT_FAILURE);
       }
       return;
    }
    args->append("--trace-silent");
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::print()
  {
     QTextDocument *document = editor->document();
     QPrinter printer;

     QPrintDialog *dlg = new QPrintDialog(&printer, this);
     if (dlg->exec() != QDialog::Accepted)
         return;

     document->print(&printer);

     statusBar()->showMessage(tr("Ready"), 2000);
     delete dlg;
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::save()
  {
     QString fileName = QFileDialog::getSaveFileName(this,
                         tr("Choose a file name"), ".",
                         tr("BAS (*.bas)"));
     if (fileName.isEmpty())
        return;
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Dock Widgets"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }
     QTextStream out(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     out << editor->toPlainText();
     QApplication::restoreOverrideCursor();

     statusBar()->showMessage(tr("Saved '%1'").arg(fileName), 2000);
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::undo()
  {
     QTextDocument *document = editor->document();
     document->undo();
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::createStatusBar()
  {
     statusBar()->showMessage(tr("Ready"));
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::createActions()
  {
     newFileAct = new QAction(QIcon(":/images/new.png"),tr("&New source"),this);
     newFileAct->setShortcut(tr("Ctrl+N"));
     newFileAct->setStatusTip(tr("Create a new source"));
     connect(newFileAct, SIGNAL(triggered()), this, SLOT(newFile()));

     interpAct = new QAction(QIcon(":/images/interpret.png"),
                                                         tr("&Interpret"),this);
     interpAct->setShortcut(tr("F5"));
     interpAct->setStatusTip(tr("Interpret current source"));
     connect(interpAct, SIGNAL(triggered()), this, SLOT(interp()));

     runAct = new QAction(QIcon(":/images/run.png"), tr("&Run"), this);
     runAct->setShortcut(tr("F9"));
     runAct->setStatusTip(tr("Run current source"));
     connect(runAct, SIGNAL(triggered()), this, SLOT(run()));

     saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
     saveAct->setShortcut(tr("Ctrl+S"));
     saveAct->setStatusTip(tr("Save the current source"));
     connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

     printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
     printAct->setShortcut(tr("Ctrl+P"));
     printAct->setStatusTip(tr("Print the source"));
     connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

     undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
     undoAct->setShortcut(tr("Ctrl+Z"));
     undoAct->setStatusTip(tr("Undo the last editing action"));
     connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

     quitAct = new QAction(tr("&Quit"), this);
     quitAct->setShortcut(tr("Ctrl+Q"));
     quitAct->setStatusTip(tr("Quit the application"));
     connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

     controlInitVars = new QAction(tr("&Control initialization"), this);
     controlInitVars->setCheckable(true);
     controlInitVars->setStatusTip(tr("Control initialization of variables"));
     connect(controlInitVars,SIGNAL(triggered()),this,SLOT(toggleInitVars()));

     traceActions = new QAction(tr("&Trace actions"), this);
     traceActions->setCheckable(true);
     traceActions->setStatusTip(tr("Tracing of performed actions"));
     connect(traceActions,SIGNAL(triggered()),this,SLOT(toggleTraceActions()));

     traceTree = new QAction(tr("&Trace tree"), this);
     traceTree->setCheckable(true);
     traceTree->setStatusTip(tr("Tracing of program tree"));
     connect(traceTree, SIGNAL(triggered()), this, SLOT(toggleTraceTree()));

     traceSilent = new QAction(tr("&Silent tracing"), this);
     traceSilent->setCheckable(true);
     traceSilent->setStatusTip(tr("Tracing saved to file"));
     connect(traceSilent, SIGNAL(triggered()), this, SLOT(toggleTraceSilent()));

     configureAct = new QAction(tr("&Configure"), this);
     configureAct->setShortcut(tr("Ctrl+1"));
     configureAct->setStatusTip(tr("Configure application"));
     connect(configureAct, SIGNAL(triggered()), this, SLOT(configure()));
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::createToolBars()
  {
     fileToolBar = addToolBar(tr("File"));
     fileToolBar->addAction(newFileAct);
     fileToolBar->addAction(interpAct);
     fileToolBar->addAction(runAct);
     fileToolBar->addAction(saveAct);
     fileToolBar->addAction(printAct);

     editToolBar = addToolBar(tr("Edit"));
     editToolBar->addAction(undoAct);

     QSize size(36,36);
     fileToolBar->setIconSize(size);
     editToolBar->setIconSize(size);
  }
/* -------------------------------------------------------------------------- */
  void MainWindow::createDockWindow()
  {
     dock->setMinimumHeight(10);
     dock->setMaximumHeight(100);
     QFont font;
     font.setFixedPitch(true);
     font.setPointSize(10);
     font.setStyleHint(QFont::System,QFont::PreferAntialias);

     QTextCharFormat format;
     format.setForeground(Qt::red);

     buildLog->setReadOnly(true);
     buildLog->setWordWrapMode(QTextOption::WrapAnywhere);
     buildLog->setFont(font);
     buildLog->mergeCurrentCharFormat(format);

     addDockWidget(Qt::BottomDockWidgetArea, dock);
     dock->setWidget(buildLog);
     fileMenu->addAction(dock->toggleViewAction());
  }
/* -------------------------------------------------------------------------- */
  MainWindow::~MainWindow ()
  {
        delete editor;
        delete fileMenu;
        delete helpMenu;
        delete settingsMenu;
        delete buildLog;
        delete fileToolBar;
        delete editToolBar;
        delete newFileAct;
        delete interpAct;
        delete runAct;
        delete saveAct;
        delete printAct;
        delete undoAct;
        delete aboutAct;
        delete aboutQtAct;
        delete quitAct;
        delete configureAct;
        delete controlInitVars;
        delete traceActions;
        delete traceTree;
        delete traceSilent;
        delete readErrAct;
        delete interpreter;
        delete args;
        delete terminal;
        delete dock;

        delete filename;
  }
/* -------------------------------------------------------------------------- */
#endif
/* -------------------------------------------------------------------------- */
