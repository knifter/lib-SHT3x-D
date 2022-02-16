#include "SHT4X.h"

// #define DEBUG
// #define TOOLS_LOG_DEBUG
// #include "tools-log.h"

static uint8_t crc8(const uint8_t *data, const int len) 
{
	uint8_t crc = 0xFF;

  	for (int j = len; j; --j)
  	{
		crc ^= *data++;
    	for (int i = 8; i; --i) 
		{
      		crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
    	}
	};
  	return crc;
};

bool SHT4X::begin() {
	if (!TwoWireDevice::begin()) {
		// ERROR("TwoWireDevice.begin() failed.");
		return false;
	};
	reset();
    delay(1);
	//uint32_t sn = get_serial_number();
	//if (!sn) {
	//	// ERROR("SHT4X reset failed, SerialNum=FF");
	//	return false;
	//};
	return true;
};

void SHT4X::reset() {
	write8(static_cast<uint8_t>(CMD_SOFTRESET));
};


uint32_t SHT4X::get_serial_number() {
	uint8_t buf[6];
	uint32_t SerNum;

	write8(static_cast<uint8_t>(CMD_READSERIALNUM));
	delay(1);
    int res = read(buf, 6);
	if(res != 6) {
		// ERROR("I2C Read error: %d/%s", _wire.lastError(), _wire.getErrorText(_wire.lastError()));
		Serial.printf("buf<>6: %d\n",res);
		return(0);
	};
	if( (buf[2] != crc8(buf, 2)) || (buf[5] != crc8(buf+3, 2)) ) {
		Serial.printf("Crc8\n");
		// One of tweo Crc8 in the 4-byte serial num is false
		return(0);
	}
	SerNum = static_cast <uint32_t> (buf[0]) << 24;				//Really? -YAK- shifting 32 times to get  4bytes in the right Uint32 ?
    SerNum |= static_cast<uint32_t> (buf[1]) << 16;				//What a waiste of processing time
    SerNum |= static_cast<uint32_t> (buf[3]) << 8;
    SerNum |= static_cast<uint32_t> (buf[4]);
	return(SerNum);
}

float SHT4X::getHumidity(Meas_t Mode) {
	if(!get_measurement(Mode))
		return NAN;
	return _humidity;
};

float SHT4X::getTemperature(Meas_t Mode){
	if(!get_measurement(Mode))
		return NAN;
	return _temperature;
};


//**** PROTECTED ******************************************************************************************//
bool SHT4X::get_measurement(Meas_t Mode) {
	uint8_t buf[6];
	Command_t MeasCmd=CMD_HIGHREP;

    switch(Mode) {
			case MEAS_HIGHREP: 
			  MeasCmd = CMD_HIGHREP; break;
			case MEAS_MEDREP:  
			  MeasCmd = CMD_MEDREP; break;
			case MEAS_LOWREP: 
			  MeasCmd = CMD_LOWREP; break;
			case MEAS_HEAT200S1_HIGHREP:
			  MeasCmd = CMD_HEAT200S1_HIGHREP; break;
			case MEAS_HEAT200S01_HIGHREP:
			  MeasCmd = CMD_HEAT200S01_HIGHREP; break;
			case MEAS_HEAT110S1_HIGHREP:
			  MeasCmd = CMD_HEAT110S1_HIGHREP; break;
			case MEAS_HEAT110S01_HIGHREP:
			  MeasCmd = CMD_HEAT110S01_HIGHREP; break;
			case MEAS_HEAT20S1_HIGHREP:
			  MeasCmd = CMD_HEAT20S1_HIGHREP; break;
			case MEAS_HEAT20S01_HIGHREP:
			  MeasCmd = CMD_HEAT20S01_HIGHREP; break;
	}

	write8(static_cast<uint8_t>(MeasCmd));
  	delay(10);										// Low/Med/Hi max. 1.7 / 4.5 / 8.2ms
	int res = read(buf, 6);
	if (res != 6)  {   								// ERROR("I2C Read error: %d/%s", _wire.lastError(), _wire.getErrorText(_wire.lastError()));
	    Serial.printf("Meas: buf<>6: %d\n",res);
		return false;
	};
	// Check crc
	if ( buf[2] != crc8(buf, 2)) {					//ERROR CRC8 of Temp
		// DBG("Temp CRC8 failed: %u != %u", buf[2], crc8(buf, 2));
		Serial.printf("Meas: Crc8 tmp \n");
		return false;
	};

	if ( buf[5] != crc8(buf+3, 2)) {				//ERROR CRC8 of Hum.
		// DBG("Hum CRC8 failed.");
		Serial.printf("Meas: Crc8 hum\n");
		return false;
	};

	uint32_t rawtemp = buf[0] << 8 | buf[1];
	uint32_t rawhum = buf[3] << 8 | buf[4];
	
	// temp = (rawtemp * 175.0f / 65535.0f) - 45.0f;   		Degrees Celcius!
	_temperature = (float) (((4375 * rawtemp) >> 14) - 4500) / 100.0f;            //4375 should be 4375.067; so tiny error
	// humidity = (rawhum * 125.0f) / 65535.0f) - 6.0f ;
  	_humidity = (float) (((3125 * rawhum) >> 14) - 600 ) / 100.0f;				  //3125 should be 3125.048; so tiny error

	return true;
};

