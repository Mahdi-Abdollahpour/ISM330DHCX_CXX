#pragma once

#include <queue>
// #include <string>
#include <thread>
#include <mutex>
#include <condition_variable>



struct IMU_Data {
	//  dt 
	int dt = 0;
	// accel data
	float ax = 0;
	float ay = 0;
	float az = 0;
	// gyro data
	float gx = 0;
	float gy = 0;
	float gz = 0; 

};
struct IMU_DataRaw {
	//  dt 
	int dt = 0;
	// accel data
	int ax = 0;
	int ay = 0;
	int az = 0;
	// gyro data
	int gx = 0;
	int gy = 0;
	int gz = 0; 

};

class ProcessData
{
public:
	// Starts a background thread writing enqueue entries to a file.
	ProcessData();
	// Gracefully shut down background thread.
	virtual ~ProcessData();
	// Prevent copy construction and assignment.
	ProcessData(const ProcessData& src) = delete;

	ProcessData& operator=(const ProcessData& rhs) = delete;
	// Add enqueue entry to the queue.
	void enqueue(IMU_Data entry);
	void enqueue(IMU_DataRaw entry);

private:
	// The function running in the background thread.
	void processEntries();
	void processEntriesRaw();

	// Helper method to process a queue of entries.
	void processEntriesHelper(std::queue<IMU_Data>& queue, std::ofstream& ofs) const;
	void processEntriesHelper(std::queue<IMU_DataRaw>& queue, std::ofstream& ofs) const;
	// Boolean telling the background thread to terminate.
	bool m_exit{ false };
	// Mutex and condition variable to protect access to the queue.
	std::mutex m_mutex;
	std::condition_variable m_condVar;
	std::queue<IMU_Data> m_queue;
	std::queue<IMU_DataRaw> m_queue_raw;
	// The background thread.
	std::thread m_logger_thread;
};









