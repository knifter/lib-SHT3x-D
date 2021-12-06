#include <Arduino.h>
#include <Wire.h>
#include <SHT3x.h>
// #include <Adafruit_SHT31.h>
#include <SPI.h>

#define PIN_SDA		GPIO_NUM_21
#define PIN_SCL		GPIO_NUM_22

SHT3X sht;

void setup()
{
	Serial.begin(115200);
	Serial.println("Start setup()");

    if(!Wire.begin(PIN_SDA, PIN_SCL, 100E3))
    {
        Serial.println("ERROR: Wire.begin()");
        while(1);
    };

    if(!sht.begin())
    {
        Serial.println("ERROR: sht.begin() failed.");
        while(1);
    };

    return;
};

void loop()
{   
    if(!sht.newMeasurement()->error)
    {
        Serial.println("Measurement failed!");
    }else{
        Serial.printf("T = %0.2f C\t\tRH = %0.2f %%\n", sht.getTemperature(), sht.getHumidity());
    };

    // or, if only one value needed:
    Serial.printf("T = %0.2f\n", sht.newMeasurement()->temperature);
    delay(1000);
};

