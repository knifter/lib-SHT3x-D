# lib-SHT
Sensirion SHT30-D, SHT31-D, SHT35-D, SHT40/41/45 ESP32/Arduino library

This library uses the TwoWireDevice base class present in lib-TwoWireDevice

At this point the SHT3x and SHT4x libraries are still separate files. Thanks to Edwin @ uva.nl for porting the SHT3x code to SHT4x.

# Datasheet
https://www.mouser.com/datasheet/2/682/Sensirion_Humidity_Sensors_SHT3x_Datasheet_digital-971521.pdf
https://www.mouser.com/datasheet/2/682/Datasheet_SHT4x-1917879.pdf

# Sensor types:
SHT30-D Least Accurate version  
SHT31-D: Most Accurrate version over the whole range (90% RH)  
SHT35-D: slight reduced accuracy, mostly smaller accurate range (80% RH)  
SHT40: Gen 4, little more accurate, less power, smaller package, no alert, single address
SHT41/45: No specs given yet

## SHT3x
* Supply: 2.4V..5.5V
* Power Usage idle / measure/ avg(1hz): 0.2uA / 800 uA / 2uA 
* I2C @ 1 MHz
* I2C Addresses: 0x44 / 0x45
▪ Operating range: 0 … 100 %RH, -40…125 °C
* Normal range: 5..60C / 20..80% RH (all versions!)
* Alert out
* 4-pin DFN 2.5mm x 2.5mm
* 33mW Heater

### SHT30
* Typical RH Range/Accuracy: 0..100%, 3%
* Typical T Range/Accuracy: 0C..65C, 0.3C
### SHT31
* Typical RH Range/Accuracy: 0..100%, 2%
* Typical T Range/Accuracy: -40C..90C, 0.3C
### SHT35
* Typical RH Range/Accuracy: 0..100%, 3%
* Typical T Range/Accuracy: -40C-90C, 0.2C

### SHT4x
* Supply: 1.1V..3.6V
* Power Usage idle / measure/ avg(1hz): 0.08uA / 350 uA / 2.6uA 
* I2C @ 'Fast mode plus'
* I2C Addresses: 0x44 Only
▪ Operating range: 0 … 100 %RH, -40…125 °C
* Recommended Normal range: 5..60C / 20..80% RH (all versions!)
* 4-pin QFN 1.5mm x 1.5mm
* Heater
### SHT40
* Typical RH Range/Accuracy: 0..100%, 1.8% (30-60%, 2% otherwise)
* Typical T Range/Accuracy: -40C..125C, 0.2C (0..65C)
### SHT41/45
Data t.b.d. in datasheet
