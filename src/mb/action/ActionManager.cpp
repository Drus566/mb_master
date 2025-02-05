/** timing params */
#define DEFAULT_TIME_BETWEEN_REQUESTS 10
#define DEFAULT_RESPONSE_TIMEOUT 500
#define DEFAULT_BYTE_TIMEOUT 10
#define DEFAULT_POOL_DELAY 10

#define DEFAULT_MAX_COUNT_ERRORS 10

/** rtu default params */
#define DEFAULT_BAUDRATE 19200
#define DEFAULT_STOP_BIT 1
#define DEFAULT_DATA_BITS 8
#define DEFAULT_PARITY 'N'

/** ethernet default params */
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 502

#include "ActionManager.h"

#include "ModbusMaster.h"
#include "Logger.h"

#include <iostream>
#include <fstream>

namespace mb {
namespace action {

ActionManager::ActionManager(Config* config, 
							 DataManager* data_manager,
							 MemManager* mem_manager) : m_config(config), 
									  					m_data_manager(data_manager),
														m_mem_manager(mem_manager),
							   		  					m_run(false),
														m_connect(false) {
	std::chrono::milliseconds response_timeout = m_config->responseTimeout();
	std::chrono::milliseconds byte_timeout = m_config->byteTimeout();

	m_time_between_requests = m_config->timeBetweenRequests();
	m_poll_delay = m_config->pollDelay();
	
	m_max_count_errors = m_config->maxCountErrors();

	if (m_time_between_requests.count() == 0) m_time_between_requests = std::chrono::milliseconds(DEFAULT_TIME_BETWEEN_REQUESTS);
	if (m_poll_delay.count() == 0) m_poll_delay = std::chrono::milliseconds(DEFAULT_POOL_DELAY);
	if (m_max_count_errors == 0) m_max_count_errors = DEFAULT_MAX_COUNT_ERRORS;

	if (response_timeout.count() == 0) response_timeout = std::chrono::milliseconds(DEFAULT_RESPONSE_TIMEOUT);
	if (byte_timeout.count() == 0) byte_timeout = std::chrono::milliseconds(DEFAULT_BYTE_TIMEOUT);

	m_modbus_connection.response_timeout = response_timeout.count();
	m_modbus_connection.byte_timeout = byte_timeout.count();

	std::string type = m_config->type();
	type = toUpperCase(type);

	if (type == "RTU") {
		m_modbus_connection.type = ModbusConnectionType::RTU;

		m_modbus_connection.rtu.serial_port = m_config->serialPort();

		int baudrate = m_config->baudrate();
		if (baudrate == 0) m_modbus_connection.rtu.baudrate = DEFAULT_BAUDRATE;
		m_modbus_connection.rtu.baudrate = baudrate;

		int stop_bit = m_config->stopBit();
		if (stop_bit == 0) m_modbus_connection.rtu.stop_bit = DEFAULT_STOP_BIT;
		m_modbus_connection.rtu.stop_bit = stop_bit;

		int data_bits = m_config->dataBits();
		if (data_bits == 0) m_modbus_connection.rtu.data_bits = DEFAULT_DATA_BITS;
		m_modbus_connection.rtu.data_bits = data_bits;

		std::string parity = m_config->parity();
		if (parity.empty()) m_modbus_connection.rtu.parity = DEFAULT_PARITY;
		else m_modbus_connection.rtu.parity = parity.c_str()[0];
	}
	else if (type == "TCP") {
		m_modbus_connection.type = ModbusConnectionType::ETH;
		
		std::string ip = m_config->ip();
		if (ip.empty()) m_modbus_connection.eth.ip = DEFAULT_IP;
		else m_modbus_connection.eth.ip = ip;
		
		int port = m_config->port();
		if (port == 0) port = DEFAULT_PORT;
		m_modbus_connection.eth.port = port;
	}

	m_modbus_master = new ModbusMaster(m_modbus_connection);
} 

ActionManager::~ActionManager() {
	stop();
	if (m_modbus_master) delete m_modbus_master;
}

bool ActionManager::start() {
	if (m_config->printSettings()) printInfo();

	Logger::Instance()->log(LogLevel::INFO, "Modbus master initialize...");

	if (m_run.load()) {
		Logger::Instance()->log(LogLevel::INFO, "Modbus master already runned");
		return false;
	}
	
	if (!m_modbus_master->setContext()) {
		Logger::Instance()->log(LogLevel::ERROR, "Modbus master invalid set context");
		return false;
	}

	if (!m_modbus_master->connect()) {
		Logger::Instance()->log(LogLevel::ERROR, "Modbus master invalid connect");
		return false;
	}

	if (m_config->log()) {
		m_modbus_master->setLog(TRUE);
		Logger::Instance()->log(LogLevel::INFO, "Modbus master set log true");
	}

	if (m_config->debugMode()) {
		Logger::Instance()->log(LogLevel::INFO, "Modbus master set debug true");
		m_modbus_master->setDebug(TRUE);
	}

	m_modbus_master->setErrorRecovery(true);

	m_thread = std::thread(&ActionManager::payload, 
									this, 
									m_data_manager, 
									m_mem_manager);
	m_thread.detach();
	
	return true;
}

bool ActionManager::stop() {
	if (!m_run.load()) return false;
	
	if (m_thread.joinable()) m_thread.join();

	m_modbus_master->close();

	m_run.store(false);
	return true;
}

bool ActionManager::isRun() { return m_run.load(); }

bool ActionManager::isConnect() { return m_connect.load(); };

void ActionManager::setLog(int flag) { m_modbus_master->setLog(flag); }

void ActionManager::setDebug(int flag) { m_modbus_master->setDebug(flag); }

bool ActionManager::handleDirectRequest(void* vals, const int slave_id, const int func, const int addr, const int count) {
	bool result = false;
	DirectRequest* req = nullptr;
	req = m_direct_req_manager->addDirectRequest(vals, slave_id, func, addr, count);

	if (req) {
		while (!req->finish.load()) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
		result = true;
	}
	return result;
}

// bool ActionManager::f1(uint8_t *val, int id, int addr, int count) {
// 	// m_pool_out_requests;
// 	// Queue<OutRequest> m_queue_out_requests;

// 	while (!out_req->finish) {
// 		std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 	}

// //     // Получаем объекты из пула
// //     Particle* p1 = particlePool.acquire();
// //     Particle* p2 = particlePool.acquire();

// //     if (p1) p1->init(10, 20);
// //     if (p2) p2->init(30, 40);

// //     particlePool.debug();

// //     if (p1) p1->display();
// //     if (p2) p2->display();

// //     // Возвращаем объекты в пул
// //     particlePool.release(p1);
// //     particlePool.release(p2);

// //     particlePool.debug();
// 	return true;
// }
// bool ActionManager::f1(std::string &name, uint8_t *val) override;
// bool ActionManager::f2(uint8_t *val, int slave_id, int addr, int count) override;
// bool ActionManager::f2(std::string &name, uint8_t *val) override;
// bool ActionManager::f3(uint16_t *val, int slave_id, int addr, int count) override;
// bool ActionManager::f3(std::string &name, uint16_t *val) override;
// bool ActionManager::f4(uint16_t *val, int slave_id, int addr, int count) override;
// bool ActionManager::f4(std::string &name, uint16_t *val) override;
// bool ActionManager::f5(uint8_t val, int slave_id, int adr) override;
// bool ActionManager::f5(std::string &name, uint8_t val) override;

void ActionManager::payload(DataManager* data_manager, MemManager* mem_manager) {
	m_run.store(true);

	const std::vector<Request>& read_requests = data_manager->getReadRequests();
	const int max_count_read_regs = data_manager->getMaxCountReadRegs();
	
	uint16_t buffer[max_count_read_regs];
	uint8_t* u8_ptr = reinterpret_cast<uint8_t*>(buffer);
	uint16_t* u16_ptr = buffer;
	int error;
	bool response;
	DirectRequest* r;

	while (m_run.load()) {
		for (const auto& request : read_requests) {
			switch (request.function) {
				case FuncNumber::READ_COIL:
					response = m_modbus_master->readBits(request.slave_id, request.address, request.quantity, u8_ptr);
					if (response) mem_manager->writeMem(u8_ptr, request.mem_chunk->u8_ptr, request.quantity, request.offset_mem_chunk);
					break;
				
				case FuncNumber::READ_INPUT_COIL: 
					response = m_modbus_master->readInputBits(request.slave_id, request.address, request.quantity, u8_ptr); 
					if (response) mem_manager->writeMem(u8_ptr, request.mem_chunk->u8_ptr, request.quantity, request.offset_mem_chunk);
					break;

				case FuncNumber::READ_REGS: 
					response = m_modbus_master->readRegisters(request.slave_id, request.address, request.quantity, u16_ptr); 
					if (response) mem_manager->writeMem(u16_ptr, request.mem_chunk->u16_ptr, request.quantity, request.offset_mem_chunk);
					break;

				case FuncNumber::READ_INPUT_REGS:
					response = m_modbus_master->readInputRegisters(request.slave_id, request.address, request.quantity, u16_ptr); 
					if (response) mem_manager->writeMem(u16_ptr, request.mem_chunk->u16_ptr, request.quantity, request.offset_mem_chunk);
					break;
			}			

			if (!response) { 
				m_modbus_master->flush();
				++error;
			} 
			else { 
				error = 0;
				m_connect.store(true);
			}

			if (error > m_max_count_errors) { 
				error = m_max_count_errors;
				m_connect.store(false);
			}

			std::this_thread::sleep_for(m_time_between_requests);
			
			// TODO: check out requests
			// while (!m_queue_out_requests.empty()) {
			// 	if (m_queue_out_requests.pop(r)) {
					switch (request.function) {
						case FuncNumber::WRITE_SINGLE_COIL:
							response = m_modbus_master->readBits(r->slave_id, r->address, r->quantity, r->u8_out_mem);
							break;

						case FuncNumber::WRITE_MULTIPLE_COILS:
							response = m_modbus_master->readInputBits(r->slave_id, r->address, r->quantity, r->u8_out_mem);
							break;

						case FuncNumber::WRITE_SINGLE_WORD:
							response = m_modbus_master->readRegisters(r->slave_id, r->address, r->quantity, r->u16_out_mem);
							break;

						case FuncNumber::WRITE_MULTIPLE_WORDS:
							response = m_modbus_master->readInputRegisters(r->slave_id, r->address, r->quantity, r->u16_out_mem);
							break;
					}
				}
				std::this_thread::sleep_for(m_time_between_requests);
		// 	}
		// }
		std::this_thread::sleep_for(m_poll_delay);
	}
}

void ActionManager::printInfo() {
	if (m_config->printSettings()) {
		std::cout << "******************************** SETTINGS *******************************" << std::endl;
		Logger::Instance()->rawLog("******************************** SETTINGS *******************************");
		
		if (m_modbus_connection.type == ModbusConnectionType::RTU) {
			std::cout << "Type=RTU" << std::endl;
			std::cout << "SerialPort=" << m_modbus_connection.rtu.serial_port << std::endl;
			std::cout << "Baudrate=" << m_modbus_connection.rtu.baudrate << std::endl;
			std::cout << "StopBit=" << m_modbus_connection.rtu.stop_bit << std::endl;
			std::cout << "DataBits=" << m_modbus_connection.rtu.data_bits << std::endl;
			std::cout << "Parity=" << m_modbus_connection.rtu.parity << std::endl;

			Logger::Instance()->rawLog("Type=RTU");
			Logger::Instance()->rawLog("SerialPort=%s", m_modbus_connection.rtu.serial_port.c_str());
			Logger::Instance()->rawLog("Baudrate=%d", m_modbus_connection.rtu.baudrate);
			Logger::Instance()->rawLog("StopBit=%d", m_modbus_connection.rtu.stop_bit);
			Logger::Instance()->rawLog("DataBits=%d", m_modbus_connection.rtu.data_bits);
			Logger::Instance()->rawLog("Parity=%c", m_modbus_connection.rtu.parity);
		}

		else if (m_modbus_connection.type == ModbusConnectionType::ETH) {
			std::cout << "Type=ETH(Modbus TCP)" << std::endl;
			std::cout << "Ip=" << m_modbus_connection.eth.ip << std::endl;
			std::cout << "Port=" << m_modbus_connection.eth.port << std::endl;

			Logger::Instance()->rawLog("Type=ETH(Modbus TCP)");
			Logger::Instance()->rawLog("Ip=%s", m_modbus_connection.eth.ip);
			Logger::Instance()->rawLog("Port=%d", m_modbus_connection.eth.port);
		}

		std::cout << "MaxCountRegsRead=" << m_data_manager->getMaxCountReadRegs() << std::endl;
		std::cout << "TimeBetweenRequests=" << m_time_between_requests.count() << " milliseconds" << std::endl;
		std::cout << "ResponseTimeout=" << m_modbus_connection.response_timeout << " milliseconds" << std::endl;
		std::cout << "ByteTimeout=" << m_modbus_connection.byte_timeout << " milliseconds" << std::endl;
		std::cout << "PoolDelay=" << m_poll_delay.count() << " milliseconds" << std::endl;
		std::cout << "MaxErrors=" << m_max_count_errors << std::endl;

		Logger::Instance()->rawLog("MaxCountRegsRead=%d", m_data_manager->getMaxCountReadRegs());
		Logger::Instance()->rawLog("TimeBetweenRequests=%d milliseconds", m_time_between_requests.count());
		Logger::Instance()->rawLog("ResponseTimeout=%d milliseconds", m_modbus_connection.response_timeout);
		Logger::Instance()->rawLog("ByteTimeout=%d milliseconds", m_modbus_connection.byte_timeout);
		Logger::Instance()->rawLog("PoolDelay=%d milliseconds", m_poll_delay.count());
		Logger::Instance()->rawLog("MaxErrors=%d milliseconds", m_max_count_errors);

		std::cout << "*************************************************************************" << std::endl;
		Logger::Instance()->rawLog("*************************************************************************");
	}
}

} // action
} // mb
