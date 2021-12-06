#ifndef __SHT3X_H
#define __SHT3X_H

#include <Arduino.h>
#include <TwoWireDevice.h>

// CONFIG
#define SHT3X_ADDRESS_DEFAULT      (0x44)
#define SHT3X_ADDRESS_ALT	       (0x45)

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with CCS811 gas sensor chips
*/
/**************************************************************************/
class SHT3X : public TwoWireDevice 
{
	public:
		//constructors
		SHT3X(TwoWire& wire, const uint8_t addr = SHT3X_ADDRESS_DEFAULT)  : TwoWireDevice(wire, addr) {};
		SHT3X(const uint8_t addr = SHT3X_ADDRESS_DEFAULT) : TwoWireDevice(addr) {};
        ~SHT3X() {};

		bool begin();
		void reset();

		typedef struct
		{
			int error;
			time_t time;
			float temperature;
			float humidity;
		} measurement_t;
		measurement_t measurement;

		enum class Mode_t : uint16_t
		{
			MODE_STRETCH_HIGHREP 	= 0x2C06,
			MODE_STRETCH_MEDREP 	= 0x2C0D,
			MODE_STRETCH_LOWREP 	= 0x2C10,
			MODE_HIGHREP 			= 0x2400,
			MODE_MEDREP 			= 0x240B,
			MODE_LOWREP 			= 0x2416
		};
		void setMode(Mode_t mode);

		void enableHeater(bool);

		measurement_t *newMeasurement();
		float getTemperature() { return measurement.temperature; };
		float getHumidity() { return measurement.humidity; };
		
    protected:
		typedef enum : uint16_t
		{
			CMD_RESET			= 0x303A,
			CMD_FETCH			= 0xE000,
			CMD_ART				= 0x2B32,
			CMD_BREAK			= 0x3093,
			CMD_HEATER_ON		= 0x306D,
			CMD_HEATER_OFF		= 0x3066,
			CMD_READSTATUS		= 0xF32D,
			CMD_CLEAR_STATUS	= 0x3041
		} Command_t;

		// The status register
        typedef struct {

			// 0: No alter pending
			// 1: at least one pending alert
			uint8_t ALERT: 1;

            // reserved : 1

			// 0: OFF
			// 1: ON
            uint8_t HEATER: 1;

            // reserved : 1

			// 0: No alert
			// 1: alert
            uint8_t RH_TRACKING_ALERT: 1;
            uint8_t T_TRACKING_ALERT: 1;

            // reserved : 5

			// 0: no reset detected since clear
			// 1: reset detected (hard, soft or supply)
            uint8_t SYSRESET: 1;

            // reserved : 2

			// 0: Last command executed successfully
			// 1: Last command failed
			uint8_t COMMAND : 1;

            // 0: Checksum of last write transfer was correct
            // 1: Checksum of last write transfer failed
            uint8_t CHECKSUM: 1;

			bool failed: 1;
            uint16_t set(uint16_t data)
			{
				// <alert><res><heat><res>
				ALERT = (data >> 15);
				HEATER = (data >> 13);

				// <rta:1><tta:1><res><res>
				RH_TRACKING_ALERT = (data >> 11);
				T_TRACKING_ALERT = (data >> 10);

				// <res><res><res><sysreset>
				SYSRESET = (data >> 4);

				// <res><res><command><chksum>
				COMMAND = (data >> 1);
				CHECKSUM = (data >> 0);

				failed = data == 0xFFFF;

				return data;
            };
        } status_t;
        status_t _status;
		uint16_t get_status();
		void clear_status();

        // Methods

        // Other member variables
		Mode_t _mode = Mode_t::MODE_HIGHREP;

	private:
		SHT3X(const SHT3X&);
		SHT3X& operator=(const SHT3X&);
};

#endif //__SHT3X_H
