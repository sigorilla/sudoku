#include <QDrag>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QScrollArea>
#include <QDesktopWidget>
#include "QTime"

#include "mainwindow.h"
#include "ui_mainwindow_my.h"
#include "settings.h"

QTime midnight(0, 0, 0);

MainWindow::MainWindow(QString name, QWidget *parent) :
 QMainWindow(parent),
 ui (new Ui::MainWindow),
 name(name) {
  ui->setupUi(this);
  setWindowTitle(this->name + " | Sudoku");
  sudoku = new Sudoku(this);
  settings = new Settings(this);
  future = new QFuture<void>;
  watcher = new QFutureWatcher<void>;

  setAcceptDrops(!acceptDrops());

  QBrush* brush = new QBrush(QColor(255, 255, 255));
  QPalette* palette = new QPalette;
  palette->setBrush(QPalette::Window, *brush);
  this->setPalette(*palette);

  QDesktopWidget widget;
  QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());

  restoreGeometry(settings->settings->value("general/geometry").toByteArray());
  restoreState(settings->settings->value("general/state").toByteArray());

  if ((geometry().x() + 40 + geometry().width()) > mainScreenSize.width()
      || (geometry().y() + 40 + geometry().height()) > mainScreenSize.height())
    setGeometry(40, 60, geometry().width(), geometry().height());
  else
    setGeometry(geometry().x() + 40, geometry().y() + 60, geometry().width(), geometry().height());

  flags.insert("changed", false);
  flags.insert("saved", false);
  flags.insert("droped", true);
  flags.insert("dragged", false);

  connect(this, SIGNAL(nameChange()), this, SLOT(changeName()));
  connect(watcher, SIGNAL(finished()), this, SLOT(solverFinished()));

  connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newSudoku()));
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(getName()));
  connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveSudoku()));
  connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveSudokuAs()));
  connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
  connect(ui->actionClear, SIGNAL(triggered()), sudoku, SLOT(clearSudoku()));
  connect(ui->actionLoad, SIGNAL(triggered()), sudoku, SLOT(loadSudoku()));
  connect(ui->actionCreate, SIGNAL(triggered()), sudoku, SLOT(createSudoku()));
  connect(ui->actionSolve, SIGNAL(triggered()), this, SLOT(solveSudoku()));
  connect(ui->actionSet, SIGNAL(triggered()), settings, SLOT(exec()));

  createTable();

}

MainWindow::~MainWindow() {
  delete ui;
  delete sudoku;
}

void MainWindow::newSudoku() {
  settings->settings->setValue("general/geometry", this->saveGeometry());
  settings->settings->setValue("general/state", this->saveState());

  MainWindow *window = new MainWindow();
  window->show();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* pe) {
  if (pe->mimeData()->hasFormat("text/uri-list") && flags["droped"]) {
    if (pe->mimeData()->urls().size() == 1) {
      QFileInfo info(pe->mimeData()->urls()[0].toLocalFile());
      if (info.completeSuffix() == "sudoku")
        pe->acceptProposedAction();
    }
  }
}

void MainWindow::dropEvent(QDropEvent* pe) {
  flags["dragged"] = true;
  fullName = pe->mimeData()->urls()[0].toLocalFile();
  openSudoku();
  qDebug() << "Dropped" << fullName;
  QFileInfo info(fullName);
  name = info.baseName();
  flags["changed"] = false;
}

void MainWindow::changeName() {
  QString title = name + " | Sudoku";

  if ( !flags["changed"] ) {
    flags["changed"] = true;
    setWindowTitle("* " + title);
  }
}

// create grid
void MainWindow::createTable()
{
  QHBoxLayout *mainvert = new QHBoxLayout();

  QSpacerItem *spacer;
  spacer = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum);

  mainvert->addSpacerItem(spacer);
  mainvert->addWidget(sudoku);
  mainvert->addSpacerItem(spacer);
  mainvert->setSpacing(15);

  this->mainlayout = mainvert;
  ui->centralWidget->setLayout(this->mainlayout);

}

// change grid
void MainWindow::changeSudoku(QString str) {
  QLineEdit* edit = qobject_cast<QLineEdit*>(QObject::sender());
  if (edit) {
    emit nameChange();
  }
}

// check in-typing
void MainWindow::checkSudoku(QString str) {
  QLineEdit* edit = qobject_cast<QLineEdit*>(QObject::sender());
  if (edit) {
    QString tname = edit->objectName();
    int id = tname.mid(0, 2).toInt();
    int n = id / 10;
    int m = id % 10;
    int num = str.toInt();
    sudoku->grid[n][m] = num;
    sudoku->checkSudoku();
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        sudoku->table[i][j]->setProperty("not", false);
        if (sudoku->unvalid[i][j] == 1 && sudoku->grid[i][j] != 0)
          sudoku->table[i][j]->setProperty("not", true);
        style()->unpolish(sudoku->table[i][j]);
        style()->polish(sudoku->table[i][j]);
      }
    }
    if (sudoku->win)
      win();
    emit nameChange();
  }
}

// solve
void MainWindow::solveSudoku() {
  qDebug() << "solve";
  if (!sudoku->loadSudoku()) return;

  ui->statusBar->showMessage("Solving...");
  this->menuBar()->setEnabled(false);
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      sudoku->table[i][j]->setEnabled(!sudoku->table[i][j]->isEnabled());
    }
  this->setCursor(Qt::WaitCursor);

  *future = QtConcurrent::run(this, &MainWindow::futureSolver);
  watcher->setFuture(*future);
}

void MainWindow::futureSolver() {
  if (!sudoku->fillSudoku(0, 0))
    QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("No Solution"));
}

void MainWindow::solverFinished() {
  menuBar()->setEnabled(!menuBar()->isEnabled());
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      sudoku->table[i][j]->setEnabled(!sudoku->table[i][j]->isEnabled());

  setCursor(Qt::ArrowCursor);
  ui->statusBar->clearMessage();

  sudoku->showSudoku();
}

// create subwindows
void MainWindow::help() {
  qDebug() << "help";
  QString text = "<h1>Help</h1>"
      "<div>Вы можете загрузить судоку либо с помощью ...</div>";
  QMessageBox help;
  help.about(this, "Help", text);
  help.show();
}

void MainWindow::about() {
  qDebug() << "about";
  QString text = "Проект по информатике «Решатель судоку» представляет собой"
                 " графическое кроссплатформенное приложение,"
                 " написанное на языке программирования C++"
                 " с использование сторонней библиотеки Qt 5.2.";
  QMessageBox about;
  about.about(this, "About Sudoku", text);
  about.show();
}

void MainWindow::win() {
  qDebug() << "win";
  QString text = "Yeah! You're very clever!";
  QMessageBox::information(this, "Congratulations!", text);
}

// open|save game
void MainWindow::getName() {

  QString tmp;
  tmp = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("Open File"),
        settings->settings->value("general/dir").toString(),
        QStringLiteral("Sudoku Files (*.sudoku);; All Files (*)"));
  if (tmp != "") {
    fullName = tmp;
    openSudoku();
  }
}

void MainWindow::openSudoku() {
  qDebug() << "open";

  if (!saveEvent()) return;
  if (fullName/*tmp*/ != "") {
//    fullName = tmp;

    QFile file(fullName);
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::critical(this, QStringLiteral("Error"),  QStringLiteral("Cannot open file"));
      return;
    }

    QFileInfo info(fullName);
    name = info.baseName();
//    qDebug() << name;

    QString contents = file.readAll().constData();
    fromFile(contents);

    sudoku->checkSudoku();
    for (int i = 0; i < 9; ++i) {
      for (int j = 0; j < 9; ++j) {
        sudoku->table[i][j]->setProperty("not", false);
        if (sudoku->unvalid[i][j] == 1)
          sudoku->table[i][j]->setProperty("not", true);
        style()->unpolish(sudoku->table[i][j]);
        style()->polish(sudoku->table[i][j]);
      }
    }

    file.close();
    sudoku->showSudoku();
    flags["changed"] = false;
    flags["saved"] = true;
    emit nameChange();
  }
}

void MainWindow::saveSudoku() {
  qDebug() << "save";

  QString tmp;
  tmp = (fullName.isEmpty()) ? settings->settings->value("general/dir").toString() : fullName;
  if (!flags["saved"])
    tmp = QFileDialog::getSaveFileName(
                  this,
                  tr("Save File"),
                  tmp + name,
                  tr("Sudoku Files (*.sudoku);; All Files (*)"));

  if (tmp != "") {

    QFile file(tmp);
//    qDebug() << fullName;
    if (!file.open(QIODevice::WriteOnly)) {
      QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Cannot open file"));
      flags["changed"] = true;
      return;
    } else {
      file.write(toFile());
      file.close();
      fullName = tmp;
      QFileInfo info(fullName);
      name = info.baseName();
      this->setWindowTitle(name + " - Sudoku");
      flags["changed"] = false;
      flags["saved"] = true;
    }
  }
}

void MainWindow::saveSudokuAs() {
  qDebug() << "save as";
  QString tmp;
  tmp = QFileDialog::getSaveFileName(
                  this,
                  tr("Save File"),
                  fullName,
                  tr("Sudoku Files (*.sudoku);; All Files (*)"));

  if (tmp != "") {

    QFile file(tmp);
//    qDebug() << fullName;
    if (!file.open(QIODevice::WriteOnly)) {
      QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Cannot open file"));
      flags["changed"] = true;
      return;
    } else {
      file.write(toFile());
      file.close();
      fullName = tmp;
      QFileInfo info(fullName);
      name = info.baseName();
      this->setWindowTitle(name + " - Sudoku");
      flags["changed"] = false;
      flags["saved"] = true;
    }
  }
}

QByteArray MainWindow::toFile() {
  QJsonObject *mainobj = new QJsonObject();
  QJsonArray marr = QJsonArray();
  for (int i = 0; i < 9; i++) {
    QJsonArray arr = QJsonArray();
    for (int j = 0; j < 9; j++) {
      QJsonObject obj;
      obj.insert("cell", QJsonValue(sudoku->table[i][j]->text().toInt()));
      obj.insert("fixed", QJsonValue(sudoku->table[i][j]->isReadOnly()));

      arr.insert(j, QJsonValue(obj));
    }
    marr.insert(i, QJsonValue(arr));
  }
  mainobj->insert("grid", QJsonValue(marr));
  QJsonDocument save = QJsonDocument(*mainobj);

  return save.toJson();
}

void MainWindow::fromFile(QString str) {
  QJsonDocument jsonResponse = QJsonDocument::fromJson(str.toUtf8());

  QJsonObject jsonObject = jsonResponse.object();

  QJsonArray jsonArray = jsonObject["grid"].toArray();
  int i = 0, j = 0;
  foreach (const QJsonValue & value, jsonArray) {
    QJsonArray arr = value.toArray();
    j = 0;
    foreach (const QJsonValue & a, arr) {
      QJsonObject obj = a.toObject();
      sudoku->grid[i][j] = obj["cell"].toInt();
      sudoku->table[i][j]->setReadOnly(obj["fixed"].toBool());
      j++;
    }
    i++;
  }
}

// events
bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QString tname = obj->objectName();
    int id = tname.mid(0, 2).toInt();
    int n = id / 10;
    int m = id % 10;
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    int k = keyEvent->key();
//    qDebug() << k;
    if (k == Qt::Key_Up || k == Qt::Key_W) {
      sudoku->table[(--n < 0) ? 8 : n][m]->setFocus(Qt::OtherFocusReason);
      if (!sudoku->table[(n < 0) ? 8 : n][m]->isReadOnly())
        sudoku->table[(n < 0) ? 8 : n][m]->selectAll();
      return true;
    } else if (k == Qt::Key_Right || k == Qt::Key_D) {
      sudoku->table[n][(++m > 8) ? 0 : m]->setFocus(Qt::OtherFocusReason);
      if (!sudoku->table[n][(m > 8) ? 0 : m]->isReadOnly())
        sudoku->table[n][(m > 8) ? 0 : m]->selectAll();
      return true;
    } else if (k == Qt::Key_Down || k == Qt::Key_S ||
               k == Qt::Key_Enter || k == Qt::Key_Return) {
      sudoku->table[(++n > 8) ? 0 : n][m]->setFocus(Qt::OtherFocusReason);
      if (!sudoku->table[(n > 8) ? 0 : n][m]->isReadOnly())
        sudoku->table[(n > 8) ? 0 : n][m]->selectAll();
      return true;
    } else if (k == Qt::Key_Left || k == Qt::Key_A) {
      sudoku->table[n][(--m < 0) ? 8 : m]->setFocus(Qt::OtherFocusReason);
      if (!sudoku->table[n][(m < 0) ? 8 : m]->isReadOnly())
        sudoku->table[n][(m < 0) ? 8 : m]->selectAll();
      return true;
    } else if (k == Qt::Key_Plus || keyEvent->key() == Qt::Key_0) {
      sudoku->table[n][m]->setText("");
      return true;
    }
  }
  return false;
  return QMainWindow::eventFilter(obj, event);
}

bool MainWindow::saveEvent() {
  if (flags["changed"]) {
    int ret = QMessageBox::warning(
                  this,
                  tr("Open new file"),
                  tr("The document has been modified.\n"
                     "Do you want to save your changes?"),
                  QMessageBox::Save | QMessageBox::Discard |
                  QMessageBox::Cancel,
                  QMessageBox::Save);

    switch (ret) {
      case QMessageBox::Save: {
        this->saveSudoku();
        return true;
        break;
      }
      case QMessageBox::Discard: {
//        this->close();
        return true;
        break;
      }
      case QMessageBox::Cancel: {
        return false;
        break;
      }
      default: {
        return true;
        break;
      }
    }
  }
  return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {

  settings->settings->setValue("general/geometry", this->saveGeometry());
  settings->settings->setValue("general/state", this->saveState());

  if (flags["changed"]) {
    int ret = QMessageBox::warning(
                  this,
                  tr("Exit"),
                  tr("The document has been modified.\n"
                     "Do you want to save your changes?"),
                  QMessageBox::Save | QMessageBox::Discard |
                  QMessageBox::Cancel,
                  QMessageBox::Save);

    switch (ret) {
      case QMessageBox::Save:
        this->saveSudoku();
        if (flags["changed"])
          event->ignore();
        break;
      case QMessageBox::Discard:
        event->accept();
        break;
      case QMessageBox::Cancel:
        event->ignore();
        break;
      default:
        event->ignore();
        break;
    }
  } else {
    event->accept();
  }
}
