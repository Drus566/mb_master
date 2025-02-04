#ifndef MB_H
#define MB_H

#include "IMB.h"

#include "Config.h"
#include "DataManager.h"
#include "ActionManager.h"
#include "Logger.h"

namespace mb {

using namespace mb::data;
using namespace mb::config;
using namespace mb::action;

class MB : public IMB {

public: 
	MB(const std::string& path_config);
	~MB();

	bool start() override;
   bool isConnect() override;

   void startLog() override;
   void stopLog() override;
   void startDebug() override;
   void stopDebug() override;

   IMB::ModbusData* getDataOnlyByName(const std::string& name);
   IMB::ModbusData* getData(const std::string& name, const int func, const int slave_id) override;
   IMB::ModbusData* getData(const int addr, const int func, const int slave_id) override;
   
   // Data getCoilData(const std::string& name, const int slave_id = MB_DEFAULT_SLAVE) override; 
   // Data getCoilData(const int addr, const int slave_id = MB_DEFAULT_SLAVE) override; 
 
   // Data getDiscreteData(const std::string& name, const int slave_id = MB_DEFAULT_SLAVE) override; 
   // Data getDiscreteData(const int addr, const int slave_id = MB_DEFAULT_SLAVE) override; 

   // Data getHoldingRegisterData(const std::string& name, const int slave_id = MB_DEFAULT_SLAVE) override; 
   // Data getHoldingRegisterData(const int addr, const int slave_id = MB_DEFAULT_SLAVE) override; 
 
   // Data getInputRegisterData(const std::string& name, const int slave_id = MB_DEFAULT_SLAVE) override; 
   // Data getInputRegisterData(const int addr, const int slave_id = MB_DEFAULT_SLAVE) override;

   bool f1(uint8_t *val, int slave_id, int addr, int count) override;
   bool f1(std::string &name, uint8_t *val) override;

   bool f2(uint8_t *val, int slave_id, int addr, int count) override;
   bool f2(std::string &name, uint8_t *val) override;

   bool f3(uint16_t *val, int slave_id, int addr, int count) override;
   bool f3(std::string &name, uint16_t *val) override;

   bool f4(uint16_t *val, int slave_id, int addr, int count) override;
   bool f4(std::string &name, uint16_t *val) override;

   bool f5(uint8_t val, int slave_id, int adr) override;
   bool f5(std::string &name, uint8_t val) override;

   bool f6(uint8_t *vals, int slave_id, int adr) override;
   bool f6(std::string &name, uint8_t *vals, int count) override;

   bool f15(uint16_t val, int slave_id, int adr) override;
   bool f15(std::string &name, uint16_t val) override;

   bool f16(uint16_t *vals, int slave_id, int adr, int count) override;
   bool f16(std::string &name, uint16_t *vals, int count) override;

   class Data : public ModbusData {
      public:
         Data(MemManager* mem_manager, 
              mb::types::Register* reg, 
              uint16_t* u16_data_ptr, 
              uint8_t* u8_data_ptr) : m_mem_manager(mem_manager),
                                      m_reg(reg),
                                      m_u16_data_ptr(u16_data_ptr),
                                      m_u8_data_ptr(u8_data_ptr) {}

         virtual ~Data() {}

         bool getBit() override;
         int getShort() override;
         int getUShort() override;
         float getFloat16() override;
         float getFloat() override;
         int getInt() override;
         unsigned int getUInt() override;
      
      private:
         MemManager* m_mem_manager;
         mb::types::Register* m_reg;
         uint16_t* m_u16_data_ptr;
         uint8_t* m_u8_data_ptr;
   }; // Data

private:
	Config* m_config;
   MemManager* m_mem_manager;
	DataManager* m_data_manager;
   ActionManager* m_action_manager;

	bool m_start;
}; // MB
 
} // mb

#endif // MB_H