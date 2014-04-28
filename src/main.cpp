#include <QApplication>

#include "headers/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication::setDesktopSettingsAware(false);
  QApplication a(argc, argv);

  QString errMsg;
  QString finalCss;
  QFile baseCssFile(":/style.qss");
  if (!baseCssFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    errMsg = baseCssFile.errorString();
    qDebug() << errMsg;
  } else {
    finalCss.append(baseCssFile.readAll().data());
  }

  MainWindow *w = new MainWindow();

  a.setStyleSheet(finalCss);
  w->show();

  return a.exec();
}
