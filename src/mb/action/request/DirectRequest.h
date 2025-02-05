#ifndef MB_DIRECT_REQUEST_H
#define MB_DIRECT_REQUEST_H

#include "ModbusEnums.h"
#include "DirectRequestPool.h"

#include <cstdint>
#include <mutex>

namespace mb {
namespace action {

using namespace mb::types;

class DirectRequest {
public:
	DirectRequest() : m_finish(false), m_mtx() {}
	DirectRequest(int id, FuncNumber func, int addr, int quantity, 
				  uint16_t* u16_mem, uint8_t* u8_mem) : slave_id(id),
																	 function(func),
																	 address(addr),
																	 quantity(quantity),
																	 u16_out_mem(u16_mem),
																	 u8_out_mem(u8_mem),
																	 m_finish(false),m_mtx() {}
	int slave_id;
	FuncNumber function;
	int address;
	int quantity;

	uint16_t *u16_out_mem;
	uint8_t *u8_out_mem;

	void activate() {
		std::lock_guard<std::mutex> lock(m_mtx);
		m_finish = true;
	}

	void deactivate() {
		std::lock_guard<std::mutex> lock(m_mtx);
		m_finish = false;
	}

	bool isActive() const {
		std::lock_guard<std::mutex> lock(m_mtx);
		return m_finish;
	}

private:
	mutable std::mutex m_mtx;
	bool m_finish;
};

} // action
} // mb

#endif // MB_DIRECT_REQUEST_H