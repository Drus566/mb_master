#ifndef MB_MODBUS_MASTER_H
#define MB_MODBUS_MASTER_H

#define LOG_BUFFER_SIZE 256

#include "ModbusConnection.h"
#include "Logger.h"
#include "modbus.h"

#include <cstdint>

namespace mb {
namespace action {

using namespace mb::types;

class ModbusMaster  {
public:
    /** @brief Constructor */
    ModbusMaster(ModbusConnection& connection);
    ~ModbusMaster();

    bool setContext();
    bool setContext(const ModbusConnection& connection);

    /** @brief set debug */
    void setDebug(int flag);
    
    /** @brief set log */
    void setLog(int flag);

    /** @brief set log buffer */
    void setLogBuffer(char* buffer);

    /** @brief connect on current context */
    bool connect();

    /** @brief set byte timeout */
    bool setByteTimeout();

    /** @brief set byte timeout */
    bool setByteTimeout(int sec, int usec);

    /** @brief get byte timeout */
    bool getByteTimeout(uint32_t* to_sec, uint32_t* to_usec);

    /** @brief set response timeoute */
    bool setResponseTimeout();

    /** @brief set response timeoute */
    bool setResponseTimeout(int tv_sec, int tv_usec);

    /** @brief get response timeoute */
    bool getResponseTimeout(uint32_t* tv_sec, uint32_t* tv_usec);

    /** @brief set slave id */
    bool setSlave(int slave_id);

    /** @brief get slave id */
    bool getSlave(int& slave_id);

    /** @brief read bits */
    bool readBits(int slave_id, int addr, int count, uint8_t* dest);

    /** @brief read input bits */
    bool readInputBits(int slave_id, int addr, int count, uint8_t* dest);

    /** @brief read registers */
    bool readRegisters(int slave_id, int addr, int count, uint16_t* dest);

    /** @brief read input registers */
    bool readInputRegisters(int slave_id, int addr, int count, uint16_t* dest);

    /** @brief write bit */
    bool writeBit(int slave_id, int addr, int bit);

    /** @brief write bits */
    bool writeBits(int slave_id, int addr, int number, const uint8_t* bits);

    /** @brief write register */
    bool writeRegister(int slave_id, int addr, const uint16_t value);

    /** @brief write registers */
    bool writeRegisters(int slave_id, int addr, int number, const uint16_t* values);

    /** @brief flush modbus buffer */
    bool flush();

    /** @brief close modbus connection on current context */
    void close();

    /** @brief free modbus current context */
    void free();

    /** @brief get error message */
    std::string getStrError();

    /** @brief get error code */
    int getError();

    /** @brief set recovery mode */
    int setErrorRecovery(bool mode) {
        bool result = true;
        if (mode) result = modbus_set_error_recovery(m_ctx, MODBUS_ERROR_RECOVERY_LINK);
        else result = modbus_set_error_recovery(m_ctx, MODBUS_ERROR_RECOVERY_NONE);
        return result;
    }

private:
    ModbusConnection& m_connection;
    modbus_t* m_ctx;
    char m_tx_log_buffer[LOG_BUFFER_SIZE];
    char m_rx_log_buffer[LOG_BUFFER_SIZE];  
    char m_error_log_buffer[LOG_BUFFER_SIZE];
};

} // action
} // mb

#endif // MB_MODBUS_MASTER_H
