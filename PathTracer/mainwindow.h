#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_raySpinBox_valueChanged(int arg1);

    void on_rayTracingButton_clicked();

    void on_pathTracingButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
