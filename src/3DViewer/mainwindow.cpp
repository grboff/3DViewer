
#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), settings(APPLICATION_NAME) {
  ui->setupUi(this);
  this->setWindowTitle(APPLICATION_NAME);

  // load_setting(); // загрузить сохраненную модель
  chooseModel_pressed();

  // обработка пользовательских настроек (слайдеры, кноки, боксы)

  connect(ui->numX, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](float value) {
            ui->widget->xTrans = value / 4,
            ui->sladerTransX->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerTransX, &QSlider::valueChanged, ui->numX,
          &QSpinBox::setValue);

  connect(ui->numSizeV, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](float value) {
            ui->widget->sizeV = value,
            ui->sladerVertexValue->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerVertexValue, &QSlider::valueChanged, ui->numSizeV,
          &QSpinBox::setValue);

  connect(ui->numSizeR, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](float value) {
            ui->widget->sizeR = value,
            ui->sladerRibsValue->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerRibsValue, &QSlider::valueChanged, ui->numSizeR,
          &QSpinBox::setValue);

  connect(ui->numY, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](int value) {
            ui->widget->yTrans = value,
            ui->sladerTransY->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerTransY, &QSlider::valueChanged, ui->numY,
          &QSpinBox::setValue);

  connect(ui->numZ, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](int value) {
            ui->widget->normaliza_slader_value(value),
                ui->sladerTransZ->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerTransZ, &QSlider::valueChanged, ui->numZ,
          &QSpinBox::setValue);
  connect(ui->numZ, SIGNAL(valueChanged(int)), this,
          SLOT(on_numZ_valueChanged(int)));

  connect(ui->numRotX, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](int value) {
            ui->widget->xRot = value,
            ui->sladerRotatX->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerRotatX, &QSlider::valueChanged, ui->numRotX,
          &QSpinBox::setValue);

  //  connect(ui->numRotY, qOverload<int>(&QSpinBox::valueChanged), this,
  //          [=](int value) {
  //            ui->widget->yRot = value,
  //            ui->sladerRotatY->setValue(value) /*, save_settings()*/;
  //          });
  //  connect(ui->sladerRotatY, &QSlider::valueChanged, ui->numRotY,
  //          &QSpinBox::setValue);

  connect(ui->numRotZ, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](int value) {
            ui->widget->zRot = value,
            ui->sladerRotatZ->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerRotatZ, &QSlider::valueChanged, ui->numRotZ,
          &QSpinBox::setValue);

  connect(ui->numScale, qOverload<int>(&QSpinBox::valueChanged), this,
          [=](int value) {
            ui->widget->normaliza_slader_value(value),
                ui->sladerScale->setValue(value) /*, save_settings()*/;
          });
  connect(ui->sladerScale, &QSlider::valueChanged, ui->numScale,
          &QSpinBox::setValue);

  connect(ui->chooseProjection,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          [=](int index) {
            ui->widget->typeProjection = index /*, save_settings()*/;
          });

  connect(
      ui->chooseLine, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [=](int index) { ui->widget->typeLine = index /*, save_settings()*/; });

  connect(ui->choosePoint, QOverload<int>::of(&QComboBox::currentIndexChanged),
          this, [=](int index) {
            ui->widget->typePoint = index /*, save_settings()*/;
          });

  connect(ui->resetSetting, SIGNAL(clicked()), this,
          SLOT(resetSetting_pressed()));

  connect(ui->chooseBackColor, SIGNAL(clicked()), this,
          SLOT(chooseBackColor_clicked()));

  ui->widget->backColor =
      settings.value("backColor", QColor(Qt::black)).value<QColor>();

  connect(ui->resetSetting, SIGNAL(clicked()), this,
          SLOT(resetSetting_pressed()));
  ui->widget->update();

  connect(ui->chooseVertexColor, SIGNAL(clicked()), this,
          SLOT(chooseVertexColor_clicked()));
}

MainWindow::~MainWindow() {
  //   save_settings();
  free(ui->widget->data);
  delete ui;
}

bool MainWindow::chooseModel_pressed() {
  connect(ui->chooseModel, &QPushButton::clicked, this, &MainWindow::file_path);
  return true;
}

void MainWindow::file_path() {
  QDir::currentPath();  // возвращает абсолютный путь к текущему каталогу
                        // приложения.
  QString path = QDir::currentPath() + "/../obj_files/";
  QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), path,
                                                  tr("objects (*.obj)"));

  if (filePath.isEmpty()) {
    status = false;
    QMessageBox::information(this, "Attention!", "Please, path is empty");
  } else {
    free(ui->widget->data);
    qDebug() << filePath;
    status = true;

    QByteArray tmp = filePath.toLocal8Bit();

    ui->labelFileName_2->setText(filePath.section('/', -1, -1));
    ui->widget->fileName = tmp.data();
    ui->widget->create_data();
    ui->widget->set();
    ui->widget->initializeGL();

    ui->labelVertRes->setText(
        QString::number(ui->widget->data->vertex_count / 3));
    ui->labelEdgRes->setText(QString::number(ui->widget->data->ribs_count / 4));
    ui->widget->update();
  }
}

void MainWindow::resetSetting_pressed() {
  ui->numX->setValue(0);
  ui->numY->setValue(0);
  ui->numZ->setValue(-5);

  ui->numRotX->setValue(0);
  ui->numRotY->setValue(0);
  ui->numRotZ->setValue(0);

  ui->sladerScale->setValue(0);
  ui->numScale->setValue(0);

  ui->numSizeR->setValue(1);
  ui->numSizeV->setValue(1);

  ui->chooseLine->setCurrentIndex(0);
  ui->choosePoint->setCurrentIndex(0);

  ui->widget->backColor = QColor(Qt::black);
  ui->widget->VertexColor = QColor(Qt::white);

  ui->widget->update();
}

void MainWindow::save_settings() {
  settings.setValue("backColor", ui->widget->backColor);
  settings.setValue("typeProjection", ui->widget->typeProjection);
  settings.setValue("SizeR", ui->widget->sizeR);
  settings.setValue("SizeV", ui->widget->sizeV);
  settings.setValue("typePoint", ui->widget->typePoint);
  settings.setValue("typeLine", ui->widget->typeLine);
  ui->widget->update();
}

void MainWindow::chooseBackColor_clicked() {
  QColor color =
      QColorDialog::getColor(Qt::black, this, "Select a background color");

  if (color.isValid()) {
    ui->widget->backColor = color;  // Устанавливаем значение в виджете
    ui->widget->update();
    update();
    save_settings();
  }
}

void MainWindow::chooseVertexColor_clicked() {
  QColor color =
      QColorDialog::getColor(Qt::white, this, "Select a background color");

  if (color.isValid()) {
    ui->widget->VertexColor = color;
    ui->widget->update();
    update();
    save_settings();
  }
}

void MainWindow::on_numScale_valueChanged(int arg1) {
  if (status) {
    int delta = preScale - arg1;
    ui->widget->set_scale(delta);
    preScale = arg1;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg1, choose a model");
  }
}

void MainWindow::on_numX_valueChanged(int arg2) {
  if (status) {
    int delta = preNumX - arg2;
    ui->widget->set_trans_x(delta);
    preNumX = arg2;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg2, choose a model");
  }
}

void MainWindow::on_numY_valueChanged(int arg3) {
  if (status) {
    int delta = preNumY - arg3;
    ui->widget->set_trans_y(delta);
    preNumY = arg3;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg3, choose a model");
  }
}

void MainWindow::on_numZ_valueChanged(int arg4) {
  if (status) {
    int delta = preNumZ - arg4;
    ui->widget->set_trans_z(delta);
    preNumZ = arg4;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg4, choose a model");
  }
}

void MainWindow::on_numRotX_valueChanged(int arg5) {
  if (status) {
    int delta = preRotX - arg5;
    ui->widget->set_rot_x(delta);
    preRotX = arg5;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg5, choose a model");
  }
}

void MainWindow::on_numRotY_valueChanged(int arg6) {
  if (status) {
    int delta = preRotY - arg6;
    ui->widget->set_rot_y(delta);
    preRotY = arg6;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg6, choose a model");
  }
}

void MainWindow::on_numRotZ_valueChanged(int arg7) {
  if (status) {
    int delta = preRotZ - arg7;
    ui->widget->set_rot_z(delta);
    preRotZ = arg7;
    ui->widget->update();
  } else {
    QMessageBox::information(this, "Attention!", "Arg7, choose a model");
  }
}

void MainWindow::on_choosePoint_activated(int index) {
  if (index >= 0) ui->widget->update();
}

void MainWindow::on_chooseLine_activated(int index) {
  if (index >= 0) ui->widget->update();
}

void MainWindow::on_sladerVertexValue_actionTriggered(int action) {
  if (action >= 0) ui->widget->update();
}

void MainWindow::on_sladerRibsValue_actionTriggered(int action) {
  if (action >= 0) ui->widget->update();
}
