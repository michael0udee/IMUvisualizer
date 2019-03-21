#include "processimudata.h"
#include "arrayoperations.h"

processIMUdata::processIMUdata(QObject *parent) : QObject(parent)
{
    timestamp = QDateTime::currentDateTime();
}

void processIMUdata::convertToYPR()
{

}

void processIMUdata::compensateSensorErrors()
{
    // Compensate accelerometer error
    accel[0] = (accel[0] - ACCEL_X_OFFSET) * ACCEL_X_SCALE;
    accel[1] = (accel[1] - ACCEL_Y_OFFSET) * ACCEL_Y_SCALE;
    accel[2] = (accel[2] - ACCEL_Z_OFFSET) * ACCEL_Z_SCALE;

    // Compensate magnetometer error
#if CALIBRATION__MAGN_USE_EXTENDED == true
    for (int i = 0; i < 3; i++)
      magnetom_tmp[i] = magnetom[i] - magn_ellipsoid_center[i];
    Matrix_Vector_Multiply(magn_ellipsoid_transform, magnetom_tmp, magnetom);
#else
    magnetom[0] = (magnetom[0] - MAGN_X_OFFSET) * MAGN_X_SCALE;
    magnetom[1] = (magnetom[1] - MAGN_Y_OFFSET) * MAGN_Y_SCALE;
    magnetom[2] = (magnetom[2] - MAGN_Z_OFFSET) * MAGN_Z_SCALE;
#endif

    // Compensate gyroscope error
    gyro[0] -= GYRO_AVERAGE_OFFSET_X;
    gyro[1] -= GYRO_AVERAGE_OFFSET_Y;
    gyro[2] -= GYRO_AVERAGE_OFFSET_Z;
}

void processIMUdata::compassHeading()
{
    float mag_x;
    float mag_y;
    float cos_roll;
    float sin_roll;
    float cos_pitch;
    float sin_pitch;

    cos_roll = cos(roll);
    sin_roll = sin(roll);
    cos_pitch = cos(pitch);
    sin_pitch = sin(pitch);

    // Tilt compensated magnetic field X
    mag_x = magnetom[0] * cos_pitch + magnetom[1] * sin_roll * sin_pitch + magnetom[2] * cos_roll * sin_pitch;
    // Tilt compensated magnetic field Y
    mag_y = magnetom[1] * cos_roll - magnetom[2] * sin_roll;
    // Magnetic Heading
    MAG_Heading = atan2(-mag_y, mag_x);
}

void processIMUdata::normalize()
{
    float error=0;
    float temporary[3][3];
    float renorm=0;

    error= -Vector_Dot_Product(&DCM_Matrix[0][0],&DCM_Matrix[1][0])*.5; //eq.19

    Vector_Scale(&temporary[0][0], &DCM_Matrix[1][0], error); //eq.19
    Vector_Scale(&temporary[1][0], &DCM_Matrix[0][0], error); //eq.19

    Vector_Add(&temporary[0][0], &temporary[0][0], &DCM_Matrix[0][0]);//eq.19
    Vector_Add(&temporary[1][0], &temporary[1][0], &DCM_Matrix[1][0]);//eq.19

    Vector_Cross_Product(&temporary[2][0],&temporary[0][0],&temporary[1][0]); // c= a x b //eq.20

    renorm = .5 *(3 - Vector_Dot_Product(&temporary[0][0],&temporary[0][0])); //eq.21
    Vector_Scale(&DCM_Matrix[0][0], &temporary[0][0], renorm);

    renorm = .5 *(3 - Vector_Dot_Product(&temporary[1][0],&temporary[1][0])); //eq.21
    Vector_Scale(&DCM_Matrix[1][0], &temporary[1][0], renorm);

    renorm = .5 *(3 - Vector_Dot_Product(&temporary[2][0],&temporary[2][0])); //eq.21
    Vector_Scale(&DCM_Matrix[2][0], &temporary[2][0], renorm);
}

void processIMUdata::driftCorrection()
{
    float mag_heading_x;
    float mag_heading_y;
    float errorCourse;
    //Compensation the Roll, Pitch and Yaw drift.
    static float Scaled_Omega_P[3];
    static float Scaled_Omega_I[3];
    float Accel_magnitude;
    float Accel_weight;

    //*****Roll and Pitch***************

    // Calculate the magnitude of the accelerometer vector
    Accel_magnitude = sqrt(Accel_Vector[0]*Accel_Vector[0] + Accel_Vector[1]*Accel_Vector[1] + Accel_Vector[2]*Accel_Vector[2]);
    Accel_magnitude = Accel_magnitude / GRAVITY; // Scale to gravity.
    // Dynamic weighting of accelerometer info (reliability filter)
    // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
    Accel_weight = constrain(1 - 2*abs(1 - Accel_magnitude),0,1);  //

    Vector_Cross_Product(&errorRollPitch[0],&Accel_Vector[0],&DCM_Matrix[2][0]); //adjust the ground of reference
    Vector_Scale(&Omega_P[0],&errorRollPitch[0],Kp_ROLLPITCH*Accel_weight);

    Vector_Scale(&Scaled_Omega_I[0],&errorRollPitch[0],Ki_ROLLPITCH*Accel_weight);
    Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);

    //*****YAW***************
    // We make the gyro YAW drift correction based on compass magnetic heading

    mag_heading_x = cos(MAG_Heading);
    mag_heading_y = sin(MAG_Heading);
    errorCourse=(DCM_Matrix[0][0]*mag_heading_y) - (DCM_Matrix[1][0]*mag_heading_x);  //Calculating YAW error
    Vector_Scale(errorYaw,&DCM_Matrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.

    Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);//.01proportional of YAW.
    Vector_Add(Omega_P,Omega_P,Scaled_Omega_P);//Adding  Proportional.

    Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);//.00001Integrator
    Vector_Add(Omega_I,Omega_I,Scaled_Omega_I);//adding integrator to the Omega_I
}

void processIMUdata::matrixUpdate()
{
    Gyro_Vector[0]=GYRO_SCALED_RAD(gyro[0]); //gyro x roll
    Gyro_Vector[1]=GYRO_SCALED_RAD(gyro[1]); //gyro y pitch
    Gyro_Vector[2]=GYRO_SCALED_RAD(gyro[2]); //gyro z yaw

    Accel_Vector[0]=accel[0];
    Accel_Vector[1]=accel[1];
    Accel_Vector[2]=accel[2];

    Vector_Add(&Omega[0], &Gyro_Vector[0], &Omega_I[0]);  //adding proportional term
    Vector_Add(&Omega_Vector[0], &Omega[0], &Omega_P[0]); //adding Integrator term

  #if DEBUG__NO_DRIFT_CORRECTION == true // Do not use drift correction
    Update_Matrix[0][0]=0;
    Update_Matrix[0][1]=-G_Dt*Gyro_Vector[2];//-z
    Update_Matrix[0][2]=G_Dt*Gyro_Vector[1];//y
    Update_Matrix[1][0]=G_Dt*Gyro_Vector[2];//z
    Update_Matrix[1][1]=0;
    Update_Matrix[1][2]=-G_Dt*Gyro_Vector[0];
    Update_Matrix[2][0]=-G_Dt*Gyro_Vector[1];
    Update_Matrix[2][1]=G_Dt*Gyro_Vector[0];
    Update_Matrix[2][2]=0;
  #else // Use drift correction
    Update_Matrix[0][0]=0;
    Update_Matrix[0][1]=-G_Dt*Omega_Vector[2];//-z
    Update_Matrix[0][2]=G_Dt*Omega_Vector[1];//y
    Update_Matrix[1][0]=G_Dt*Omega_Vector[2];//z
    Update_Matrix[1][1]=0;
    Update_Matrix[1][2]=-G_Dt*Omega_Vector[0];//-x
    Update_Matrix[2][0]=-G_Dt*Omega_Vector[1];//-y
    Update_Matrix[2][1]=G_Dt*Omega_Vector[0];//x
    Update_Matrix[2][2]=0;
  #endif

    Matrix_Multiply(DCM_Matrix, Update_Matrix, Temporary_Matrix); //a*b=c

    for(int x=0; x<3; x++) //Matrix Addition (update)
    {
      for(int y=0; y<3; y++)
      {
        DCM_Matrix[x][y] += Temporary_Matrix[x][y];
      }
    }
}

void processIMUdata::eulerAngles()
{
    pitch = -asin(DCM_Matrix[2][0]);
    roll = atan2(DCM_Matrix[2][1],DCM_Matrix[2][2]);
    yaw = atan2(DCM_Matrix[1][0],DCM_Matrix[0][0]);
    pitch = TO_DEG(pitch);
    roll = TO_DEG(roll);
    yaw = TO_DEG(yaw);
    std::cout << "#YPR= " << yaw << ',' << pitch << ',' << roll << std::endl;
}

void processIMUdata::readData(QJsonObject imuData)
{
    // Time to read the sensors again?
    qint64 ms = timestamp.msecsTo(QDateTime::currentDateTime());
    std::cout << "ms: " << ms << std::endl;
    if(ms >= OUTPUT__DATA_INTERVAL)
    {
        timestamp_old = timestamp;
        timestamp = QDateTime::currentDateTime();
        if (timestamp >= timestamp_old)
            G_Dt = (float)(timestamp_old.msecsTo(timestamp)) / 1000.0f; // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
        else G_Dt = 0;

        // Update sensor readings
        accel[0] = imuData.value("acc_x").toDouble();
        accel[1] = imuData.value("acc_y").toDouble();
        accel[2] = imuData.value("acc_z").toDouble();

        magnetom[0] = imuData.value("mag_x").toDouble();
        magnetom[1] = imuData.value("mag_y").toDouble();
        magnetom[2] = imuData.value("mag_z").toDouble();

        gyro[0] = imuData.value("gyro_x").toDouble();
        gyro[1] = imuData.value("gyro_y").toDouble();
        gyro[2] = imuData.value("gyro_z").toDouble();

        //output_mode == OUTPUT__MODE_ANGLES; // Output angles

        // Apply sensor calibration
        compensateSensorErrors();

        // Run DCM algorithm
        compassHeading(); // Calculate magnetic heading
        matrixUpdate();
        normalize();
        driftCorrection();
        eulerAngles();

        emit outputAngles(yaw, pitch, roll);
    }
}
