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

   void startLog(char* filename) override;
   void startLog(std::string& filename) override;
   void startLog() override;
   void stopLog() override;
   void setLogMode(char *mode) override;
   void setLogMode(const std::string &mode) override;
   void startDebug() override;
   void stopDebug() override;

   bool runRequest(void *vals, const int slave_id, const int func, const int addr, int count) override;
   bool runRequest(void *vals, const std::string &name, int count) override;
   
   bool readFloat32(float* vals, const int slave_id, const int func, const int addr, int precision, int count, RegDataOrder order) override;
   bool readFloat32(void *vals, const std::string &name, int count) override;

   IMB::ModbusData* getDataOnlyByName(const std::string& name);
   IMB::ModbusData* getData(const std::string& name, const int func, const int slave_id) override;
   IMB::ModbusData* getData(const int addr, const int func, const int slave_id) override;
   
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

   void initLog();
}; // MB
 
} // mb

#endif // MB_H