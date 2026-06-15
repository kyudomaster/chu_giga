// Fine, I'll do it myself.
// MPR121 library header file.
#ifndef MPR121_H
#define MPR121_H

#include <Arduino.h>
#include <Wire.h>

class MPR121
{
public:
    MPR121(uint8_t address = 0x5A); // Default address and number of electrodes.

    bool begin(TwoWire &wire = Wire);
    
    uint16_t touched();
    
    uint16_t filterData(uint8_t electrode);
    uint8_t baseData(uint8_t electrode);

    void setThresholds(uint8_t touch, uint8_t release);
    
    void softReset();
    void resume(uint8_t ecr);
    uint8_t stop();

private:
    uint8_t _addr;
    TwoWire *_wire;

    void writeReg(uint8_t reg, uint8_t val);
    uint8_t readReg(uint8_t reg);
    uint16_t readReg16(uint8_t reg);
    void dumpRegisters();
    void dumpRegistersAnnotated();
};

#endif