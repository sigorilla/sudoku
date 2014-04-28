#include "settings.h"

#include <QDrag>
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QListWidget>
#include <QStackedWidget>
#include <QGroupBox>

Settings::Settings(QWidget *parent) : QDialog(parent) {
  name = "Settings";
  m_dir = "";
  fileSettings = "settings.ini";
  resize(400, 200);

  settings = new QSettings(fileSettings, QSettings::IniFormat);
  // add QWidget with QGroupBox
  QGroupBox* pathGroup = new QGroupBox("Path to save files");
  QGroupBox* designGroup = new QGroupBox("Design");
  QHBoxLayout* pathLayout = new QHBoxLayout;
  pathLayout->addWidget(new QLabel("Path:"));
  pathEdit = new QLineEdit(settings->value("general/dir").toString());
  QPushButton* browse = new QPushButton("Browse");
  connect(browse, SIGNAL(clicked()), this, SLOT(selectPath()));
  pathLayout->addWidget(pathEdit);
  pathLayout->addWidget(browse);
  pathGroup->setLayout(pathLayout);

  QPushButton *closeButton = new QPushButton(tr("Close"));

  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

  QHBoxLayout *buttonsLayout = new QHBoxLayout;
  buttonsLayout->addStretch(1);
  buttonsLayout->addWidget(closeButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(pathGroup);
  mainLayout->addStretch(1);
  mainLayout->addSpacing(12);
  mainLayout->addLayout(buttonsLayout);
  setLayout(mainLayout);

  setWindowTitle(name);
}

Settings::~Settings() {
}

void Settings::selectPath() {
  QString dir = QFileDialog::getExistingDirectory(
        this,
        "Find directory",
        settings->value("general/dir").toString(),
        QFileDialog::ShowDirsOnly);
  if (!dir.isEmpty()) {
    m_dir = dir;
    pathEdit->setText(dir + "/");
  }
}

void Settings::closeEvent(QCloseEvent *pe) {
  settings->setValue("general/dir", pathEdit->text());
  settings->sync();
  pe->accept();
}
