#include "DirectRequestManager.h"

#define POOL_OUT_REQUESTS_SIZE 20

namespace mb {
namespace action {

DirectRequestManager::DirectRequestManager() : m_pool(POOL_OUT_REQUESTS_SIZE),
									 						  m_queue() {}

DirectRequest* DirectRequestManager::addDirectRequest(void* vals, const int slave_id, const int func, const int addr, const int count) {
	DirectRequest* req = nullptr;
	req = m_pool.acquire();
	uint8_t* u8_vals = nullptr;
	uint16_t* u16_vals = nullptr;

	if (isCoilFunc(func)) u8_vals = static_cast<uint8_t*>(vals);
	else u16_vals = static_cast<uint16_t*>(vals);

	if (req) {
		req->address = addr;
		req->slave_id = slave_id;
		req->quantity = count;
		req->function = FuncNumber::READ_COIL;
		req->u8_out_mem = u8_vals;
		req->u16_out_mem = u16_vals;
		// req->setFinish(false);
		if (!m_queue.push(req)) req = nullptr;
	}

	return req;
}

// DirectRequest DirectRequestManager::*addDirectRequest(const int func, uint8_t *const vals, const std::string &name, const int slave_id, const int addr, const int count) {
// 	DirectRequest *req = nullptr;
// 	req = m_pool.acquire();

// 	if (req) {
// 		req->address = addr;
// 		req->slave_id = id;
// 		req->quantity = count;
// 		req->function = FuncNumber::READ_COIL;
// 		req->u8_out_mem = val;
// 		req->finish = false;
// 		m_queue.push(req);
// 	}
// }

} // action
} // mb
