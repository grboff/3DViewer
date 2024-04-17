
#include <locale.h>

#include <QApplication>
#include <QLocale>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  setlocale(LC_ALL, "en_US.UTF-8");

  QCoreApplication::setApplicationName(
      APPLICATION_NAME);  // устанавливаем название приложения в виджете для
                          // сохранения настроек

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
