#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QList>
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
#include <QListWidget>
#include <QStackedWidget>
#include <QSettings>

#include "sudoku.h"

class Settings : public QDialog {
  Q_OBJECT

 public:
  Settings(QWidget *parent = 0);
  ~Settings();
  QString name;
  QLineEdit* pathEdit;
  QSettings* settings;

 public slots:
  void selectPath();

 private:
  void closeEvent(QCloseEvent* pe);
  QString fileSettings;

 protected:
  QString m_dir;

};

#endif // SETTINGS_H
