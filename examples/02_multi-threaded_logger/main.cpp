#include "ProcessData.h"
#include <iostream>
#include <thread>
#include <vector>
#include <sstream>

#include "ism330dhcx.h"
#include <wiringPiI2C.h>
#include <chrono>

using namespace std;

void read_queue_imu(ProcessData &ProcessData)
{
	using namespace std::chrono;
	// --------------------------- Setup I2C communication
	int devId = ISM330DHCX_ADDRESS_LOW;
	const char *device;
	device = "/dev/i2c-0";
	// device = "/dev/i2c-1" ;

	int fd = wiringPiI2CSetupInterface(device, devId);
	// int fd = wiringPiI2CSetup(DEVICE_ID);

	if (fd == -1)
	{
		std::cout << "Failed to init I2C communication.\n";
		return;
	}

	// --------------------------- Setup ISM330DHCX myISM;
	QwDevISM330DHCX myISM;
	myISM.setI2C(fd);
	std::cout << "is connected? " << (myISM.isConnected() ? "Yes" : "No") << std::endl;

	myISM.deviceReset();
	// Wait for it to finish reseting
	while (!myISM.getDeviceReset())
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
	std::cout << "device reset compelete.\n";

	myISM.setDeviceConfig();
	myISM.setBlockDataUpdate();

	// Set the output data rate and precision of the accelerometer
	myISM.setAccelDataRate(ISM_XL_ODR_104Hz);
	myISM.setAccelFullScale(ISM_4g);

	// Set the output data rate and precision of the gyroscope
	myISM.setGyroDataRate(ISM_GY_ODR_104Hz);
	myISM.setGyroFullScale(ISM_500dps);

	// Turn on the accelerometer's filter and apply settings.
	myISM.setAccelFilterLP2();
	myISM.setAccelSlopeFilter(ISM_LP_ODR_DIV_100);

	// Turn on the gyroscope's filter and apply settings.
	myISM.setGyroFilterLP1();
	myISM.setGyroLP1Bandwidth(ISM_MEDIUM);
	std::cout << "device config compelete.\n";
	cout<< "processing ..."<<endl;
	sfe_ism_data_t accelData;
	sfe_ism_data_t gyroData;
	auto start = chrono::high_resolution_clock::now();
	// unsync the I/O of C and C++.
	ios_base::sync_with_stdio(false);
	auto end = chrono::high_resolution_clock::now();


	while (true)
	{
		if (myISM.checkStatus())
		{
			end = high_resolution_clock::now();
			int dt = duration_cast<milliseconds>(end - start).count();
			start = end;

			myISM.getAccel(&accelData);
			myISM.getGyro(&gyroData);

			IMU_Data imu_data{dt, accelData.xData, accelData.yData, accelData.zData,
							  gyroData.xData, gyroData.yData, gyroData.zData};

			// cout << "dt:" << dt << " ";

			// cout << "Accelerometer: ";
			// cout << " X:";
			// cout << accelData.xData;
			// cout << " Y:";
			// cout << accelData.yData;
			// cout << " Z:";
			// cout << accelData.zData;

			// cout << "    Gyroscope: ";
			// cout << "X:";
			// cout << gyroData.xData;
			// cout << " Y:";
			// cout << gyroData.yData;
			// cout << " Z:";
			// cout << gyroData.zData;
			// cout << endl;
			
			ProcessData.enqueue(imu_data);
		}
	}
}

int main()
{

	ProcessData ProcessData;
	thread read_imu_thread{read_queue_imu, ref(ProcessData)};
	read_imu_thread.join();
}
