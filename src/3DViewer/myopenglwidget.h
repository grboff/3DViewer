#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <mainwindow.h>
#include <qevent.h>

#include <QColor>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
// #include <QTime>
// #include <QTimer>
#include <QWheelEvent>
#include <QWidget>
#include <QtOpenGL>
#include <cmath>
#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
#include "../c_files/s21_view.h"
#ifdef __cplusplus
}
#endif

#define RGB_MIN 1
#define RGB_MAX 255

class MyOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions {
  //    Q_OBJECT

 public:
  MyOpenGLWidget(QWidget *parent = nullptr);
  char *fileName = NULL;
  data_t *data = {};

  GLfloat xTrans{}, yTrans{}, zTrans{-5};  // перемещение модели по осям X, Y, Z
  GLfloat xRot{}, yRot{}, zRot{};  // вращение модели по осям  X, Y, Z
  GLint sizeV{};
  GLint sizeR{};
  float scale{};
  int typeProjection{};
  int typeLine{};
  int typePoint{};
  int res{};
  QColor backColor;
  QColor VertexColor;
  QColor preColor;
  //  QColor

  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  void set();
  void create_data();
  void set_center();
  float normaliza_slader_value(int sliderValue);
  void back_color();
  void type_projection();
  void set_scale(int delta);
  void set_trans_x(int delta);
  void set_trans_y(int delta);
  void set_trans_z(int delta);
  void set_rot_x(int delta);
  void set_rot_y(int delta);
  void set_rot_z(int delta);

 private:
  //    float z;
  QPoint mPos;
  //  QTimer timer;
  int data_{};

  void drawModel();

  void mousePressEvent(QMouseEvent *) override;  // вызов при нажатии мыши
  void mouseMoveEvent(QMouseEvent *) override;  // вызоа при движении мыши
  void wheelEvent(
      QWheelEvent *event) override;  // вызов при работе колесика мыши
  // public slots:
  //     void changeZ();
};

#endif  // MYOPENGLWIDGET_H
