#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QList>
#include <QMap>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QString>
#include <QIcon>
#include <QChar>
#include <QMovie>
#include <QFile>
#include <QDebug>
#include <QIntValidator>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMutex>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "sudoku.h"
#include "settings.h"

namespace Ui{
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QString name = "untitled", QWidget *parent = 0);
  ~MainWindow();
  void futureSolver();
  Sudoku* sudoku;
  Settings* settings;
  QHBoxLayout* mainlayout;
  QString name;
  QString fullName;

 signals:
  void nameChange();

 public slots:
  void about();
  void help();
  void win();
  void newSudoku();
  void getName();
  void openSudoku();
  void saveSudoku();
  void saveSudokuAs();
  void solveSudoku();
//  void hintSudoku();
  void solverFinished();
  void changeSudoku(QString str);
  void checkSudoku(QString str);
  void changeName();

 private:
  Ui::MainWindow *ui;
  void createTable();
  void closeEvent(QCloseEvent *event);
  bool eventFilter(QObject *obj, QEvent *event);
  bool saveEvent();
  QFuture<void> *future;
  QFutureWatcher<void> *watcher;
  QLineEdit *namenew;
  QMap<QString, bool> flags;
  QString urlDrag;
  QWidget* dropwidget;
  QByteArray toFile();
  void fromFile(QString str);

 protected:
  void dragEnterEvent(QDragEnterEvent* pe);
  void dropEvent(QDropEvent* pe);
};

#endif // MAINWINDOW_H
