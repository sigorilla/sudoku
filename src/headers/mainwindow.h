#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include "other.h"
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
