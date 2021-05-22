#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "raytracing.h"
#include "renderer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
