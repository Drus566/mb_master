#ifndef IMB_H
#define IMB_H

#include "ModbusRegister.h"
#include "ModbusTrans.h"
#include "ModbusEnums.h"

#include <string>
#include <memory>
#include <cstdint>

#define MB_F1  1
#define MB_F2  2
#define MB_F3  3
#define MB_F4  4
#define MB_F5  5
#define MB_F6  6
#define MB_F15 15
#define MB_F16 16

namespace mb {

using namespace types;

class IMB {
public:
    class ModbusData;

    virtual ~IMB() {}
    
    static std::unique_ptr<IMB> create(const std::string& config_path);

    virtual bool start() = 0;

    virtual bool isConnect() = 0;

    virtual void startLog(char* filename) = 0;
    virtual void startLog(std::string& filename) = 0;
    virtual void startLog() = 0;

    virtual void stopLog() = 0;

    virtual void setLogMode(char* mode) = 0;
    virtual void setLogMode(const std::string& mode) = 0;

    virtual void startDebug() = 0;

    virtual void stopDebug() = 0;

    virtual bool runRequest(void* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool runRequest(void* vals, const std::string &name, int count = 1) = 0;

    virtual bool readCoil(bool* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool readCoil(bool* vals, const std::string &name, int count = 1) = 0;

    virtual bool readInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool readInt16(int* vals, const std::string &name, int count = 1) = 0;
   
    virtual bool readUInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool readUInt16(int* vals, const std::string &name, int count = 1) = 0;

    virtual bool readFloat16(float* vals, const int slave_id, const int func, const int addr, int count = 1, int precision = 1) = 0;
    virtual bool readFloat16(float* vals, const std::string &name, int count = 1) = 0;

    virtual bool readFloat32(float *vals, const int slave_id, const int func, const int addr, int precision = 1, int count = 1, RegDataOrder order = RegDataOrder::NONE) = 0;
    virtual bool readFloat32(float* vals, const std::string &name, int count = 1) = 0;

    virtual bool readInt32(int* vals, const int slave_id, const int func, const int addr, int count = 1, RegDataOrder order = RegDataOrder::NONE) = 0;
    virtual bool readInt32(int* vals, const std::string &name, int count = 1) = 0;

    virtual bool readUInt32(unsigned int* vals, const int slave_id, const int func, const int addr, int count = 1, RegDataOrder order = RegDataOrder::NONE) = 0;
    virtual bool readUInt32(unsigned int* vals, const std::string &name, int count = 1) = 0;

    virtual bool writeCoil(bool* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool writeCoil(bool* vals, const std::string &name, int count = 1) = 0;

    virtual bool writeInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool writeInt16(int* vals, const std::string &name, int count = 1) = 0;

    virtual bool writeUInt16(int* vals, const int slave_id, const int func, const int addr, int count = 1) = 0;
    virtual bool writeUInt16(int* vals, const std::string &name, int count = 1) = 0;

    virtual bool writeFloat16(float* vals, const int slave_id, const int func, const int addr, int count = 1, int precision = 1) = 0;
    virtual bool writeFloat16(float* vals, const std::string &name, int count = 1, int precision = 1) = 0;

    virtual bool writeFloat32(float* vals, const int slave_id, const int addr, int count = 1) = 0;
    virtual bool writeFloat32(float* vals, const std::string &name, int count = 1) = 0;

    virtual bool writeInt32(int* vals, const int slave_id, const int addr, int count = 1) = 0;
    virtual bool writeInt32(int* vals, const std::string &name, int count = 1) = 0;

    virtual bool writeUInt32(unsigned int* vals, const int slave_id, const int addr, int count = 1) = 0;
    virtual bool writeUInt32(unsigned int* vals, const std::string &name, int count = 1) = 0;

    virtual ModbusData* getDataOnlyByName(const std::string& name) = 0;
    virtual ModbusData* getData(const std::string& name, const int func, const int slave_id = 1) = 0;
    virtual ModbusData* getData(const int addr, const int func, const int slave_id = 1) = 0;
    
    class ModbusData {
        public:
        virtual ~ModbusData() { }

        virtual bool getBit() = 0;
        virtual int getShort() = 0;
        virtual int getUShort() = 0;
	    virtual float getFloat16() = 0;
        virtual int getInt() = 0;
        virtual unsigned int getUInt() = 0;
        virtual float getFloat() = 0;
    };
};

} // mb

#endif // IMB_H
