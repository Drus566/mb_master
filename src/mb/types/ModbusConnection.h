#ifndef MODBUS_CONNECTION_H
#define MODBUS_CONNECTION_H

#include <string>

namespace mb {
namespace types {

/** @brief типы данных регистров */
enum class ModbusConnectionType {
	NONE,
   RTU,
   ETH
};

struct ModbusConnection {
	ModbusConnectionType type;
	int response_timeout;
	
	struct {
		std::string serial_port;
		int baudrate;
		int stop_bit;
		int data_bits;
		char parity;
	} rtu;

	struct {
		std::string ip;
		int port;
	} eth;
};


// преобразование enum в строку
// inline const char* RegDataTypeToString(RegDataType e) noexcept {
//     switch(e) {
//         case RegDataType::UINT16:  return "UINT16";
//         case RegDataType::INT16:   return "INT16";
//         case RegDataType::FLOAT16: return "FLOAT16";
//         case RegDataType::FLOAT32: return "FLOAT32";
//         case RegDataType::INT32:   return "INT32";
//         case RegDataType::UINT32:  return "UINT32";
//     }
//     return "";
// };

// inline bool isFloatDataType(RegDataType& type) { return (type == RegDataType::FLOAT16 || type == RegDataType::FLOAT32); }

} // types
} // mb

#endif // MODBUS_CONNECTION_H