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
    float time;
    QImage image = ui->openGLWidget->getRayTracedImage(time);

    ui->label->setPixmap(QPixmap::fromImage(image));
    //ui->label_2->setText("Tempo de renderizazao: " + QString::number(time) + " s");
}
