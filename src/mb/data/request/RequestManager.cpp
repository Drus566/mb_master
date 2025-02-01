#include "RequestManager.h"

namespace mb {
namespace data {

void RequestManager::addRequest(Request& req) { m_read_requests.push_back(req); }

void RequestManager::addRequest(int id, FuncNumber func, int addr, int quantity, MemoryChunk* mem_chunk, int offset) {
    m_read_requests.emplace_back(id, func, addr, quantity, mem_chunk, offset);
}

const int RequestManager::getMaxCountRegsRead() { return m_max_count_regs_read; }

const std::vector<Request>& RequestManager::getReadRequests() { return m_read_requests; }

} // data
} // mb