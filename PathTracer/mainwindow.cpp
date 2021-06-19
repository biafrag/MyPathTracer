#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "raytracing.h"
#include "renderer.h"
#include "scene.h"

/**
 * @brief MainWindow
 *
 * The MainWindow class implements the main window of the software.
 *
 * @author Bianca Fragoso
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Scene scene;
    ui->openGLWidget->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_raySpinBox_valueChanged(int rayNumber)
{
    ui->openGLWidget->setNumberOfRays(rayNumber);
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
