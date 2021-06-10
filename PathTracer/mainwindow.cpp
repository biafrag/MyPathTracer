#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "raytracing.h"
#include "renderer.h"
#include "scene.h"

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    activeCPU(true);

    Scene scene;
    ui->openGLWidget->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_raySpinBox_valueChanged(int arg1)
{
    ui->openGLWidget->setNumberOfRays(arg1);
}

void MainWindow::on_rayTracingButton_clicked()
{
    float time;
    QImage image = ui->openGLWidget->getRayTracedImage(time);

    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->timeLabel->setText(QString::number(time) + " s");
}

void MainWindow::on_pathTracingButton_clicked()
{
    float time;
    QImage image = ui->openGLWidget->getPathTracedImage(time);

    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->timeLabel->setText(QString::number(time) + " s");
}



void MainWindow::activeCPU(bool active)
{
    ui->label->setVisible(active);
    ui->raySpinBox->setVisible(active);
    ui->rayTracingButton->setVisible(active);
    ui->pathTracingButton->setVisible(active);
    ui->timeFixedLabel->setVisible(active);
    ui->numberOfRaysLabel->setVisible(active);
    ui->timeLabel->setVisible(active);
}
