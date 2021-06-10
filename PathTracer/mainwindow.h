#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief MainWindow
     * @param parent
     */
    MainWindow(QWidget *parent = nullptr);


    ~MainWindow();

private slots:

    /**
     * @brief on_raySpinBox_valueChanged
     * @param arg1
     */
    void on_raySpinBox_valueChanged(int arg1);

    /**
     * @brief on_rayTracingButton_clicked
     */
    void on_rayTracingButton_clicked();

    /**
     * @brief on_pathTracingButton_clicked
     */
    void on_pathTracingButton_clicked();

    /**
     * @brief activeCPU
     * @param active
     */
    void activeCPU(bool active);

private:
    Ui::MainWindow *ui;
};
