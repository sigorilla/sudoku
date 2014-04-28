#ifndef SUDOKU_H
#define SUDOKU_H

#include "other.h"

class Sudoku : public QWidget
{
  Q_OBJECT
 public:
  Sudoku(QWidget *parent = 0);
  ~Sudoku();
  int isAvailable(int row, int col, int num);
  int fillSudoku(int row, int col);
  void checkSudoku();
  QList <QList <int> > grid;
  QList <QList <int> > gridStatic;
  QList <QList <int> > gridStart;
  QList <QList <QLineEdit *> > table;
  QList<QList<int> > unvalid;
  bool win;

 public slots:
  void createSudoku();
  bool loadSudoku();
  void clearSudoku();
  void hintSudoku();
  void showSudoku();

 private:
  void transposing();
  void swapRowsSmall();
  void swapColumnsSmall();
  void swapRowsArea();
  void swapColumnsArea();
  void mix(int amt = 15);
  void suffle(QList<int>* list);
  void startDrag();
  QPoint m_ptDragPos;
  qint32 random(qint32 A, qint32 B);
  int howmuch;
  QWidget* parent;
  QGridLayout* gridLayout;

 protected:
  void mousePressEvent(QMouseEvent* pe);
  void mouseMoveEvent(QMouseEvent* pe);
};

#endif // SUDOKU_H
