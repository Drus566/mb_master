#ifndef MB_ACTION_MANAGER_H
#define MB_ACTION_MANAGER_H

#include "Config.h"
#include "DataManager.h"
#include "MemManager.h"
#include "ModbusConnection.h"
#include "ModbusRequest.h"
#include "Queue.h"
#include "ObjectPool.h"

#include <chrono>
#include <thread>
#include <atomic>

namespace mb {
namespace action {

class DirectRequest {
	DirectRequest() {}
	DirectRequest(int id, FuncNumber func, int addr, int quantity, 
				  uint16_t* u16_mem, uint8_t* u8_mem) : slave_id(id),
																	 function(func),
																	 address(addr),
																	 quantity(quantity),
																	 u16_out_mem(u16_mem),
																	 u8_out_mem(u8_mem) {}
	bool active;
	bool finish;

	int slave_id;
	FuncNumber function;
	int address;
	int quantity;

	uint16_t *u16_out_mem;
	uint8_t *u8_out_mem;
};

} // action
} // mb