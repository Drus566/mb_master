#include "MB.h"

#include <iostream>

namespace mb {

std::unique_ptr<IMB> IMB::create(const std::string& config_path) { return std::unique_ptr<IMB>(new MB(config_path)); }

MB::MB(const std::string& config_path) : m_start(false) {
	m_config = new Config(config_path);
	m_mem_manager = new MemManager();
	m_data_manager = new DataManager(m_config, m_mem_manager);
	m_action_manager = new ActionManager(m_config, m_data_manager, m_mem_manager);
}

MB::~MB() {
	if (m_config) delete m_config;
	if (m_mem_manager) delete m_mem_manager;
	if (m_data_manager) delete m_data_manager;
	if (m_action_manager) delete m_action_manager;
}

bool MB::start() {
	if (m_config->log()) Logger::Instance()->setActive(true);
	Logger::Instance()->log(LogLevel::INFO, "============== Start MB ==============");

	if (m_start) {
		Logger::Instance()->log(LogLevel::INFO, "Already started");
		return false;
	}

	if (!m_data_manager->start()) return false;

	Logger::Instance()->log(LogLevel::INFO, "Modbus data success initialized");

	if (!m_action_manager->start()) return false;

	Logger::Instance()->log(LogLevel::INFO, "Modbus master success initialized");

	Logger::Instance()->setActive(false);

	m_start = true;
	return true;
}

bool MB::isConnect() { return m_action_manager->isConnect(); }

void MB::startLog() { Logger::Instance()->setActive(true); }

void MB::stopLog() { Logger::Instance()->setActive(false); }

void MB::startDebug() { m_action_manager->setDebug(true); }

void MB::stopDebug() { m_action_manager->setDebug(false); }

bool MB::runRequest(void *vals, const int slave_id, const int func, const int addr, const int count) {
	void *values = static_cast<void *>(vals);
	return m_action_manager->handleDirectRequest(values, slave_id, func, addr, count);
}

bool MB::runRequest(void *vals, const std::string &name, const int count) {
	bool result = false;
	void *values = static_cast<void *>(vals);
	Register *reg = m_data_manager->findReadRegOnlyByName(name);
	if (reg) result = m_action_manager->handleDirectRequest(values, reg->slave_id, reg->function, reg->address, count);
	return result;
}

IMB::ModbusData* MB::getDataOnlyByName(const std::string& name) { 
	MB::Data* result = nullptr;
	Register* reg = nullptr;
	uint16_t* u16_data_ptr = nullptr;
	uint8_t* u8_data_ptr = nullptr;

	reg = m_data_manager->findReadRegOnlyByName(name);
	if (reg) {
		if (reg->isCoil()) {
			u8_data_ptr = m_data_manager->findU8DataMemory(reg->address, reg->function, reg->slave_id);
			if (u8_data_ptr) result = new MB::Data(m_mem_manager, reg, u16_data_ptr, u8_data_ptr);
		}
		else {
			u16_data_ptr = m_data_manager->findU16DataMemory(reg->address, reg->function, reg->slave_id);
			if (u16_data_ptr) result = new MB::Data(m_mem_manager, reg, u16_data_ptr, u8_data_ptr);
		}
	}

	return result;
}

/* В случае если есть регистры с одинаковым именем в разных slave_id и/или в разных функциях */
IMB::ModbusData* MB::getData(const std::string& name, const int func, const int slave_id) { 
	MB::Data* result = nullptr;
	Register* reg = nullptr;
	uint16_t* u16_data_ptr = nullptr;
	uint8_t* u8_data_ptr = nullptr;

	reg = m_data_manager->findReadReg(name, func, slave_id);
	if (reg) {
		if (reg->isCoil()) {
			u8_data_ptr = m_data_manager->findU8DataMemory(reg->address, reg->function, reg->slave_id);
			if (u8_data_ptr) result = new MB::Data(m_mem_manager, reg, u16_data_ptr, u8_data_ptr);
		}
		else  {
			u16_data_ptr = m_data_manager->findU16DataMemory(reg->address, reg->function, reg->slave_id);
			if (u16_data_ptr) result = new MB::Data(m_mem_manager, reg, u16_data_ptr, u8_data_ptr);
		}
	}
	return result;
}

IMB::ModbusData* MB::getData(const int addr, const int func, const int slave_id) { 
	MB::Data* result = nullptr;
	Register* reg = nullptr;
	uint16_t* u16_data_ptr = nullptr;
	uint8_t* u8_data_ptr = nullptr;

	reg = m_data_manager->findReadReg(addr, func, slave_id);
	if (!reg) reg = m_data_manager->createReadReg(addr, func, slave_id);
	if (reg) {
		if (reg->isCoil()) {
			u8_data_ptr = m_data_manager->findU8DataMemory(reg->address, reg->function, reg->slave_id);
			if (u8_data_ptr) result = new MB::Data(m_mem_manager, reg, u16_data_ptr, u8_data_ptr);
		}
		else  {
			u16_data_ptr = m_data_manager->findU16DataMemory(reg->address, reg->function, reg->slave_id);
			if (u16_data_ptr) result = new MB::Data(m_mem_manager, reg, u16_data_ptr, u8_data_ptr);
		}
	}
	return result;
}

bool MB::Data::getBit() { 
	bool result;
	uint8_t val;
	m_mem_manager->getCoil(&val, m_u8_data_ptr);

	result = static_cast<bool>(val); 
	return result;
}
         
int MB::Data::getShort() {
   int result;
	uint16_t val;
	m_mem_manager->getWord(&val, m_u16_data_ptr);

   mb::types::ModbusTrans::interpretReg16(result, val, *m_reg);
   return result;
}
      
int MB::Data::getUShort() {
   unsigned int result;
	uint16_t val;
	m_mem_manager->getWord(&val, m_u16_data_ptr);
   mb::types::ModbusTrans::interpretReg16(result, val, *m_reg);
   return result;
}
      
float MB::Data::getFloat16() { 
   float result;
	uint16_t val;
	m_mem_manager->getWord(&val, m_u16_data_ptr);
   mb::types::ModbusTrans::interpretReg16(result, val, *m_reg);
   return result;
}  
      
int MB::Data::getInt() {
   int result = 0;
	uint32_t val;
	uint16_t* val_ptr = reinterpret_cast<uint16_t*>(&val);
	m_mem_manager->getDWord(&val, m_u16_data_ptr);
	mb::types::ModbusTrans::interpretReg32(result, val_ptr, *m_reg);
	return result;
}
      
unsigned int MB::Data::getUInt() {
	unsigned int result = 0;
	uint32_t val;
	uint16_t* val_ptr = reinterpret_cast<uint16_t*>(&val);
	m_mem_manager->getDWord(&val, m_u16_data_ptr);
	mb::types::ModbusTrans::interpretReg32(result, val_ptr, *m_reg);
	return result;
}
      
float MB::Data::getFloat() {
   float result = 0;
	uint32_t val;
	uint16_t* val_ptr = reinterpret_cast<uint16_t*>(&val);
	m_mem_manager->getDWord(&val, m_u16_data_ptr);
	mb::types::ModbusTrans::interpretReg32(result, val_ptr, *m_reg);
	return result;
}

// Data IMB::getData(const int addr, const int func, const int slave_id) { return Data(); }

// Data IMB::getCoilData(const std::string& name, const int slave_id) { return Data(); }
// Data IMB::getCoilData(const int addr, const int slave_id) { return Data(); }

// Data IMB::getDiscreteData(const std::string& name, const int slave_id) { return Data(); } 
// Data IMB::getDiscreteData(const int addr, const int slave_id) { return Data(); }

// Data IMB::getHoldingRegisterData(const std::string& name, const int slave_id) { return Data(); }
// Data IMB::getHoldingRegisterData(const int addr, const int slave_id) {return Data(); }

// Data IMB::getInputRegisterData(const std::string& name, const int slave_id) { return Data(); } 
// Data IMB::getInputRegisterData(const int addr, const int slave_id) { return Data(); } 


} // mb