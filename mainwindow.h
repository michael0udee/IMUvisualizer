#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// www.amin-ahmadi.com

#include <QMainWindow>
#include "autonomyprotocol.h"
#include "processimudata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void received(const QJsonObject &object);
    void processedIMUdata(float yaw, float pitch, float roll);

    void on_x_rot_slider_valueChanged(int value);

    void on_y_rot_slider_valueChanged(int value);

    void on_z_rot_slider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
    AutonomyProtocol* protocol_;
    processIMUdata* imu;
};

#endif // MAINWINDOW_H
