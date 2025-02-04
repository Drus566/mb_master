#ifndef MB_MODBUS_REQUEST_H
#define MB_MODBUS_REQUEST_H

#include "MemoryChunk.h"
#include "ModbusEnums.h"

namespace mb {
namespace types {

struct Request {
	Request(int id, 
			  FuncNumber func, 
			  int addr, 
			  int quantity, 
			  MemoryChunk* mem_chunk, 
			  int offset) : slave_id(id), 
	   					    function(func), 
							    address(addr), 
							    quantity(quantity), 
							    mem_chunk(mem_chunk), 
							    offset_mem_chunk(offset) {}

   int slave_id;
   FuncNumber function;
   int address;
   int quantity;

   int offset_mem_chunk;
   MemoryChunk* mem_chunk;
   
	bool isIncludeAddress(const int addr) const {
		int limit = address + quantity - 1;
		return (addr >= address && addr <= limit);  
	};
	bool isReadReq() { return function >= 1 && function <= 4; }
	bool isWriteReq() { return !isReadReq(); }
};

} // types
} // mb

#endif // MB_MODBUS_REQUEST_H