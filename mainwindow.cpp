#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QJsonArray>
#include <QJsonObject>
// www.amin-ahmadi.com

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    protocol_ = new AutonomyProtocol(this);
    connect(protocol_, &AutonomyProtocol::received, this, &MainWindow::received);
    protocol_->joinGroup(QHostAddress("225.2.0.3"));

    imu = new processIMUdata();
    connect(imu, &processIMUdata::outputAngles, this, &MainWindow::processedIMUdata);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::received(const QJsonObject &object)
{
    if (object.value("sender_id") == "rover soft" && object.value("msg_id") == "imu")
        imu->readData(object.value("data").toObject());
}

void MainWindow::processedIMUdata(float yaw, float pitch, float roll)
{
    //if(yaw > 0)
    ui->openGLWidget->setRotation(roll+90, pitch, yaw);
}

void MainWindow::on_x_rot_slider_valueChanged(int value)
{
    Q_UNUSED(value);
    ui->openGLWidget->setRotation(
                ui->x_rot_slider->value(),
                ui->y_rot_slider->value(),
                ui->z_rot_slider->value()
                );
}

void MainWindow::on_y_rot_slider_valueChanged(int value)
{
    Q_UNUSED(value);
    ui->openGLWidget->setRotation(
                ui->x_rot_slider->value(),
                ui->y_rot_slider->value(),
                ui->z_rot_slider->value()
                );
}

void MainWindow::on_z_rot_slider_valueChanged(int value)
{
    Q_UNUSED(value);
    ui->openGLWidget->setRotation(
                ui->x_rot_slider->value(),
                ui->y_rot_slider->value(),
                ui->z_rot_slider->value()
                );
}
