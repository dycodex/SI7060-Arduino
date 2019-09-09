#ifndef __SI7060_ARDUINO_H__
#define __SI7060_ARDUINO_H__

#include <Wire.h>
#include <Arduino.h>
#include "SI7060_registers.h"

#define SI7060_HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define SI7060_LO_NIBBLE(b) ((b) & 0x0F)

class SI7060 {
public:
    SI7060(uint8_t address = 0x31, TwoWire *wire = &Wire);

    bool verifyPart();
    double readTemperatureC();

    bool sleep();
    bool beginActiveMode();
private:
    bool readByte(uint8_t registerAddress, uint8_t *data);
    bool writeByte(uint8_t registerAddress, uint8_t data);

    uint8_t address_;
    TwoWire *i2c_;
};

#endif