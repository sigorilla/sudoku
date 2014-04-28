#include "sudoku.h"
#include <QIntValidator>
#include "QTime"

Sudoku::Sudoku(QWidget *parent) {
//  setAcceptDrops(true);
  this->parent = parent;
  win = false;
  howmuch = 0;

  QVBoxLayout *vl = new QVBoxLayout();
  vl->setSpacing(0);
  vl->setAlignment(Qt::AlignHCenter);
  QLabel *drag = new QLabel("...");
  drag->setMaximumSize(100, 50);
  gridLayout = new QGridLayout();
  gridLayout->setSpacing(2);
  for (int i = 0; i < 9; i++) {
    grid.append(QList<int>());
    gridStatic.append(QList<int>());
    gridStart.append(QList<int>());
    table.append(QList<QLineEdit *>());
    unvalid.append(QList<int>());
  }
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      QLineEdit *b = new QLineEdit();
      b->setInputMethodHints(Qt::ImhDigitsOnly);
      b->setMaxLength(1);
      b->setAlignment(Qt::AlignCenter);
      b->setMinimumHeight(50);
      b->setMinimumWidth(50);
      b->setMaximumHeight(100);
      b->setMaximumWidth(100);
      QIntValidator *validator = new QIntValidator(1, 9, b);
      b->setValidator(validator);
      b->setFocusPolicy(Qt::ClickFocus);
      b->installEventFilter(this->parent);
      b->setObjectName(QString::number(i) + QString::number(j));
      b->setProperty("not", false);
      connect(b, SIGNAL(textChanged(QString)), this->parent, SLOT(changeSudoku(QString)));
      connect(b, SIGNAL(textEdited(QString)), this->parent, SLOT(checkSudoku(QString)));

      if (i % 3 == 0 && j % 3 == 0)
        b->setStyleSheet("QLineEdit{ border-color: #333 #ccc #ccc #333; }");
      if (i % 3 == 2 && j % 3 == 2)
        b->setStyleSheet("QLineEdit{ border-color: #ccc #333 #333 #ccc; }");
      if (i % 3 == 0 && j % 3 == 2)
        b->setStyleSheet("QLineEdit{ border-color: #333 #333 #ccc #ccc; }");
      if (i % 3 == 2 && j % 3 == 0)
        b->setStyleSheet("QLineEdit{ border-color: #ccc #ccc #333 #333; }");
      if (i % 3 == 2 && j % 3 == 1)
        b->setStyleSheet("QLineEdit{ border-color: #ccc #ccc #333 #ccc; }");
      if (i % 3 == 0 && j % 3 == 1)
        b->setStyleSheet("QLineEdit{ border-color: #333 #ccc #ccc #ccc; }");
      if (i % 3 == 1 && j % 3 == 0)
        b->setStyleSheet("QLineEdit{ border-color: #ccc #ccc #ccc #333; }");
      if (i % 3 == 1 && j % 3 == 2)
        b->setStyleSheet("QLineEdit{ border-color: #ccc #333 #ccc #ccc; }");
      grid[i] << 0;
      gridStatic[i] << 0;
      gridStart[i] << 0;
      unvalid[i] << 0;
      table[i] << b;
    }

  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
    {
      table[0][0]->setFocus(Qt::OtherFocusReason);
      gridLayout->addWidget(table[i][j], i, j);
    }

//  vl->addWidget(drag);
  vl->addLayout(gridLayout);
  this->setLayout(vl);

  createSudoku();
}

Sudoku::~Sudoku() {

}

void Sudoku::mousePressEvent(QMouseEvent *pe) {
  if (pe->button() == Qt::LeftButton) {
    m_ptDragPos = pe->pos();
  }
  QWidget::mousePressEvent(pe);
}

void Sudoku::mouseMoveEvent(QMouseEvent *pe) {
  if (pe->buttons() & (pe->pos() - m_ptDragPos).manhattanLength()) {
    int distance = (pe->pos() - m_ptDragPos).manhattanLength();
    if (distance > QApplication::startDragDistance()) {
//      startDrag();
    }
  }
  QWidget::mouseMoveEvent(pe);
}

void Sudoku::startDrag() {
  QMimeData* pMimeData = new QMimeData;
  pMimeData->setText("1");
  QDrag* pDrag = new QDrag(this);
  pDrag->setMimeData(pMimeData);
  pDrag->setPixmap(QPixmap(":/images/open.gif"));
  pDrag->exec();
}

void Sudoku::clearSudoku() {
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      grid[i][j] = 0;
      gridStatic[i][j] = 0;
      gridStart[i][j] = 0;
      table[i][j]->setReadOnly(false);
      table[i][j]->setProperty("not", false);
    }
  win = false;
  showSudoku();
}

// return unvalid cells
void Sudoku::checkSudoku() {
  howmuch = 0;
  win = false;
  for (int i = 0; i < 9; ++i) {
    unvalid.append(QList<int>());
    for (int j = 0; j < 9; ++j) {
      unvalid[i].insert(j, 0);
      bool check = true;
      if (table[i][j]->isReadOnly()) {
        howmuch++;
        continue;
      }
      int num = grid[i][j];
      int rowStart = (i/3) * 3;
      int colStart = (j/3) * 3;
      for (int k = 0; k < 9; ++k ) {
        if (grid[i][k] == num && k != j && num != 0) check = false;
        if (grid[k][j] == num && k != i && num != 0) check = false;
        if (grid[rowStart + (k % 3)][colStart + (k / 3)] == num &&
          (rowStart + (k % 3)) != i && (colStart + (k / 3)) != j && num != 0)
          check = false;
      }
      if (!check) unvalid[i][j] = 1;
      else if (num != 0) howmuch++;
    }
  }
  if (howmuch == 81) {
    win = true;
    howmuch = 0;
  }
}

void Sudoku::showSudoku() {
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      if (grid[i][j] != 0)
        table[i][j]->setText(QString::number(grid[i][j]));
      else
        table[i][j]->setText("");
    }
}

bool Sudoku::loadSudoku() {
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (!table[i][j]->property("not").toBool()) {
        grid[i][j] = table[i][j]->text().toInt();
      } else {
//        sudoku->grid[i][j] = 0;
        QMessageBox::critical(this->parent, QStringLiteral("Error"),
                              "Check your grid");
        return false;
      }

  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (grid[i][j] != 0) {
        table[i][j]->setReadOnly(true);
        table[i][j]->setText(QString::number(grid[i][j]));
      }
  return true;
}

// Solve
void Sudoku::hintSudoku() {
    // Make hint
}

int Sudoku::isAvailable(int row, int col, int num) {
  int rowStart = (row/3) * 3;
  int colStart = (col/3) * 3;

  for (int i = 0; i < 9; ++i ) {
    if (grid[row][i] == num) return 0;
    if (grid[i][col] == num) return 0;
    if (grid[rowStart + (i % 3)][colStart + (i / 3)] == num) return 0;
  }

  return 1;
}

int Sudoku::fillSudoku(int row, int col) {
  if (row < 9 && col < 9) {
    if(grid[row][col] != 0) {
      if ((col + 1) < 9) return fillSudoku(row, col + 1);
      else if ((row + 1) < 9) return fillSudoku(row + 1, 0);
      else return 1;
    } else {
      for (int i = 0; i < 9; ++i) {
        if (isAvailable(row, col, i + 1)) {
          grid[row][col] = i+1;
          if ((col + 1) < 9) {
            if (fillSudoku(row, col + 1)) return 1;
            else grid[row][col] = 0;
          } else if ((row + 1) < 9) {
            if (fillSudoku(row + 1, 0)) return 1;
            else grid[row][col] = 0;
          } else {
            return 1;
          }
        }
      }
    }
    return 0;
  } else {
    return 1;
  }
}


// Create
void Sudoku::createSudoku() {
  clearSudoku();
  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

  QList<int> tmp;
  int N = 70;

  for (int i = 0; i < 9; ++i) {
    tmp << 0;
    for (int j = 0; j < 9; ++j) {
//      grid[i][j] = ((i*N + i/N + j) % (N*N) + 1);
      grid[i][j] = j+1;
      table[i][j]->setReadOnly(true);
    }
  }

  suffle(&grid[0]);

  for (int i = 0; i < 3; ++i) {
    grid[i*3] = grid[0].mid(9-i) + grid[0].mid(0, 9-i);
    grid[i*3+1] = grid[i*3].mid(6) + grid[i*3].mid(0, 6);
    grid[i*3+2] = grid[i*3].mid(3) + grid[i*3].mid(0, 3);
  }

  for (int i = 0; i < 3; ++i) {
    int r1 = random(i*3, i*3+2);
    int r2 = random(i*3, i*3+2);
    while (r1 == r2)
      r2 = random(i*3, i*3+2);
    int r3 = random(i*3, i*3+2);
    int r4 = random(i*3, i*3+2);
    while (r3 == r4)
      r4 = random(i*3, i*3+2);

    grid.swap(r1, r2);
    for (int j = 0; j < 9; ++j)
      grid[j].swap(r3, r4);
  }

//  mix(500);

  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      gridStatic[i][j] = grid[i][j];

  int m, n;
  for (int i = 0; i < N; i++) {
    n = random(0, 8);
    m = random(0, 8);
    grid[n][m] = 0;
    table[n][m]->setReadOnly(false);
  }

  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      gridStart[i][j] = grid[i][j];

  showSudoku();
}

void Sudoku::transposing() {
  int tmp;
  for (int i = 0; i < 9; i++)
    for (int j = i; j < 9; j++) {
      tmp = grid[i][j];
      grid[i][j] = grid[j][i];
      grid[j][i] = tmp;
    }
}

void Sudoku::swapRowsSmall() {
  int area = qrand() % 3;
  int line1 = qrand() % 3;
  int N1 = area*3 + line1;
  int line2 = qrand() % 3;
  while (line1 == line2)
    line2 = qrand() % 3;
  int N2 = area*3 + line2;
  int tmp;
  for (int i = 0; i < 9; i++) {
    tmp = grid[N1][i];
    grid[N1][i] = grid[N2][i];
    grid[N2][i] = tmp;
  }
}

void Sudoku::swapColumnsSmall() {
  transposing();
  swapRowsSmall();
  transposing();
}

void Sudoku::swapRowsArea() {
  int area1 = qrand() % 3;
  int area2 = qrand() % 3;
  while (area1 == area2)
    area2 = qrand() % 3;
  int N1, N2, tmp;
  for (int j = 0; j < 3; j++) {
    N1 = area1*3 + j;
    N2 = area2*3 + j;
    for (int i = 0; i < 9; i++) {
      tmp = grid[N1][i];
      grid[N1][i] = grid[N2][i];
      grid[N2][i] = tmp;
    }
  }
}

void Sudoku::swapColumnsArea() {
  transposing();
  swapRowsArea();
  transposing();
}

void Sudoku::suffle(QList<int>* list) {
  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
  int i, j;
  for (i = 8; i > 0; i--) {
    j = qrand() % i;
    list->swap(i, j);
  }
}

qint32 Sudoku::random(qint32 A, qint32 B) {
  return qrand()%(B-A+1)+A;
}

void Sudoku::mix(int amt) {
  int rd = qrand() % 100 + 80;
//  qDebug() << rd << amt;
  for (int i = 0; i < rd; i++) {
    transposing();
    swapColumnsArea();
    swapRowsArea();
    transposing();
    swapRowsSmall();
    transposing();
    swapRowsSmall();
    swapColumnsSmall();
    swapColumnsArea();
    transposing();
    swapColumnsArea();
    swapRowsSmall();
    transposing();

    /*switch (qrand() % 5) {
      case 0:
        transposing();
        break;
      case 1:
        swapRowsSmall();
        break;
      case 2:
        swapColumnsSmall();
        break;
      case 3:
        swapRowsArea();
        break;
      case 4:
        swapColumnsArea();
        break;
    }*/
  }
}

// grid = array[size][size]
// void algorithmX(int size, grid) {
//   int R, C, N;
//   R = size;
//   C = size;
//   N = R*C;
  
// }
