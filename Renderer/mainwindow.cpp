#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "raytracing.h"
#include "renderer.h"
#include "scene.h"
#include <fstream>

/**
 * @brief MainWindow
 *
 * The MainWindow class implements the main window of the software.
 *
 * @author Bianca Fragoso
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    Scene scene;
    scene.createDefaultScene();
    _ui->openGLWidget->setScene(scene);

}



MainWindow::~MainWindow()
{
    delete _ui;
}



void MainWindow::on_raySpinBox_valueChanged(int rayNumber)
{
    _ui->openGLWidget->setNumberOfRays(rayNumber);
}



void MainWindow::on_rayTracingButton_clicked()
{
    float time;
    QImage image = _ui->openGLWidget->getRayTracedImage(time);

    _ui->label->setPixmap(QPixmap::fromImage(image));
    _ui->timeLabel->setText(QString::number(time) + " s");
}



void MainWindow::on_pathTracingButton_clicked()
{
    float time;
    QImage image = _ui->openGLWidget->getPathTracedImage(time);

    _ui->label->setPixmap(QPixmap::fromImage(image));
    _ui->timeLabel->setText(QString::number(time) + " s");
}



void MainWindow::makeTest()
{
    std::ofstream myfile;
    myfile.open ("teste.log");

    Scene scene;
    float time;
    int width = _ui->openGLWidget->width();
    int height = _ui->openGLWidget->height();

    //Print informations
    myfile << "############################################## TESTE ################################################" << std::endl;
    myfile << std::endl;

    myfile << "######################################## Cena Vazia so com Background ###########################################################" << std::endl;
    scene.createEmptyScene();
    _ui->openGLWidget->setScene(scene);
    _ui->openGLWidget->getRayTracedImage(time);
    std::vector<unsigned int> count = _ui->openGLWidget->getCountVector();

    myfile << "Total of Rays: " << width *height << std::endl;
    myfile << "Background: " << count[0] << std::endl;
    myfile << "Sphere: " << count[1] << std::endl;
    myfile << "Plane: " << count[2] << std::endl;
    myfile << "Triangle Mesh: " << count[3] << std::endl;

    myfile << "###################################################################################################" << std::endl;
    myfile << std::endl;

    myfile << "######################################## Cena mostrando Background, Esferas e um Plano ###########################################################" << std::endl;

    scene.createDefaultScene();
    _ui->openGLWidget->setScene(scene);
    _ui->openGLWidget->getRayTracedImage(time);
    count = _ui->openGLWidget->getCountVector();
    myfile << "Total of Rays: " << width *height << std::endl;
    myfile << "Background: " << count[0] << std::endl;
    myfile << "Sphere: " << count[1] << std::endl;
    myfile << "Plane: " << count[2] << std::endl;
    myfile << "Triangle Mesh: " << count[3] << std::endl;

    myfile << "###################################################################################################" << std::endl;
    myfile << std::endl;

    myfile << "######################################## Cena sem Background apenas com um Plano e Esferas ###########################################################" << std::endl;

    _ui->openGLWidget->setEye(QVector3D(0,0,8));
    _ui->openGLWidget->getRayTracedImage(time);

    count = _ui->openGLWidget->getCountVector();
    myfile << "Total of Rays: " << width *height << std::endl;
    myfile << "Background: " << count[0] << std::endl;
    myfile << "Sphere: " << count[1] << std::endl;
    myfile << "Plane: " << count[2] << std::endl;
    myfile << "Triangle Mesh: " << count[3] << std::endl;

    myfile << "###################################################################################################" << std::endl;
    myfile << std::endl;
    _ui->openGLWidget->setEye(QVector3D(0,0,10));

    myfile << "######################################## Cena apenas com Plano ###########################################################" << std::endl;

    scene.createOnlyPlane();
    _ui->openGLWidget->setScene(scene);
    _ui->openGLWidget->getRayTracedImage(time);

    count = _ui->openGLWidget->getCountVector();
    myfile << "Total of Rays: " << width *height << std::endl;
    myfile << "Background: " << count[0] << std::endl;
    myfile << "Sphere: " << count[1] << std::endl;
    myfile << "Plane: " << count[2] << std::endl;
    myfile << "Triangle Mesh: " << count[3] << std::endl;

    myfile << "###################################################################################################" << std::endl;
    myfile << std::endl;

    myfile << "######################################## Cena com Background, Malha de triângulos, Esferas e Plano ###########################################################" << std::endl;
    scene.createTriangleMeshAndPlaneAndSpheres();
    _ui->openGLWidget->setScene(scene);
    _ui->openGLWidget->getRayTracedImage(time);

    count = _ui->openGLWidget->getCountVector();
    myfile << "Total of Rays: " << width * height << std::endl;
    myfile << "Background: " << count[0] << std::endl;
    myfile << "Sphere: " << count[1] << std::endl;
    myfile << "Plane: " << count[2] << std::endl;
    myfile << "Triangle Mesh: " << count[3] << std::endl;

    myfile << "###################################################################################################" << std::endl;
    myfile << std::endl;


    myfile.close();

    scene.createDefaultScene();
    _ui->openGLWidget->setScene(scene);
    on_rayTracingButton_clicked();

}



void MainWindow::on_Scene1RadioButton_clicked()
{
    Scene scene;
    scene.createDefaultScene();
    _ui->openGLWidget->setScene(scene);
}



void MainWindow::on_Scene2RadioButton_clicked()
{
    Scene scene;
    scene.createDefaultScene2();
    _ui->openGLWidget->setScene(scene);
}



void MainWindow::on_Scene3RadioButton_clicked()
{
    Scene scene;
    scene.createDefaultScene3();
    _ui->openGLWidget->setScene(scene);
}



void MainWindow::on_TestePushButton_clicked()
{
    makeTest();
}
