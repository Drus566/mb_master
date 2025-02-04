#ifndef MB_ACTION_MANAGER_H
#define MB_ACTION_MANAGER_H

#include "Config.h"
#include "DataManager.h"
#include "MemManager.h"
#include "ModbusConnection.h"
#include "ModbusRequest.h"
#include "Queue.h"

#include <chrono>
#include <thread>
#include <atomic>

namespace mb {
namespace action {

class ModbusMaster;

using namespace mb::helpers;
using namespace mb::config;
using namespace mb::types;
using namespace mb::data;
using namespace mb::mem;

class ActionManager {    
public:
   ActionManager(Config* config, DataManager* data_manager, MemManager* mem_manager);
   ~ActionManager();
	
   bool start();
	bool stop();
	bool isRun();
	bool isConnect();

	bool addOutRequest();

	void setDebug(int flag);
	void setLog(int flag);

	void printInfo();

private:
	ModbusConnection m_modbus_connection;
  	ModbusMaster* m_modbus_master;

	Config* m_config;
	DataManager* m_data_manager;
	MemManager* m_mem_manager;

	std::atomic<bool> m_run;
	std::atomic<bool> m_connect;
	std::thread m_thread;

	std::chrono::milliseconds m_time_between_requests;
	std::chrono::milliseconds m_poll_delay;
	
	int m_max_count_errors;

	Queue<OutRequest> m_out_requests;

	void payload(DataManager* data_manager, MemManager* mem_manager);

	class OutRequestQueue {
		OutRequest queue[20];
		int count;

		void push();
		void pop();
	};
};

} // action
} // mb

#endif // MB_ACTION_MANAGER