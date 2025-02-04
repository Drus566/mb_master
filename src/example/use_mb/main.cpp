#include "IMB.h"

#include <iostream>
#include <stdio.h>
#include <thread>

int main(void) {
	std::string path_config = "example.ini";
	std::unique_ptr<mb::IMB> mb = mb::IMB::create(path_config);
	bool result = mb->start();
	if (!result) { 
		std::cout << "mb start error" << std::endl;
		return 0;
	}

	mb::IMB::ModbusData* d = mb->getData(300,1);
	mb::IMB::ModbusData* d1 = mb->getData(301,1);
	mb::IMB::ModbusData* d2 = mb->getData(302,1);
	// mb::IMB::IData* d3 = mb->getData(303,1);

	// worked
	// mb::IMB::IData* d4 = mb->getDataOnlyByName("r1");
	// mb::IMB::IData* d5 = mb->getDataOnlyByName("r2");
	// mb::IMB::IData* d6 = mb->getDataOnlyByName("r3");

	mb::IMB::ModbusData* d4 = mb->getData(300,3);
	mb::IMB::ModbusData* d5 = mb->getData(301,3);
	mb::IMB::ModbusData* d6 = mb->getData(302,3);
	mb::IMB::ModbusData* d7 = mb->getData(304,3);
	mb::IMB::ModbusData* d8 = mb->getData(306,3);
	mb::IMB::ModbusData* d9 = mb->getData(308,3);

	mb::IMB::ModbusData* d10 = mb->getData(310,3);
	mb::IMB::ModbusData* d11 = mb->getData(312,3);
	mb::IMB::ModbusData* d12 = mb->getData(314,3);
	mb::IMB::ModbusData* d13 = mb->getData(316,3);

	// mb->writeRaw(uint16_t* vals, int slave_id, int adr, int func, int count);

	// bool mb->readF1(uint8_t* val, int slave_id, int addr, int count);
	// bool mb->readF1("NAME", uint8_t* val);

	// bool mb->readF2(uint8_t* val, int slave_id, int addr, int count);
	// bool mb->readF2("NAME", uint8_t* val)

	// bool mb->readF3(uint16_t* val, int slave_id, int addr, int count);
	// bool mb->readF3("NAME", uint16_t* val);

	// bool mb->readF4(uint16_t *val, int slave_id, int addr, int count);
	// bool mb->readF4("NAME", uint16_t *val);

	// // name, val
	// bool mb->writeF5(uint8_t val, int slave_id, int adr);
	// bool mb->writeF5("CCS_START", uint8_t val);
	// // name, u8[], count;
	// bool mb->writeF6(uint8_t* vals, int slave_id, int adr);
	// bool mb->writeF6("CCS_START", uint8_t *vals, count);

	// bool mb->writeF15(uint16_t val, int slave_id, int adr);
	// bool mb->writeF15("name", uint16_t val);

	// bool mb->writeF16(uint16_t *vals, int slave_id, int adr, int count);
	// bool mb->writeF16("name", uint16_t *vals, count);

	// value, slave, addr
	// slave, addr
	// value, addr
	// d10->writeRegValue(val, addr, slave = default_slave);
	// d10->writeReg(adr, slave = default_adr);
	// d10->writeRegsValue(val, addr, slave = default_slave);
	// d10->writeRegs(adr, slave = default_adr);

	// mb::IMB::IData* d7 = mb->getData(303,3);

	// if (g == nullptr) std::cout << "GGWP" << std::endl;
	// g->getBit();

	while(true) {
		// std::cout << "**** DATA ****" << std::endl;
		// std::cout << "Func 1:" << std::endl;
		// std::cout << "300: " << d->getBit() << std::endl;
		// std::cout << "301: " << d1->getBit() << std::endl;
		// std::cout << "302: " << d2->getBit() << std::endl;
		// // // std::cout << "303: " << d3->getBit() << std::endl;
		// std::cout << "Func 3:" << std:: endl;
		// std::cout << "300: " << d4->getShort() << std::endl;
		// std::cout << "301: " << d5->getFloat16() << std::endl;
		// std::cout << "302: " << d6->getFloat16() << std::endl;
		// std::cout << "304: " << d7->getInt() << std::endl;
		// std::cout << "306: " << d8->getInt() << std::endl;
		// printf("308: %.5f\n", d9->getFloat());

		// std::cout << "310: " << d10->getUInt() << std::endl;
		// std::cout << "312: " << d11->getUInt() << std::endl;
		// std::cout << "314: " << d12->getUInt() << std::endl;
		// std::cout << "316: " << d13->getUInt() << std::endl;
		
		// // std::cout << "308: " << d9->getFloat() << std::endl;

		// std::cout << "**************" << std::endl;
		// std::cout << "PRIVET" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	// g.getBit();
	
	// mb::MB mb(path_config);
	// mb::MB::Data data;
	// data.getBit();
	// mb.start();
	// mb::IMB::IData data;
	// mb::Data d;
	// g.getBit();

	// mb::config::Config config(path_config);
	// mb::data::DataManager dm(config);
	// bool result = mb.start();
	// if (result) std::cout << "Mb started " << std::endl;
	
	// mb::Data data = mb->getCoilData(1);
	// data.getBit();
	// mb->getCoil(123,1);
	// mb->getData(123,3,1);

	return 0;
}

// class {
// 	public:
// 		errors = mb.getCoil(1);
// 		// mb->getData("asd",3,1);
	// Напрямую
	// mb->getData("")

// 	private;
// 	mb::Data errors;
// 	mb::Data warnings;

// }