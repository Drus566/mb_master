#include "RequestManager.h"
#include "Logger.h"

#include <iostream>

namespace mb {
namespace data {

using namespace mb::log;

void RequestManager::addRequest(Request& req) { m_read_requests.push_back(req); }

void RequestManager::addRequest(int id, FuncNumber func, int addr, int quantity, MemoryChunk* mem_chunk, int offset) {
    m_read_requests.emplace_back(id, func, addr, quantity, mem_chunk, offset);
    
}

const int RequestManager::getMaxCountRegsRead() { return m_max_count_regs_read; }

const std::vector<Request>& RequestManager::getReadRequests() { return m_read_requests; }

void RequestManager::printInfo() {
    std::cout << "*************** REQUESTS [SLAVE_ID, FUNC, ADDR, QUANTITY] ***************" << std::endl;
    Logger::Instance()->rawLog("*************** REQUESTS [SLAVE_ID, FUNC, ADDR, QUANTITY] ***************");
    for (Request& r : m_read_requests) {
        std::cout << "[" << r.slave_id << "," 
        << r.function << "," 
        << r.address << "," 
        << r.quantity << "]" << std::endl;

        Logger::Instance()->rawLog("[%d, %d, %d, %d]", r.slave_id, r.function, r.address, r.quantity);
    }
    std::cout << "*************************************************************************" << std::endl;
    Logger::Instance()->rawLog("*************************************************************************");
}

} // data
} // mb