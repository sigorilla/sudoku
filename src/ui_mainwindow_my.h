#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
 public:
  QAction *actionNew;
  QAction *actionOpen;
  QAction *actionSave;
  QAction *actionSaveAs;
  QAction *actionClear;
  QAction *actionLoad;
  QAction *actionCreate;
  QAction *actionSolve;
  QAction *actionHint;
  QAction *actionQuit;
  QAction *actionHelp;
  QAction *actionAbout;
  QAction *actionLogin;
  QAction *actionSet;
  QAction *actionServer;
  QAction *actionClient;
  QMenuBar *menuBar;
  QMenu *menuFile;
  QMenu *menuGame;
  QMenu *menuHelp;
  QToolBar *mainToolBar;
  QWidget *centralWidget;
  QStatusBar *statusBar;
  QIcon icon;

  void setupUi(QMainWindow *MainWindow)
  {
    if (MainWindow->objectName().isEmpty())
      MainWindow->setObjectName(QStringLiteral("MainWindow"));
    icon.addFile(QStringLiteral(":/images/icon.gif"), QSize(),
                 QIcon::Normal, QIcon::Off);
    MainWindow->resize(500, 500);
    MainWindow->setWindowIcon(icon);
    actionNew = new QAction(MainWindow);
    actionNew->setObjectName(QStringLiteral("actionNew"));
    actionOpen = new QAction(MainWindow);
    actionOpen->setObjectName(QStringLiteral("actionOpen"));
    actionSave = new QAction(MainWindow);
    actionSave->setObjectName(QStringLiteral("actionSave"));
    actionSaveAs = new QAction(MainWindow);
    actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
    actionClear = new QAction(MainWindow);
    actionClear->setObjectName(QStringLiteral("actionClear"));
    actionLoad = new QAction(MainWindow);
    actionLoad->setObjectName(QStringLiteral("actionLoad"));
    actionCreate = new QAction(MainWindow);
    actionCreate->setObjectName(QStringLiteral("actionCreate"));
    actionSolve = new QAction(MainWindow);
    actionSolve->setObjectName(QStringLiteral("actionSolve"));
    actionHint = new QAction(MainWindow);
    actionHint->setObjectName(QStringLiteral("actionHint"));
    actionQuit = new QAction(MainWindow);
    actionQuit->setObjectName(QStringLiteral("actionQuit"));
    actionHelp = new QAction(MainWindow);
    actionHelp->setObjectName(QStringLiteral("actionHelp"));
    actionAbout = new QAction(MainWindow);
    actionAbout->setObjectName(QStringLiteral("actionAbout"));
    actionLogin = new QAction(MainWindow);
    actionLogin->setObjectName(QStringLiteral("actionLogin"));
    actionSet = new QAction(MainWindow);
    actionSet->setObjectName(QStringLiteral("actionSet"));
    actionServer = new QAction(MainWindow);
    actionServer->setObjectName(QStringLiteral("actionServer"));
    actionClient = new QAction(MainWindow);
    actionClient->setObjectName(QStringLiteral("actionClient"));

    menuBar = new QMenuBar(MainWindow);
    menuFile = new QMenu(menuBar);
    menuFile->setObjectName(QStringLiteral("menuFile"));
    menuHelp = new QMenu(menuBar);
    menuHelp->setObjectName(QStringLiteral("menuHelp"));
    menuGame = new QMenu(menuBar);
    menuGame->setObjectName(QStringLiteral("menuGame"));
    menuBar->setObjectName(QStringLiteral("menuBar"));
    MainWindow->setMenuBar(menuBar);

    mainToolBar = new QToolBar(MainWindow);
    mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
//    MainWindow->addToolBar(mainToolBar);
    mainToolBar->setIconSize(QSize(40, 40));

    centralWidget = new QWidget(MainWindow);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    MainWindow->setCentralWidget(centralWidget);

    statusBar = new QStatusBar(MainWindow);
    statusBar->setObjectName(QStringLiteral("statusBar"));
    MainWindow->setStatusBar(statusBar);

    menuBar->addAction(menuFile->menuAction());
//    menuBar->addAction(menuGame->menuAction());
    menuBar->addAction(menuHelp->menuAction());
    menuFile->addAction(actionNew);
    menuFile->addAction(actionOpen);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionSaveAs);
    menuFile->addSeparator();
    menuFile->addAction(actionClear);
    menuFile->addAction(actionLoad);
    menuFile->addAction(actionCreate);
    menuFile->addAction(actionSolve);

    menuFile->addSeparator();
    menuFile->addAction(actionSet);
    menuFile->addSeparator();
    menuFile->addAction(actionQuit);
    menuGame->addAction(actionServer);
    menuGame->addAction(actionClient);
    menuHelp->addAction(actionHelp);
    menuHelp->addAction(actionAbout);

    mainToolBar->addAction(actionNew);
    mainToolBar->addAction(actionSave);
    mainToolBar->addAction(actionOpen);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionClear);
    mainToolBar->addAction(actionLoad);
    mainToolBar->addAction(actionCreate);
    mainToolBar->addAction(actionSolve);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionSet);
    mainToolBar->addAction(actionHelp);
    mainToolBar->addAction(actionAbout);
    mainToolBar->addAction(actionQuit);

    retranslateUi(MainWindow);

    QMetaObject::connectSlotsByName(MainWindow);
  } // setupUi

  void retranslateUi(QMainWindow *MainWindow)
  {
    MainWindow->setWindowTitle(QApplication::translate("MainWindow",
                                                       "Sudoku", 0));
    actionNew->setText(QApplication::translate("MainWindow",
                                               "&New Game", 0));
    actionNew->setShortcut(QApplication::translate("MainWindow",
                                                   "Ctrl+N", 0));
    actionNew->setIcon(QIcon(":/images/new.gif"));
    actionOpen->setText(QApplication::translate("MainWindow", "&Open", 0));
    actionOpen->setShortcut(QApplication::translate("MainWindow",
                                                    "Ctrl+O", 0));
    actionOpen->setIcon(QIcon(":/images/open.gif"));
    actionSave->setText(QApplication::translate("MainWindow", "&Save", 0));
    actionSave->setShortcut(QApplication::translate("MainWindow",
                                                    "Ctrl+S", 0));
    actionSave->setIcon(QIcon(":/images/save.gif"));
    actionSaveAs->setText(QApplication::translate("MainWindow", "Save &as", 0));
    actionSaveAs->setShortcut(QApplication::translate("MainWindow",
                                                    "Ctrl+Shift+S", 0));
//    actionSaveAs->setIcon(QIcon(":/images/save.gif"));
    actionClear->setText(QApplication::translate("MainWindow", "Clea&r", 0));
    actionClear->setIcon(QIcon(":/images/clear.gif"));
//    actionClear->setShortcut(QApplication::translate("MainWindow", "Ctrl+R", 0));
    actionLoad->setText(QApplication::translate("MainWindow", "&Load", 0));
    actionLoad->setIcon(QIcon(":/images/load.gif"));
//    actionLoad->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", 0));
    actionCreate->setText(QApplication::translate("MainWindow", "&Create", 0));
    actionCreate->setIcon(QIcon(":/images/create.png"));
//      actionCreate->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", 0));
    actionSolve->setText(QApplication::translate("MainWindow", "Solv&e", 0));
    actionSolve->setIcon(QIcon(":/images/solve.gif"));
//      actionSolve->setShortcut(QApplication::translate("MainWindow", "Ctrl+E", 0));
    actionHint->setText(QApplication::translate("MainWindow", "Hin&t", 0));
//      actionHint->setShortcut(QApplication::translate("MainWindow", "Ctrl+T", 0));
    actionQuit->setText(QApplication::translate("MainWindow", "&Quit", 0));
    actionQuit->setShortcut(QApplication::translate("MainWindow",
                                                    "Ctrl+Q", 0));
    actionQuit->setIcon(QIcon(":/images/quit.gif"));
    actionHelp->setText(QApplication::translate("MainWindow", "Hel&p", 0));
    actionHelp->setShortcut(QApplication::translate("MainWindow", "F1", 0));
    actionHelp->setIcon(QIcon(":/images/help.gif"));
    actionAbout->setText(QApplication::translate("MainWindow", "&About", 0));
    actionAbout->setIcon(QIcon(":/images/about.gif"));
//      actionAbout->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", 0));
    actionLogin->setText(QApplication::translate("MainWindow", "Login", 0));
    menuFile->setTitle(QApplication::translate("MainWindow", "&File", 0));
    menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", 0));
    menuGame->setTitle(QApplication::translate("MainWindow", "&Game", 0));
    actionSet->setText(QApplication::translate("MainWindow", "Se&ttings", 0));
    actionSet->setIcon(QIcon(":/images/set.gif"));
  } // retranslateUi

};

namespace Ui {

class MainWindow: public Ui_MainWindow {};

} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
