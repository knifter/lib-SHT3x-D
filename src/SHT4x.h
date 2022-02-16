#ifndef __SHT4X_H
#define __SHT4X_H

#include <Arduino.h>
#include <TwoWireDevice.h>

// CONFIG
#define SHT4X_ADDRESS_DEFAULT   (0x44)
#define SHT4X_ADDRESS_ALT		(0x45)

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with CCS811 gas sensor chips
*/
/**************************************************************************/
class SHT4X : public TwoWireDevice 
{
	public:
		//constructors
		SHT4X(TwoWire& wire, const uint8_t addr = SHT4X_ADDRESS_DEFAULT)  : TwoWireDevice(wire, addr) {};
		SHT4X(const uint8_t addr = SHT4X_ADDRESS_DEFAULT) : TwoWireDevice(addr) {};
        ~SHT4X() {};

		bool begin();
		void reset();
		uint32_t get_serial_number();
		
		typedef enum : uint8_t {
			MEAS_HIGHREP 	= 1,
			MEAS_MEDREP 	= 2,
			MEAS_LOWREP		= 3,
			MEAS_HEAT200S1_HIGHREP = 4,
			MEAS_HEAT200S01_HIGHREP = 5,
			MEAS_HEAT110S1_HIGHREP = 6,
			MEAS_HEAT110S01_HIGHREP = 7,
			MEAS_HEAT20S1_HIGHREP = 8,
			MEAS_HEAT20S01_HIGHREP = 9,
		} Meas_t;

		float getTemperature(Meas_t Mode);
		float getHumidity(Meas_t Mode);
		
    protected:
		typedef enum : uint8_t {
			CMD_HIGHREP = 0xFD,						  	  	//Hi precision measurement
			CMD_MEDREP = 0xF6,								//Medium precision measurement
			CMD_LOWREP = 0xE0,								//Low precision measurement
			CMD_READSERIALNUM	 = 0x89,
			CMD_SOFTRESET		 = 0x94,
			CMD_HEAT200S1_HIGHREP  = 0x39,					//Heater 200mW with 1 second, take Hi precision meas, then deactive heater
			CMD_HEAT200S01_HIGHREP = 0x32,
			CMD_HEAT110S1_HIGHREP = 0x2F,					
			CMD_HEAT110S01_HIGHREP = 0x24,    		    
			CMD_HEAT20S1_HIGHREP = 0x1E,			   		
			CMD_HEAT20S01_HIGHREP = 0x16				
		} Command_t;
		
        // Methods
		bool get_measurement(Meas_t Mode);

        // Other member variables
		float _temperature = NAN;
		float _humidity = NAN;
		
	private:
		SHT4X(const SHT4X&);
		SHT4X& operator=(const SHT4X&);
};

#endif //__SHT4X_H
