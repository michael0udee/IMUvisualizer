#ifndef PROCESSIMUDATA_H
#define PROCESSIMUDATA_H

// Sensor data output interval in milliseconds
#define OUTPUT__DATA_INTERVAL 20  // in milliseconds

// SENSOR CALIBRATION
/*****************************************************************/
// How to calibrate? Read the tutorial at http://dev.qu.tu-berlin.de/projects/sf-razor-9dof-ahrs
// Put MIN/MAX and OFFSET readings for your board here!
// Accelerometer
// "accel x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX"
#define ACCEL_X_MIN ((float) -272)
#define ACCEL_X_MAX ((float) 264)
#define ACCEL_Y_MIN ((float) -265)
#define ACCEL_Y_MAX ((float) 263)
#define ACCEL_Z_MIN ((float) -268)
#define ACCEL_Z_MAX ((float) 254)

// Magnetometer (standard calibration mode)
// "magn x,y,z (min/max) = X_MIN/X_MAX  Y_MIN/Y_MAX  Z_MIN/Z_MAX"
#define MAGN_X_MIN ((float) -143)
#define MAGN_X_MAX ((float) 685)
#define MAGN_Y_MIN ((float) -475)
#define MAGN_Y_MAX ((float) 363)
#define MAGN_Z_MIN ((float) -571)
#define MAGN_Z_MAX ((float) 271)

// Magnetometer (extended calibration mode)
// Uncommend to use extended magnetometer calibration (compensates hard & soft iron errors)
//#define CALIBRATION__MAGN_USE_EXTENDED true
//const float magn_ellipsoid_center[3] = {0, 0, 0};
//const float magn_ellipsoid_transform[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

// Gyroscope
// "gyro x,y,z (current/average) = .../OFFSET_X  .../OFFSET_Y  .../OFFSET_Z
#define GYRO_AVERAGE_OFFSET_X ((float) 28.96)
#define GYRO_AVERAGE_OFFSET_Y ((float) 29.71)
#define GYRO_AVERAGE_OFFSET_Z ((float) -2.07)

// DEBUG OPTIONS
/*****************************************************************/
// When set to true, gyro drift correction will not be applied
#define DEBUG__NO_DRIFT_CORRECTION false
// Print elapsed time after each I/O loop
#define DEBUG__PRINT_LOOP_TIME true

// Sensor calibration scale and offset values
#define ACCEL_X_OFFSET ((ACCEL_X_MIN + ACCEL_X_MAX) / 2.0f)
#define ACCEL_Y_OFFSET ((ACCEL_Y_MIN + ACCEL_Y_MAX) / 2.0f)
#define ACCEL_Z_OFFSET ((ACCEL_Z_MIN + ACCEL_Z_MAX) / 2.0f)
#define ACCEL_X_SCALE (GRAVITY / (ACCEL_X_MAX - ACCEL_X_OFFSET))
#define ACCEL_Y_SCALE (GRAVITY / (ACCEL_Y_MAX - ACCEL_Y_OFFSET))
#define ACCEL_Z_SCALE (GRAVITY / (ACCEL_Z_MAX - ACCEL_Z_OFFSET))

#define MAGN_X_OFFSET ((MAGN_X_MIN + MAGN_X_MAX) / 2.0f)
#define MAGN_Y_OFFSET ((MAGN_Y_MIN + MAGN_Y_MAX) / 2.0f)
#define MAGN_Z_OFFSET ((MAGN_Z_MIN + MAGN_Z_MAX) / 2.0f)
#define MAGN_X_SCALE (100.0f / (MAGN_X_MAX - MAGN_X_OFFSET))
#define MAGN_Y_SCALE (100.0f / (MAGN_Y_MAX - MAGN_Y_OFFSET))
#define MAGN_Z_SCALE (100.0f / (MAGN_Z_MAX - MAGN_Z_OFFSET))

// Gain for gyroscope (ITG-3200)
#define GYRO_GAIN 0.06957 // Same gain on all axes
#define GYRO_SCALED_RAD(x) (x * TO_RAD(GYRO_GAIN)) // Calculate the scaled gyro readings in radians per second

// DCM parameters
#define Kp_ROLLPITCH 0.02f
#define Ki_ROLLPITCH 0.00002f
#define Kp_YAW 1.2f
#define Ki_YAW 0.00002f

// Stuff
#define GRAVITY 256.0f // "1G reference" used for DCM filter and accelerometer calibration
#define TO_RAD(x) (x * 0.01745329252)  // *pi/180
#define TO_DEG(x) (x * 57.2957795131)  // *180/pi

#include <QJsonObject>
#include <QObject>
#include <QDateTime>
#include <iostream>
//#include <ctime>

class processIMUdata : public QObject
{
    Q_OBJECT
public:
    explicit processIMUdata(QObject *parent = nullptr);
    void convertToYPR();
    void compensateSensorErrors();
    void compassHeading();
    void normalize();
    void driftCorrection();
    void matrixUpdate();
    void eulerAngles();
    void readData(QJsonObject imuData);

signals:
    void outputAngles(float yaw, float pitch, float roll);

private:
    float accel[3], accel_min[3], accel_max[3], magnetom[3], magnetom_min[3], magnetom_max[3], magnetom_tmp[3], gyro[3], gyro_average[3];
    int gyro_num_samples = 0;

    // DCM variables
    float MAG_Heading;
    float Accel_Vector[3]= {0, 0, 0}; // Store the acceleration in a vector
    float Gyro_Vector[3]= {0, 0, 0}; // Store the gyros turn rate in a vector
    float Omega_Vector[3]= {0, 0, 0}; // Corrected Gyro_Vector data
    float Omega_P[3]= {0, 0, 0}; // Omega Proportional correction
    float Omega_I[3]= {0, 0, 0}; // Omega Integrator
    float Omega[3]= {0, 0, 0};
    float errorRollPitch[3] = {0, 0, 0};
    float errorYaw[3] = {0, 0, 0};
    float DCM_Matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    float Update_Matrix[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    float Temporary_Matrix[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    // Euler angles
    float yaw, pitch, roll;

    // DCM timing in the main loop
    QDateTime timestamp;
    QDateTime timestamp_old;
    float G_Dt; // Integration time for DCM algorithm

};

#endif // PROCESSIMUDATA_H
