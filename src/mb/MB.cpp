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
	initLog();

	Logger::Instance()->log(LogLevel::INFO, "============== Start MB ==============");

	if (m_start) {
		Logger::Instance()->log(LogLevel::INFO, "Already started");
		return false;
	}

	if (!m_data_manager->start()) return false;

	Logger::Instance()->log(LogLevel::INFO, "Modbus data success initialized");

	if (!m_action_manager->start()) return false;

	Logger::Instance()->log(LogLevel::INFO, "Modbus master success initialized");

	m_start = true;
	return true;
}

bool MB::isConnect() { return m_action_manager->isConnect(); }

void MB::startLog(char* filename) {
	std::string file(filename);
	m_action_manager->setLog(true);
	Logger::Instance(file)->setActive(true);
}

void MB::startLog(std::string& filename) {
	m_action_manager->setLog(true);
	Logger::Instance(filename)->setActive(true);
}

void MB::startLog() {
	m_action_manager->setLog(true);
	Logger::Instance()->setActive(true);
}

void MB::stopLog() {
	m_action_manager->setLog(false);
	Logger::Instance()->setActive(false); 
}

void MB::setLogMode(char *mode) {
	std::string mode_str(mode);
	Logger::setLogMode(mode_str);
}

void MB::setLogMode(const std::string &mode) { Logger::setLogMode(mode); }

void MB::startDebug() { m_action_manager->setDebug(true); }

void MB::stopDebug() { m_action_manager->setDebug(false); }

bool MB::runRequest(void *vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (count < 1) return false;
	return m_action_manager->handleDirectRequest(vals, slave_id, func, addr, count);
}

// count = 1 всегда, при любом типе данных coil word dword.
bool MB::runRequest(void *vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg) {
		if (!isCoilFunc(reg->function) && isDwordDataType(reg->reg_info.data_type) && count < 2) count = 2;
		result = m_action_manager->handleDirectRequest(vals, reg->slave_id, reg->function, reg->address, count);
	}
	return result;
}

bool MB::readCoil(bool* vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (!isReadCoilFunc(func)) return false;
	return runRequest(vals, slave_id, func, addr, count);
}

bool MB::readCoil(bool* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && isReadCoilFunc(reg->function)) {
		result = m_action_manager->handleDirectRequest(vals, reg->slave_id, reg->function, reg->address, count);
	}
	return result;
}

bool MB::readInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (!isReadWordFunc(func)) return false;

	int16_t new_vals[count];
	bool result = runRequest(vals, slave_id, func, addr, count);
	for (int i = 0; i < count; i++) {
		vals[i] = new_vals[i];
	}
	return result;
}

bool MB::readInt16(int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::INT16 && isReadWordFunc(reg->function)) {
		int16_t new_vals[count];
		result = m_action_manager->handleDirectRequest(new_vals, reg->slave_id, reg->function, reg->address, count);
		if (result) {
			for (int i = 0; i < count; i++) {
				vals[i] = new_vals[i];
			}
		}
	}
	return result;
}

bool MB::readUInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (!isReadWordFunc(func)) return false;

	uint16_t new_vals[count];
	bool result = runRequest(vals, slave_id, func, addr, count);
	for (int i = 0; i < count; i++) {
		vals[i] = new_vals[i];
	}
	return result;
}

bool MB::readUInt16(int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::UINT16 && isReadWordFunc(reg->function)) {
		uint16_t new_vals[count];
		result = m_action_manager->handleDirectRequest(new_vals, reg->slave_id, reg->function, reg->address, count);
		if (result) {
			for (int i = 0; i < count; i++) {
				vals[i] = new_vals[i];
			}
		}
	}
	return result;
}

bool MB::readFloat16(float* vals, const int slave_id, const int func, const int addr, int count = 1, int precision = 1) {
	if (!isReadWordFunc(func)) return false;

	int16_t new_vals[count];
	bool result = runRequest(new_vals, slave_id, func, addr, count);
	if (result) {
		for (int i = 0; i < count; i++) {
			ModbusTrans::setPrecisionFloat16(&new_vals[i], precision);
			vals[i] = new_vals[i];
		}
	}
	return result;
}

bool MB::readFloat16(float* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::FLOAT16 && isReadWordFunc(reg->function)) {
		int16_t new_vals[count];
		result = m_action_manager->handleDirectRequest(new_vals, reg->slave_id, reg->function, reg->address, count);
		if (result) {
			for (int i = 0; i < count; i++) {
				ModbusTrans::setPrecisionFloat16(&new_vals[i], reg->reg_info.precision);
				vals[i] = new_vals[i];
			}
		}
	}
	return result;
}

bool MB::readFloat32(float* vals, const int slave_id, const int func, const int addr, int precision = 2, int count = 1, RegDataOrder order = RegDataOrder::NONE) {
	if (order == RegDataOrder::NONE) order = m_data_manager->getDataOrder();

	int word_count = count * 2;
	uint16_t word_vals[word_count];
	bool result = runRequest(word_vals, slave_id, func, addr, word_count);
	if (result) {
		for (int i = 0, j = 0; i < count; i++, j+=2) {
			ModbusTrans::interpretReg32(*(vals + i), word_vals + j, RegDataType::FLOAT32, order, precision);
		}
	}
	return result;
}

bool MB::readFloat32(float* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::FLOAT32 && isReadWordFunc(reg->function)) {
		int word_count = count * 2;
		uint16_t word_vals[word_count];
		result = m_action_manager->handleDirectRequest(word_vals, reg->slave_id, reg->function, reg->address, word_count);
		if (result) {
			for (int i = 0, j = 0; i < count; i++, j+=2) {
				ModbusTrans::interpretReg32(*(vals + i), word_vals + j, RegDataType::FLOAT32, reg->reg_info.order, reg->reg_info.precision);
			}
		}
	}
	return result;
}

bool MB::readInt32(int* vals, const int slave_id, const int func, const int addr, int count = 1, RegDataOrder order = RegDataOrder::NONE) {
	if (order == RegDataOrder::NONE) order = m_data_manager->getDataOrder();

	int word_count = count * 2;
	uint16_t word_vals[word_count];
	bool result = runRequest(word_vals, slave_id, func, addr, word_count);
	if (result) {
		for (int i = 0, j = 0; i < count; i++, j+=2) {
			ModbusTrans::interpretReg32(*(vals + i), word_vals + j, RegDataType::INT32, order, -1);
		}
	}
	return result;
}

bool MB::readInt32(int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::INT32 && isReadWordFunc(reg->function)) {
		int word_count = count * 2;
		uint16_t word_vals[word_count];
		result = m_action_manager->handleDirectRequest(word_vals, reg->slave_id, reg->function, reg->address, word_count);
		if (result) {
			for (int i = 0, j = 0; i < count; i++, j+=2) {
				ModbusTrans::interpretReg32(*(vals + i), word_vals + j, RegDataType::INT32, reg->reg_info.order, reg->reg_info.precision);
			}
		}
	}
	return result;
}

bool MB::readUInt32(unsigned int* vals, const int slave_id, const int func, const int addr, int count = 1, RegDataOrder order = RegDataOrder::NONE) {
	if (order == RegDataOrder::NONE) order = m_data_manager->getDataOrder();

	int word_count = count * 2;
	uint16_t word_vals[word_count];
	bool result = runRequest(word_vals, slave_id, func, addr, word_count);
	if (result) {
		for (int i = 0, j = 0; i < count; i++, j+=2) {
			ModbusTrans::interpretReg32(*(vals + i), word_vals + j, RegDataType::UINT32, order, -1);
		}
	}
	return result;
}

bool MB::readUInt32(unsigned int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::UINT32 && isReadWordFunc(reg->function)) {
		int word_count = count * 2;
		uint16_t word_vals[word_count];
		result = m_action_manager->handleDirectRequest(word_vals, reg->slave_id, reg->function, reg->address, word_count);
		if (result) {
			for (int i = 0, j = 0; i < count; i++, j+=2) {
				ModbusTrans::interpretReg32(*(vals + i), word_vals + j, RegDataType::UINT32, reg->reg_info.order, reg->reg_info.precision);
			}
		}
	}
	return result;
}

bool MB::writeCoil(bool* vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (!isWriteCoilOrCoilsFunc(func)) return false;
	return runRequest(vals, slave_id, func, addr, count);
}

bool MB::writeCoil(bool* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && isWriteCoilOrCoilsFunc(reg->function)) {
		result = m_action_manager->handleDirectRequest(vals, reg->slave_id, reg->function, reg->address, count);
	}
	return result;
}

bool MB::writeInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (!isWriteWordOrWordsFunc(func)) return false;
	
	int16_t new_vals[count];
	for (int i = 0; i < count; i++) {
		new_vals[i] = static_cast<int16_t>(vals[i]);
	}

	return runRequest(vals, slave_id, func, addr, count);
}

bool MB::writeInt16(int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::INT16 && isWriteWordOrWordsFunc(reg->function)) {
		int16_t new_vals[count];
		for (int i = 0; i < count; i++) {
			new_vals[i] = static_cast<int16_t>(vals[i]);
		}
		result = m_action_manager->handleDirectRequest(new_vals, reg->slave_id, reg->function, reg->address, count);
	}
	return result;
}

bool MB::writeUInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) {
	if (!isWriteWordOrWordsFunc(func)) return false;
	
	uint16_t new_vals[count];
	for (int i = 0; i < count; i++) {
		new_vals[i] = static_cast<uint16_t>(vals[i]);
	}

	return runRequest(vals, slave_id, func, addr, count);
}

bool MB::writeUInt16(int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::INT16 && isWriteWordOrWordsFunc(reg->function)) {
		uint16_t new_vals[count];
		for (int i = 0; i < count; i++) {
			new_vals[i] = static_cast<uint16_t>(vals[i]);
		}
		result = m_action_manager->handleDirectRequest(new_vals, reg->slave_id, reg->function, reg->address, count);
	}
	return result;
}

// Изменяет значение vals
bool MB::writeFloat16(float* vals, const int slave_id, const int func, const int addr, int count = 1, int precision = 1) {
	if (!isWriteWordOrWordsFunc(func)) return false;
	
	int16_t new_vals[count];
	for (int i = 0; i < count; i++) {
		ModbusTrans::setPrecisionFloat16ForWrite(vals[i], precision);
		new_vals[i] = static_cast<int16_t>(vals[i]);
	}

	return runRequest(vals, slave_id, func, addr, count);
}

bool MB::writeFloat16(float* vals, const std::string &name, int count = 1, int precision = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::FLOAT16 && isWriteWordOrWordsFunc(reg->function)) {
		int16_t new_vals[count];
		for (int i = 0; i < count; i++) {
			ModbusTrans::setPrecisionFloat16ForWrite(vals[i], precision);
			new_vals[i] = static_cast<int16_t>(vals[i]);
		}
		result = m_action_manager->handleDirectRequest(new_vals, reg->slave_id, reg->function, reg->address, count);
	}
	return result;
}


bool MB::writeFloat32(float* vals, const int slave_id, const int addr, int count = 1) {	
	int word_count = count * 2;
	bool result = runRequest(vals, slave_id, FuncNumber::WRITE_MULTIPLE_WORDS, addr, word_count);
	return result;
}

bool MB::writeFloat32(float* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::FLOAT32 && reg->function == FuncNumber::WRITE_MULTIPLE_WORDS) {
		int word_count = count * 2;
		result = m_action_manager->handleDirectRequest(vals, reg->slave_id, reg->function, reg->address, word_count);
	}
	return result;
}

bool MB::writeInt32(int* vals, const int slave_id, const int addr, int count = 1) {	
	int word_count = count * 2;
	bool result = runRequest(vals, slave_id, FuncNumber::WRITE_MULTIPLE_WORDS, addr, word_count);
	return result;
}

bool MB::writeInt32(int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::INT32 && reg->function == FuncNumber::WRITE_MULTIPLE_WORDS) {
		int word_count = count * 2;
		result = m_action_manager->handleDirectRequest(vals, reg->slave_id, FuncNumber::WRITE_MULTIPLE_WORDS, reg->address, word_count);
	}
	return result;
}

bool MB::writeUInt32(unsigned int* vals, const int slave_id, const int addr, int count = 1) {	
	int word_count = count * 2;
	bool result = runRequest(vals, slave_id, FuncNumber::WRITE_MULTIPLE_WORDS, addr, word_count);
	return result;
}

bool MB::writeUInt32(unsigned int* vals, const std::string &name, int count = 1) {
	if (count < 1) return false;

	bool result = false;
	Register *reg = m_data_manager->findRegOnlyByName(name);

	if (reg && reg->reg_info.data_type == RegDataType::UINT32 && reg->function == FuncNumber::WRITE_MULTIPLE_WORDS) {
		int word_count = count * 2;
		result = m_action_manager->handleDirectRequest(vals, reg->slave_id, FuncNumber::WRITE_MULTIPLE_WORDS, reg->address, word_count);
	}
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

void MB::initLog() {
	if (m_config->log()) {
		std::string log_file = m_config->logFile();
		if (!log_file.empty()) startLog(log_file);
		else startLog();
	}

	std::string log_mode = m_config->logMode();
	if (!log_mode.empty()) setLogMode(log_mode);
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