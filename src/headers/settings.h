#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>

#include "other.h"
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
