#ifndef MB_MEMORY_CHUNK_H
#define MB_MEMORY_CHUNK_H

#include <cstdint>

namespace mb {
namespace types {

struct MemoryChunk {
	uint16_t* u16_ptr;
	uint8_t* u8_ptr;
	int quantity;

	MemoryChunk();
	MemoryChunk(uint16_t* s, int q) : u16_ptr(s), quantity(q), u8_ptr(nullptr) {}
	MemoryChunk(uint8_t* s, int q) : u8_ptr(s), quantity(q), u16_ptr(nullptr) {}

	~MemoryChunk() {
		if (u16_ptr) {
			delete[] u16_ptr; 
			u16_ptr = nullptr;
		} 

		if (u8_ptr) {
			delete[] u8_ptr; 
			u8_ptr = nullptr;
		} 
	}
};

} // types
} // mb

#endif // MB_MEMORY_CHUNK_H