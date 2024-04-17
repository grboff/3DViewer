#include "myopenglwidget.h"

#include "qevent.h"

MyOpenGLWidget::MyOpenGLWidget(QWidget* parent)
    : QOpenGLWidget{parent}, scale(1) {
  setMouseTracking(true);  // отслеживание событий мыши, если false - не
                           // отслеживает события мыши, когда кнопки не нажаты
}

void MyOpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);   // включение буфера глубины
  glEnable(GL_LINE_SMOOTH);  // сглаживание линий
  glEnable(
      GL_BLEND);  // режим смешивания цветов при выводе прозрачных полигонов
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_PROJECTION);  // устанавливает текущей проекционную матрицу
  glLoadIdentity();  // присваивает проекционной матрице единичную матрицу

  type_projection();
}

void MyOpenGLWidget::resizeGL(int w, int h) {
  glViewport(
      0, 0, w,
      h);  // область окна, в которой будет отображаться результаты нашей работы
}

void MyOpenGLWidget::paintGL() {
  glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
               backColor.alphaF());
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);  // очистка буфера изображения и глубины
  glMatrixMode(GL_MODELVIEW);  // устанавливает положение и ориентацию матрице
                               // моделирования
  glLoadIdentity();  // загружает единичную матрицу моделирования

  glTranslatef(xTrans, yTrans, zTrans);  // двигаем систему координат

  glScalef(scale, scale, scale);  // размер фигуры

  glRotatef(xRot, 1, 0, 0);  // вращение фигуры
  glRotatef(yRot, 0, 1, 0);
  glRotatef(zRot, 0, 0, 1);

  back_color();
  type_projection();

  drawModel();  // отрисовка
}

void MyOpenGLWidget::set() { data_ = 1; }

void MyOpenGLWidget::create_data() {
  data = (data_t*)malloc(sizeof(data_t));
  if (data != NULL) {
    res = pars_object(fileName, data);  // проверить парсинг
    if (res != 0)
      QMessageBox::information(this, "Attention!", "Problem: pars_object");
  } else {
    QMessageBox::information(this, "Attention!", "Problem: init_data_for_qt");
  }
  set_center();
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* mo) { mPos = mo->pos(); }

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent* mo) {
  if (mo->buttons() & Qt::RightButton) {
    GLfloat dx = static_cast<GLfloat>(mo->pos().x() - mPos.x());
    GLfloat dy = static_cast<GLfloat>(mo->pos().y() - mPos.y());

    xTrans += dx / 15;
    yTrans -= dy / 15;
    mPos = mo->pos();
    update();
  } else if (mo->buttons() & Qt::LeftButton) {
    xRot = 1 / M_PI * (mo->pos().y() - mPos.y());
    yRot = 1 / M_PI * (mo->pos().x() - mPos.x());
    update();
  }
}

void MyOpenGLWidget::wheelEvent(QWheelEvent* event) {
  int delta = event->angleDelta().y();

  if (delta > 0)
    scale *= 1.1;
  else if (delta < 0)
    scale /= 1.1;

  update();
}

void MyOpenGLWidget::drawModel() {
  if (data_ && res == 0) {
    //    update();

    // Тип линий
    if (typeLine == 0 || typeLine == 1) {
      glDisable(GL_LINE_STIPPLE);
    } else {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(10,
                    0x1111);  // работает пунктир, сделать условие для кнопочек
    }
    glLineWidth(sizeR);  // вот тут слайдер прикрутить

    glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
                 backColor.alphaF());
    glColor3d(VertexColor.redF(), VertexColor.greenF(), VertexColor.blueF());

    // Тип точек
    glPointSize(sizeV);  // вот тут слайдер прикрутить
    if (typePoint == 2) glEnable(GL_POINT_SMOOTH);
    if (typePoint == 3) glEnable(GL_POINT);

    // загружаем актуальные значения вертексов в буфер
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, data->vertices);
    glDrawElements(GL_LINES, data->ribs_count, GL_UNSIGNED_INT, data->ribs);
    if (typePoint != 0 && typePoint != 1) {
      glDrawArrays(GL_POINTS, 0, data->vertex_count / 3);
    }

    // Выкл
    glDisableClientState(GL_VERTEX_ARRAY);
    if (typePoint == 2) glDisable(GL_POINT_SMOOTH);
    if (typePoint == 3) glDisable(GL_POINT);
  }
}

float MyOpenGLWidget::normaliza_slader_value(int sliderValue) {
  float normalizaValue = static_cast<float>(sliderValue + 10) /
                         30.0f;  // функция нормализации к -1/1
  scale = 0.5 + normalizaValue * 1.5;  // диапазон значений от 0.5 до 1.5
  update();  // запрос на обновление отрисовки
  return scale;
}

void MyOpenGLWidget::set_scale(int delta) {
  float arg = static_cast<float>(delta);
  cscale(data, arg);
}

void MyOpenGLWidget::set_trans_x(int delta) {
  int axis = 0;
  move_by_axis(data, delta, axis);
}

void MyOpenGLWidget::set_trans_y(int delta) {
  int axis = 1;
  move_by_axis(data, delta, axis);
}

void MyOpenGLWidget::set_trans_z(int delta) {
  int axis = 2;
  move_by_axis(data, delta, axis);
}

void MyOpenGLWidget::set_rot_x(int delta) {
  double angle = static_cast<double>(delta);
  rotate_by_x(data, angle);
}

void MyOpenGLWidget::set_rot_y(int delta) {
  double angle = static_cast<double>(delta);
  rotate_by_y(data, angle);
}

void MyOpenGLWidget::set_rot_z(int delta) {
  double angle = static_cast<double>(delta);
  rotate_by_z(data, angle);
}

void MyOpenGLWidget::back_color() {  // Цвет фона
  if (backColor != preColor) {
    glClearColor(backColor.redF(), backColor.greenF(), backColor.blueF(),
                 backColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT);
    preColor = backColor;  // Сохраняем текущее значение фона
    update();
  }
}

void MyOpenGLWidget::type_projection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  GLfloat zoom = static_cast<GLfloat>(this->width()) /
                 static_cast<GLfloat>(this->height());

  if (typeProjection == 0 || typeProjection == 1) {
    GLfloat front = 0.009;  // ближний план
    GLfloat back = 1000.0;  //  задний план
    GLfloat fov = 60.0;     // field of view, поле обзора

    // границы проекции на оси X и Y с учетом zoom
    GLfloat updown = front * tanf(fov * M_PI / 360.0);
    GLfloat side = updown * zoom;

    glFrustum(-side, side, -updown, updown, front,
              back);  // parallel. Параметры: левая, правая, нижняя, верхняя,
                      // ближняя, дальняя плоскости отсечения.
  } else if (typeProjection == 0 || typeProjection == 2) {
    float modelCenterX = (data->min_val.x + data->max_val.x) / 2;
    float modelCenterY = (data->min_val.y + data->max_val.y) / 2;

    // Находим максимальный размер модели
    float modelWidth = data->max_val.x - data->min_val.x;
    float modelHeight = data->max_val.y - data->min_val.y;
    float modelDepth = data->max_val.z - data->min_val.z;
    float maxModelSize =
        std::max(modelWidth, std::max(modelHeight, modelDepth));

    // Вычисляем расстояние по Z, чтобы вся модель помещалась
    float zDistance = 3 * maxModelSize;

    // Устанавливаем область видимости чуть больше модели
    float visibleWidth = 1.1 * maxModelSize;
    float visibleHeight = 1.1 * maxModelSize;

    glOrtho((modelCenterX - visibleWidth) * zoom,
            (modelCenterX + visibleWidth) * zoom, modelCenterY - visibleHeight,
            modelCenterY + visibleHeight, -zDistance, zDistance);
  }
}

void MyOpenGLWidget::set_center() {
  scale = 1.0f;  // обнулить фокус
  xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;

  float sizeW =
      (data->max_val.x - data->min_val.x) * 1.2;  // уместить модель по X и Y
  float sizeH = (data->max_val.y - data->min_val.y) * 1.2;
  float focusZ = sizeW + sizeH;  // сместить фокус по Z,

  xTrans = 0, yTrans = 0;  // координаты смещения фокуса
  zTrans = -focusZ;

  type_projection();  // обновить отрисовку
  update();
}
