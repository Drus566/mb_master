#include "MemManager.h"

#include <iostream>

namespace mb {
namespace mem {

bool MemManager::createU16Memory(MemoryChunk*& mem, int quantity) {
	bool result = false;
	uint16_t* ptr = new (std::nothrow) uint16_t[quantity]();
	if (ptr) {
		m_memory_chunks.emplace_front(ptr, quantity);
		auto it = m_memory_chunks.begin();
		mem = &(*it);
		result = true;
	}
	return result;
}

bool MemManager::createU8Memory(MemoryChunk*& mem, int quantity) {
	bool result = false;
	uint8_t* ptr = new (std::nothrow) uint8_t[quantity]();
	if (ptr) {
		m_memory_chunks.emplace_front(ptr, quantity);
		auto it = m_memory_chunks.begin();
		mem = &(*it);
		result = true;
	}
	return result;
}

void MemManager::writeMem(uint16_t* payload, MemoryChunk* mem_chunk) {
	std::lock_guard<std::mutex> lock(mtx);
	for (int i = 0; i < mem_chunk->quantity; i++) {
		*(mem_chunk->u16_ptr + i) = *(payload + i);
	}
}

void MemManager::writeMem(uint8_t* payload, MemoryChunk* mem_chunk) {
	std::lock_guard<std::mutex> lock(mtx);
	for (int i = 0; i < mem_chunk->quantity; i++) {
		*(mem_chunk->u8_ptr + i) = *(payload + i);
	}
}

void MemManager::writeMem(uint8_t* payload, uint8_t* start, int quantity, int offset) {
	std::lock_guard<std::mutex> lock(mtx);
	for (int i = 0; i < quantity; i++) {
		*(start + i + offset) = *(payload + i);
	}
}

void MemManager::writeMem(uint16_t* payload, uint16_t* start, int quantity, int offset) {
	std::lock_guard<std::mutex> lock(mtx);
	for (int i = 0; i < quantity; i++) {
		*(start + i + offset) = *(payload + i);
	}
}

bool MemManager::getDWord(uint32_t* val, uint16_t* ptr) {
	std::lock_guard<std::mutex> lock(mtx);
	bool result = false;
	if (ptr) {
		uint16_t* val_ptr = reinterpret_cast<uint16_t*>(val);
		*val_ptr = *ptr;
		*(val_ptr + 1) = *(ptr + 1);
		result = true;
	}
	return result;
}

bool MemManager::getWord(uint16_t* val, uint16_t* ptr) {
	std::lock_guard<std::mutex> lock(mtx);
	bool result = false;
	if (ptr) {
		*val = *ptr;
		result = true;
	}
	return result;
}

bool MemManager::getCoil(uint8_t* val, uint8_t* ptr) {
	std::lock_guard<std::mutex> lock(mtx);
	bool result = false;
	if (ptr) {
		*val = *ptr;
		result = true;
	}
	return result;
}

bool MemManager::printMemoryChunks() {
	bool result = false;

	auto it = m_memory_chunks.begin(); // Итератор на начало списка

   while (it != m_memory_chunks.end()) {
		if (it->u8_ptr != NULL) {
			std::cout << "U8 chunks: ";
			for (int i = 0; i < it->quantity; i++) {
				std::cout << "[" << static_cast<int>(*(it->u8_ptr + i)) << "]";
			}
			std::cout << std::endl;
		}
		else if (it->u16_ptr != NULL) {
			std::cout << "U16 chunks: ";
			for (int i = 0; i < it->quantity; i++) {
				std::cout << "[" << *(it->u16_ptr + i) << "]";
			}
			std::cout << std::endl;
		}
      ++it; // Переходим к следующему элементу
		result = true;
   }
	return result;
}

} // mem
} // mb
