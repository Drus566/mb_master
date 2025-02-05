#ifndef MB_DIRECT_REQUEST_H
#define MB_DIRECT_REQUEST_H

#include "ModbusEnums.h"

#include <cstdint>
#include <mutex>
#include <atomic>

namespace mb {
namespace action {

using namespace mb::types;

class DirectRequest {
public:
	DirectRequest() : m_finish(false) {}
	DirectRequest(int id, FuncNumber func, int addr, int quantity, 
				  uint16_t* u16_mem, uint8_t* u8_mem) : slave_id(id),
																	 function(func),
																	 address(addr),
																	 quantity(quantity),
																	 u16_out_mem(u16_mem),
																	 u8_out_mem(u8_mem),
																	 m_finish(false) {}

	int slave_id;
	FuncNumber function;
	int address;
	int quantity;

	uint16_t *u16_out_mem;
	uint8_t *u8_out_mem;

	void setStatus(bool flag) { m_status.store(flag); }
	bool isSuccess() { return m_status.load() == true; }
	bool isFail() { return m_status.load() == false; }

	void setFinish(bool flag) { m_finish.store(flag); }
	bool isFinish() const { return m_finish.load(); }

private:
	std::atomic<bool> m_finish;
	std::atomic<bool> m_status;
};

} // action
} // mb

#endif // MB_DIRECT_REQUEST_H