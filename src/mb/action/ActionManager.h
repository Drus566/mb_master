#ifndef MB_ACTION_MANAGER_H
#define MB_ACTION_MANAGER_H

#include "Config.h"
#include "DataManager.h"
#include "MemManager.h"
#include "ModbusConnection.h"
#include "ModbusRequest.h"

#include <chrono>
#include <thread>
#include <atomic>

namespace mb {
namespace action {

class ModbusMaster;

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
	std::chrono::milliseconds m_response_timeout;
	std::chrono::milliseconds m_poll_delay;
	int m_max_count_errors;

	void payload(DataManager* data_manager, MemManager* mem_manager);
};

} // action
} // mb

#endif // MB_ACTION_MANAGER

/**
 * start() - начало отправки запросов чтения по modbus
 */


// libmodbuspp master read-coils example

// reads 5 coils on RTU slave at address 8

// This example code is in the public domain.
// #include <iostream>
// #include <modbuspp.h>

// using namespace std;
// using namespace Modbus;

// int main (int argc, char **argv) {
//   string port = string ("/dev/ttyUSB0");

//   if (argc > 1) {

//     port = argv[1]; // the serial port can be provided as a parameter on the command line.
//   }

//   Master mb (Rtu, port, "19200E1"); // new master on RTU
//   // if you have to handle the DE signal of the line driver with RTS,
//   // you should uncomment the lines below...
//   // mb.rtu().setRts(RtsDown);
//   // mb.rtu().setSerialMode(Rs485);

//   Slave & slv = mb.addSlave (8);

//   cout << "Reads coils of slave[" << slv.number() << "] on " <<
//        mb.connection() << " (" << mb.settings() << ")" << endl;

//   if (mb.open ()) { // open a connection
//     bool coil[5];

//     int ncoils = slv.readCoils (1, coil, 5); // reads coils 1 to 5
//     if (ncoils > 0) {

//       // if success, print the binary values
//       cout << "coils: ";
//       for (int i = 0; i < ncoils; i++) {

//         cout << coil[i];
//       }
//       cout << endl;
//     }
//     else {
//       cerr << "Unable to read coils ! "  << mb.lastError() << endl;
//       exit (EXIT_FAILURE);
//     }
//     mb.close();
//   }
//   else {
//     cerr << "Unable to open MODBUS connection to " << port << " : " << mb.lastError() << endl;
//     exit (EXIT_FAILURE);
//   }

//   return 0;
// }
