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


void MainWindow::on_pushButton_clicked()
{
    QImage image = ui->openGLWidget->getRayTracedImage();

    ui->label->setPixmap(QPixmap::fromImage(image));
}
