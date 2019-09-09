#include "SI7060.h"

SI7060::SI7060(uint8_t address, TwoWire *wire):
address_(address), i2c_(wire){

}

bool SI7060::verifyPart() {
    uint8_t identity = 0;
    if (!readByte(SI7060_REG_ID, &identity)) {
        return false;
    }

    uint8_t chipID = SI7060_HI_NIBBLE(identity);

    return chipID == 0x01;
}

double SI7060::readTemperatureC() {
    uint8_t dspSigL;
    uint8_t dspSigM;

    readByte(SI7060_REG_DSPSIGM, &dspSigM);
    dspSigM = dspSigM & 0x7F;

    readByte(SI7060_REG_DSPSIGL, &dspSigL);

    uint8_t measurement;
    readByte(SI7060_REG_MEASUREMENT, &measurement);

    uint8_t config = (measurement & 0x08) | (measurement & 0x04);
    writeByte(SI7060_REG_MEASUREMENT, config);

    return 55 + ((double)(256 * dspSigM) + (float)(dspSigL - 16384)) / 160.0;
}

bool SI7060::sleep() {
    return writeByte(SI7060_REG_MEASUREMENT, 0x09);
}

bool SI7060::beginActiveMode() {
    if (!writeByte(SI7060_REG_SLEEP_TIMER_EN, 0)) {
        return false;
    }

    return writeByte(SI7060_REG_MEASUREMENT, 0x04);
}

bool SI7060::readByte(uint8_t registerAddress, uint8_t *data) {
    byte error = 0;
    i2c_->beginTransmission(address_);
    i2c_->write(registerAddress);

    error = i2c_->endTransmission();
    if (error != 0) {
        return false;
    }

    i2c_->beginTransmission(address_);
    i2c_->requestFrom(address_, (uint8_t)1);

    if (data != NULL) {
        *data = i2c_->read();
    }

    error = i2c_->endTransmission();
    
    return error == 0;
}

bool SI7060::writeByte(uint8_t registerAddress, uint8_t data) {
    i2c_->beginTransmission(address_);
    i2c_->write(registerAddress);
    i2c_->write(data);

    return i2c_->endTransmission(true) == 0;
}