
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
// #include <QByteArray>
#include <QColorDialog>
#include <QDebug>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <iostream>

#define APPLICATION_NAME "3DViewer"

#ifdef __cplusplus
extern "C" {
#endif
#include "../c_files/s21_view.h"
#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  int preScale{};
  int preNumX{}, preNumY{}, preNumZ{};
  int preRotX{}, preRotY{}, preRotZ{};
  bool status{};

 private slots:
  bool chooseModel_pressed();
  void resetSetting_pressed();

  void chooseBackColor_clicked();
  void chooseVertexColor_clicked();

  void on_numScale_valueChanged(int arg1);

  void on_numX_valueChanged(int arg2);

  void on_numY_valueChanged(int arg3);

  void on_numZ_valueChanged(int arg4);

  void on_numRotX_valueChanged(int arg5);

  void on_numRotY_valueChanged(int arg1);

  void on_numRotZ_valueChanged(int arg1);

  void on_choosePoint_activated(int index);

  void on_chooseLine_activated(int index);

  void on_sladerVertexValue_actionTriggered(int action);

  void on_sladerRibsValue_actionTriggered(int action);

 private:
  Ui::MainWindow *ui;
  int index = 0;
  QSettings settings;
  void file_path();
  void save_settings();
  void load_settings();
};

#endif  // MAINWINDOW_H
