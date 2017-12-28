#ifndef __MPU6050_H__
#define __MPU6050_H__
union mpu6050
{
	struct mpu6050_gyro
	{
		unsigned short x;
		unsigned short y;
		unsigned short z;
	}gyro;

	struct mpu6050_accel
	{
		unsigned short x;
		unsigned short y;
		unsigned short z;
	}accel;

	unsigned short temp;
};

#define GYRO_CMD _IOR('x',0,union mpu6050)
#define ACCEL_CMD _IOR('x',1,union mpu6050)
#define TEMP_CMD _IOR('x',2,union mpu6050)

#endif
