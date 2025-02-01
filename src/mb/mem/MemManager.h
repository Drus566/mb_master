#ifndef MB_MEMORY_MANAGER_H
#define MB_MEMORY_MANAGER_H

#include "MemoryChunk.h"

#include <forward_list>
#include <string>
#include <new> // Для std::nothrow
#include <mutex>

namespace mb {
namespace mem {

using namespace mb::types;

class MemManager {    
   public:
		MemManager() {}

		bool createU16Memory(MemoryChunk*& mem, int quantity);
		bool createU8Memory(MemoryChunk*& mem, int quantity);
		void writeMem(uint16_t* payload, MemoryChunk* mem_chunk);
		void writeMem(uint8_t* payload, MemoryChunk* mem_chunk);
		void writeMem(uint8_t* payload, uint8_t* start, int quantity, int offset);
		void writeMem(uint16_t* payload, uint16_t* start, int quantity, int offset);
		bool getDWord(uint32_t* val, uint16_t* ptr);
		bool getWord(uint16_t* val, uint16_t* ptr);
		bool getCoil(uint8_t* val, uint8_t* ptr);
		
		bool printMemoryChunks();

   private:
		std::forward_list<MemoryChunk> m_memory_chunks;
		std::mutex mtx;
};

} // mem
} // mb

#endif // MB_MEMORY_MANAGER_H