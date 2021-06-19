#pragma once
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The MainWindow class has some importants signals and slots that helps
 * the user to manipulate the software. It has a OpenGL widget into its UI that it's one of the most
 * important part of this software. It also creates the scene that will be rendered and send to the renderer.
 *
 * @author Bianca Fragoso
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief The MainWindow default constructor.
     * @param parent The window parent of the main window.
     */
    MainWindow(QWidget *parent = nullptr);


    /**
     * @brief The MainWindow default destructor.
     */
    ~MainWindow();

private slots:

    /**
     * @brief The slot that is called when the spin box responsible
     * for the number of rays is changed. This slot updates the number of rays used by the renderer.
     * @param rayNumber The new ray number.
     */
    void on_raySpinBox_valueChanged(int rayNumber);

    /**
     * @brief The slot that is called when the RayTracing button is clicked.
     * It calls the function that generates a image using the ray tracing technique.
     */
    void on_rayTracingButton_clicked();

    /**
     * @brief The slot that is called when the PathTracing button is clicked.
     * It calls the function that generates a image using the path tracing technique.
    */
    void on_pathTracingButton_clicked();

private:
    /**
     * @brief The UI associated with this window.
     */
    Ui::MainWindow *ui;
};
