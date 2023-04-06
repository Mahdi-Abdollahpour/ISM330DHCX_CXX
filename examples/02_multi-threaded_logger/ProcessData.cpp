

#include "ProcessData.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

ProcessData::ProcessData()
{
	m_logger_thread = thread{&ProcessData::processEntries, this};
}

ProcessData::~ProcessData()
{
	{
		unique_lock lock{m_mutex};
		m_exit = true;
	}
	m_condVar.notify_all();
	m_logger_thread.join();
}

void ProcessData::enqueue(IMU_Data entry)
{
	unique_lock lock{m_mutex};
	m_queue.push(move(entry));

	m_condVar.notify_all();
}

void ProcessData::processEntries()
{
	// Open log file.
	ofstream logFile{"imu_data.txt"};
	if (logFile.fail())
	{
		cerr << "Failed to open logfile." << endl;
		return;
	}

	time_t ttime = time(0);
	char *dt = ctime(&ttime);
	logFile << dt;
	logFile << "ism330dhcx" << endl;
	logFile << "dt(ms) acc_x(mg) acc_y(mg) acc_z(mg) gyr_x(deg/s) gyr_y(deg/s) gyr_z(deg/s)" << endl;

	unique_lock lock{m_mutex, defer_lock};
	while (true)
	{
		lock.lock();

		if (!m_exit)
		{
			m_condVar.wait(lock);
		}
		else
		{
			processEntriesHelper(m_queue, logFile);
			break;
		}

		queue<IMU_Data> localQueue;
		localQueue.swap(m_queue);
		lock.unlock();

		processEntriesHelper(localQueue, logFile);
	}
}

void ProcessData::processEntriesHelper(queue<IMU_Data> &queue, ofstream &ofs) const
{
	while (!queue.empty())
	{

		ofs << queue.front().dt << " ";
		ofs << queue.front().ax << " ";
		ofs << queue.front().ay << " ";
		ofs << queue.front().az << " ";
		ofs << queue.front().gx << " ";
		ofs << queue.front().gy << " ";
		ofs << queue.front().gz << " ";
		ofs << endl;
		queue.pop();
	}
}
