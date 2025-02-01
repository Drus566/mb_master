#include "RegManager.h"

#include <iostream>

namespace mb {
namespace data {

bool RegManager::parseReadReg(const bool is_describe, const std::string& reg_str, int& address, RegisterInfo& reg_info) {
	bool result = false;
	char delimeter = ',';
	char precision_delimeter = '_';

	std::vector<std::string> tokens = split(reg_str, delimeter);
	// is word
	if (tokens.size() > 1) {
		for (auto it = tokens.begin(); it != tokens.end(); ++it) {
			*it = trim(*it);
			*it = toUpperCase(*it);

			// parse address
			if (isNumber(*it)) {
				address = std::stoi(*it);
				result = true;
			}
			else {
				// parse data_type
				if (getRegDataTypeFromStr(reg_info.data_type, *it) && isFloatDataType(reg_info.data_type)) {
					std::vector<std::string> float_tokens = split(*it, precision_delimeter);
					if (float_tokens.size() > 1) {
						std::string precision_str = float_tokens.at(1);
						if (isNumber(precision_str)) reg_info.precision = std::stoi(precision_str);
					}
				}
				// parse data_order
				else getRegDataOrderFromStr(reg_info.order, *it);
			}
		}
	}
	// is coil
	else if (isNumber(reg_str)) {
		address = std::stoi(reg_str);
		result = true;			
	}
	return result;
}

// TODO: реализация функции записи
// bool RegManager::parseWriteFunc(const std::string& reg_str, int& address, FuncNumber func, RegisterInfo* reg_info) {
// 	bool result = false;
// 	char delimeter = ',';
// 	std::vector<std::string> tokens;
	
// 	// parse multiple write func
// 	// if (isWriteMultipleFunc(func)) {
// 	// 	tokens = split(reg_str, delimeter);
// 	// 	if (isNumber(tokens[0]) || isNumber(tokens[1])) {
// 	// 		address = std::stoi(*it);
// 	// 	}
		
// 	// 	if (tokens.size() > 2) {
// 	// 		for (auto it = tokens.begin(); it != tokens.end(); ++it) {

// 	// 		}
// 	// 	}
// 	// 	else if (!tokens.empty()) {
			
// 	// 	}
// 	// }
// 	// parse single write func
// 	tokens = split(reg_str, delimeter);
// 	if (tokens.size() > 1) {
// 		// check address
// 		if (isNumber(tokens[0])) {
// 			reg_str = 
// 		}
// 		// check value
// 		if (isNumber(tokens[1])) {

// 		}
// 		// de
// 		else {

// 		}

// 		for (auto it = tokens.begin(); it != tokens.end(); ++it) {
// 			*it = trim(*it);
// 			*it = toUpperCase(*it);

// 			// parse address
// 			if (isNumber(*it)) {
// 				address = std::stoi(*it);
// 				result = true;
// 			}
// 		}
// 	}
	
// 	return result;
// }

bool RegManager::addReg(const bool is_describe, const int slave_id, const FuncNumber func, const std::string& name, std::string& reg_str) {
	bool result = false;
	
	char delimeter = ',';
	char precision_delimeter = '_';

	int address;
	int val = 0;
	reg_str = trim(reg_str);

	RegisterInfo reg_info;

	// parse read func
	if (isReadFunc(func)) result = parseReadReg(is_describe, reg_str, address, reg_info);
	
	// parse write func
	// else result = parseWriteFunc()

	if (result) { 
		if (is_describe) m_describe_regs.emplace_front(address, reg_info, name, slave_id, func); 
		else if (isReadFunc(func)) m_regs.emplace_front(address, reg_info, name, slave_id, func);
	}

	return result;
}

Register* RegManager::addReadReg(const int address, const int slave_id, const FuncNumber func) {
	RegisterInfo reg_info; 
	m_regs.emplace_front(address, reg_info, "", slave_id, func);
	auto it = m_regs.begin();
	Register* reg = &(*it);
	return reg;
}

std::forward_list<Register>& RegManager::getReadRegs() { return m_regs; }

} // data
} // mb