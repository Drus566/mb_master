#ifndef MB_DIRECT_REQUEST_MANAGER_H
#define MB_DIRECT_REQUEST_MANAGER_H

#include "DirectRequest.h"
#include "ObjectPool.h"
#include "Queue.h"

namespace mb {
namespace action {

using namespace mb::helpers;

class DirectRequestManager {
public:
	DirectRequestManager();
	~DirectRequestManager();

	DirectRequest* addDirectRequest(void* vals, const int slave_id, const int func, const int addr, const int count);
	Queue<DirectRequest*>* getQueue();

private:
	ObjectPool<DirectRequest>* m_pool;
	Queue<DirectRequest*>* m_queue;
};

} // mb
} // action


#endif // MB_DIRECT_REQUEST_MANAGER_H
