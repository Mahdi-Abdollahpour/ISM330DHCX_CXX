#include "ProcessData.h"
#include <iostream>
#include <vector>
#include <sstream>

#include "ism330dhcx.h"
#include <wiringPiI2C.h>
#include <chrono>
// #include <bitset>

#include "ism330_reg.h"

#include <pthread.h>
// #include <thread>
// #include "rt_thread.h"
#include <cstring>
#include <stdexcept>
#include <sys/mman.h> // necessary for mlockall

using namespace std;

void *read_queue_imu(void *procData)
{
	ProcessData *processData = static_cast<ProcessData *>(procData);

	using namespace std::chrono;
	// --------------------------- Setup I2C communication
	int devId = ISM330DHCX_ADDRESS_LOW;
	const char *device;
	device = "/dev/i2c-1";
	// device = "/dev/i2c-1" ;

	int fd = wiringPiI2CSetupInterface(device, devId);
	// int fd = wiringPiI2CSetup(DEVICE_ID);

	if (fd == -1)
	{
		std::cout << "Failed to init I2C communication.\n";
		// return false;
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
	myISM.setAccelFullScale(ISM_2g);

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
	cout << "processing ..." << endl;
	sfe_ism_data_t accelData;
	sfe_ism_data_t gyroData;
	auto start = chrono::high_resolution_clock::now();
	// unsync the I/O of C and C++.
	ios_base::sync_with_stdio(false);
	auto end = chrono::high_resolution_clock::now();
	sfe_ism_raw_data_t accelDataRaw;
	sfe_ism_raw_data_t gyroDataRaw;
	while (true)
	{
		auto time_point1 = high_resolution_clock::now();
		bool data_ready = myISM.checkStatus();

		// bool data_ready = check_status(fd);

		auto time_point2 = high_resolution_clock::now();
		int dt_checkStatus = duration_cast<microseconds>(time_point2 - time_point1).count();

		// cout<<"dt_checkStatus:"<<dt_checkStatus<<endl;
		// bool data_ready = true;
		if (data_ready)
		// if(ret==0)
		{

			auto time_point3 = high_resolution_clock::now();
			int dt = duration_cast<microseconds>(time_point3 - start).count();
			start = time_point3;

			// myISM.getAccel(&accelData);
			// myISM.getGyro(&gyroData);

			myISM.getRawAccel(&accelDataRaw);
			myISM.getRawGyro(&gyroDataRaw);
			IMU_DataRaw imu_data{dt, accelDataRaw.xData, accelDataRaw.yData, accelDataRaw.zData,
							  gyroDataRaw.xData, gyroDataRaw.yData, gyroDataRaw.zData};

			// RawData accel_raw_data;
			// RawData gyro_raw_data;
			// getRawAccel(fd, accel_raw_data);
			// getRawGyro(fd, gyro_raw_data);
			// IMU_Data imu_data{dt, accel_raw_data.x, accel_raw_data.y, accel_raw_data.z,
			// 				  gyro_raw_data.x, gyro_raw_data.y, gyro_raw_data.z};

			auto time_point4 = high_resolution_clock::now();



			cout << "dt:" << dt << " ";
			// cout<<myISM.checkStatus();

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

			processData->enqueue(imu_data);

			auto time_point5 = high_resolution_clock::now();
			int dt_getRawData = duration_cast<microseconds>(time_point4 - time_point3).count();
			int dt_ProcessData = duration_cast<microseconds>(time_point5 - time_point4).count();

			cout << " dt_checkStatus:" << dt_checkStatus << " dt_getRawData:" << dt_getRawData;
			cout << " dt_ProcessData:" << dt_ProcessData << endl;

			int ddt = duration_cast<microseconds>(time_point3 - time_point1).count();
			this_thread::sleep_for(std::chrono::microseconds(7000-ddt));
		}
		// this_thread::sleep_for(1ms);
	}
	// return true;
}

void LockMemory()
{
	int ret = mlockall(MCL_CURRENT | MCL_FUTURE);
	if (ret)
	{
		throw std::runtime_error{std::strerror(errno)};
	}
}
int main()
{
	LockMemory();

	ProcessData ProcessData;

	// thread read_imu_thread{read_queue_imu, ref(ProcessData)};
	int priority_ = 80;
	int policy_ = SCHED_FIFO;
	pthread_t thread_;
	pthread_attr_t attr;
	// Initialize the pthread attribute
	int ret = pthread_attr_init(&attr);
	if (ret)
	{
		throw std::runtime_error(std::strerror(ret));
	}
	// Set the scheduler policy
	ret = pthread_attr_setschedpolicy(&attr, policy_);
	if (ret)
	{
		throw std::runtime_error(std::strerror(ret));
	}
	// Set the scheduler priority
	struct sched_param param;
	param.sched_priority = priority_;
	ret = pthread_attr_setschedparam(&attr, &param);
	if (ret)
	{
		throw std::runtime_error(std::strerror(ret));
	}
	// Make sure threads created using the thread_attr_ takes the value
	// from the attribute instead of inherit from the parent thread.
	ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	if (ret)
	{
		throw std::runtime_error(std::strerror(ret));
	}
	// Finally create the thread
	ret = pthread_create(&thread_, &attr, &read_queue_imu, &ProcessData);
	if (ret)
	{
		throw std::runtime_error(std::strerror(ret));
	}
	pthread_join(thread_, NULL);

	// Thread rt_thread(80, SCHED_FIFO,ProcessData, read_queue_imu);

	// read_imu_thread.join();
}
