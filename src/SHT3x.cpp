#include "SHT3x.h"

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

bool SHT3X::begin()
{
	if(!TwoWireDevice::begin())
	{
		// ERROR("TwoWireDevice.begin() failed.");
		return false;
	};

	reset();

	uint16_t s = get_status();
	if(s == 0xFFFF)
	{
		// ERROR("SHT3X reset failed, STATUS=FFFF");
		return false;
	};
	
	return true;
};

void SHT3X::reset()
{
	write16_ML(CMD_RESET);
};

void SHT3X::setMode(Mode_t mode)
{
	_mode = mode;
};

void SHT3X::enableHeater(bool heat)
{
	if(heat)
		write16_ML(CMD_HEATER_ON);
	else
		write16_ML(CMD_HEATER_OFF);
	
};

SHT3X::measurement_t *SHT3X::newMeasurement()
{
	// in error cases let these be NaN
	measurement.temperature = NAN;
	measurement.humidity = NAN;

	// Get T+RH+checksums
	uint8_t buf[6];
	write16_ML(static_cast<uint16_t>(_mode));
  	delay(20);
	int res = read(buf, 6);
	if(res != 6)
	{
		// ERROR("I2C Read error: %d/%s", _wire.lastError(), _wire.getErrorText(_wire.lastError()));
		measurement.error = 1;
		return &measurement;
	};
	// DBG("BUF [%02x %02x %02x %02x %02x %02x]\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);

	// Check crc
	if(buf[2] != crc8(buf, 2))
	{
		// DBG("Temp CRC8 failed: %u != %u", buf[2], crc8(buf, 2));
		measurement.error = 2;
		return &measurement;
	};
	if(buf[5] != crc8(buf+3, 2))
	{
		// DBG("Hum CRC8 failed.");
		measurement.error = 3;
		return &measurement;
	};

	uint32_t rawtemp = buf[0] << 8 | buf[1];
	uint32_t rawhum = buf[3] << 8 | buf[4];
	
	// temp = (reg * 175.0f) / 65535.0f - 45.0f;
	measurement.temperature = (float) (((4375 * rawtemp) >> 14) - 4500) / 100.0f;

	// humidity = (shum * 100.0f) / 65535.0f;
  	measurement.humidity = (float) ((625 * rawhum) >> 12) / 100.0f;
	
	measurement.time = millis();
	measurement.error = 0;
	
	return &measurement;
};


//**** PROTECTED ******************************************************************************************//
uint16_t SHT3X::get_status()
{
	write16_ML(CMD_READSTATUS);
	return _status.set(read16_ML());
};

void SHT3X::clear_status()
{
	write16_ML(CMD_CLEAR_STATUS);
};

