// MPR121 Jasper version!

#include "MPR121.h"
#include <stdint.h>

#define TOUCH_THRESHOLD_BASE 22
#define RELEASE_THRESHOLD_BASE 15

#define MPR121_SOFT_RESET_REG 0x80

#define MPR121_TOUCH_STATUS_REG 0x00
#define MPR121_FILTERED_DATA_REG 0x04
#define MPR121_BASELINE_REG 0x1E

// Other registries
#define MPR121_MHD_RISING_REG 0x2B
#define MPR121_NHD_RISING_REG 0x2C
#define MPR121_NCL_RISING_REG 0x2D
#define MPR121_FDL_RISING_REG 0x2E

#define MPR121_MHD_FALLING_REG 0x2F
#define MPR121_NHD_FALLING_REG 0x30
#define MPR121_NCL_FALLING_REG 0x31
#define MPR121_FDL_FALLING_REG 0x32

#define MPR121_NHD_TOUCHED_REG 0x33
#define MPR121_NCL_TOUCHED_REG 0x34
#define MPR121_FDL_TOUCHED_REG 0x35

#define MPR121_TOUCH_THRESHOLD_REG 0x41
#define MPR121_RELEASE_THRESHOLD_REG 0x42

#define MPR121_DEBOUNCE_REG 0x5B

#define MPR121_AFE1_REG 0x5C
#define MPR121_AFE2_REG 0x5D
#define MPR121_ECR_REG 0x5E

#define MPR121_AUTO_CONFIG_CONTROL_0_REG 0x7B
#define MPR121_AUTO_CONFIG_CONTROL_1_REG 0x7C
#define MPR121_USL_REG 0x7D
#define MPR121_LSL_REG 0x7E
#define MPR121_TL_REG 0x7F

MPR121::MPR121(uint8_t address) {
    _addr = address;
}

bool MPR121::begin(TwoWire &wire) 
{
    _wire = &wire;
    _wire->begin();

    softReset(); // Defaults electrodes to stop.
    delay(1);

    writeReg(MPR121_ECR_REG, 0x00);
    delay(5);

    uint8_t c = readReg(MPR121_AFE2_REG);
    if (c != 0x24)
    {
        Serial.println("Config 2 not default");
        return false;
    }

    // Touchpad baseline filter
    // rising: quick rising
    writeReg(MPR121_MHD_RISING_REG, 1);
    writeReg(MPR121_NHD_RISING_REG, 1);
    writeReg(MPR121_NCL_RISING_REG, 1);
    writeReg(MPR121_FDL_RISING_REG, 1);

    // falling: slow falling
    writeReg(MPR121_MHD_FALLING_REG, 1);
    writeReg(MPR121_NHD_FALLING_REG, 1);
    writeReg(MPR121_NCL_FALLING_REG, 6);
    writeReg(MPR121_FDL_FALLING_REG, 12);

    // touched: very slow falling
    writeReg(MPR121_NHD_TOUCHED_REG, 1);
    writeReg(MPR121_NCL_TOUCHED_REG, 8);
    writeReg(MPR121_FDL_TOUCHED_REG, 30);

    // set thresholds.    
    setThresholds(TOUCH_THRESHOLD_BASE, RELEASE_THRESHOLD_BASE);

    // set debounce
    writeReg(MPR121_DEBOUNCE_REG, 0x00);

    // AFE1 same as AFES in 0x7B
    // Filter iterations = 6 samples
    // charge current = 16 uA
    writeReg(MPR121_AFE1_REG, 0b00010000);
    // AFE2:
    // Charge Discharge Time = 0.5 us
    // Filter iterations = 6 samples
    // Sample interval = 1 ms
    writeReg(MPR121_AFE2_REG, 0b00101000);

    // Enable baseline calibration.
    writeReg(MPR121_ECR_REG, 0x80);

    // Auto configuration
    // AFE Samples = 6 samples (Same as AFE1)
    // Retry = No retry
    // Baseline value adjust = load 5MSB after Auto Config.
    // ARE = Enable auto-reconfig
    // ACE = Enable auto-config.
    writeReg(MPR121_AUTO_CONFIG_CONTROL_0_REG, 0b00001011);  

    // writeReg(MPR121_AUTO_CONFIG_CONTROL_1_REG) skipped.

    // Maxxing out sensitivity.
    const uint8_t usl = (3.3 - 0.1) / 3.3 * 256;
    writeReg(MPR121_USL_REG, usl);
    //writeReg(MPR121_USL_REG, 0xC9);
    writeReg(MPR121_LSL_REG, usl * 0.65);
    //writeReg(MPR121_LSL_REG, 0x82);
    writeReg(MPR121_TL_REG, usl * 0.9);
    //writeReg(MPR121_TL_REG, 0xB5);

    // Run 12 electrodes touch, load 5MSB to baseline.
    writeReg(MPR121_ECR_REG, 0x8C);

    uint8_t check = readReg(MPR121_ECR_REG);

    return (check == 0x8C);
}

uint8_t MPR121::stop()
{
    uint8_t ecr = readReg(MPR121_ECR_REG);
    writeReg(MPR121_ECR_REG, ecr & 0xC0);
    return ecr;
}

void MPR121::resume(uint8_t ecr)
{
    writeReg(MPR121_ECR_REG, ecr);
}

void MPR121::softReset()
{
    writeReg(MPR121_SOFT_RESET_REG, 0x63);
}

void MPR121:: setThresholds(uint8_t touch, uint8_t release)
{
    for (uint8_t i = 0; i < 12; i++)
    {
        writeReg(MPR121_TOUCH_THRESHOLD_REG + i * 2, touch);
        writeReg(MPR121_RELEASE_THRESHOLD_REG + i * 2, release);
    }
}

uint16_t MPR121::touched()
{
    return readReg16(MPR121_TOUCH_STATUS_REG);
}

uint16_t MPR121::filterData(uint8_t electrode)
{
    if (electrode > 12)
    {
        return 0;
    }
    return readReg16(MPR121_FILTERED_DATA_REG + electrode * 2);
}

uint8_t MPR121::baseData(uint8_t electrode)
{
    if (electrode > 12)
    {
        return 0;
    }
    return (readReg(MPR121_BASELINE_REG + electrode) << 2);
}

void MPR121::writeReg(uint8_t reg, uint8_t val)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->write(val);
    _wire->endTransmission();
}

uint8_t MPR121::readReg(uint8_t reg)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->endTransmission(false);

    _wire->requestFrom(_addr, (uint8_t)1);

    return _wire->read();
}

uint16_t MPR121::readReg16(uint8_t reg)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->endTransmission(false);

    _wire->requestFrom(_addr, (uint8_t)2);

    uint16_t l = _wire->read();
    uint16_t h = _wire->read();

    return (h << 8) | l;
}

void MPR121::dumpRegistersAnnotated()
{
    Serial.println("\n===== MPR121 REGISTER DUMP =====");

    for (uint8_t reg = 0x00; reg <= 0x7F; reg++)
    {
        uint8_t val = readReg(reg);

        Serial.print("0x");
        if (reg < 16) Serial.print("0");
        Serial.print(reg, HEX);
        Serial.print(" : ");

        if (val < 16) Serial.print("0");
        Serial.print(val, HEX);

        Serial.print("   ");

        // ---- annotations ----
        switch(reg)
        {
            case 0x00: Serial.print("Touch Status L"); break;
            case 0x01: Serial.print("Touch Status H"); break;

            case 0x02: Serial.print("OOR Status L"); break;
            case 0x03: Serial.print("OOR Status H"); break;

            case 0x04: Serial.print("Filtered Data E0 L"); break;
            case 0x05: Serial.print("Filtered Data E0 H"); break;

            case 0x06: Serial.print("Filtered Data E1 L"); break;
            case 0x07: Serial.print("Filtered Data E1 H"); break;

            case 0x1E: Serial.print("Baseline E0"); break;
            case 0x1F: Serial.print("Baseline E1"); break;
            case 0x20: Serial.print("Baseline E2"); break;
            case 0x21: Serial.print("Baseline E3"); break;
            case 0x22: Serial.print("Baseline E4"); break;
            case 0x23: Serial.print("Baseline E5"); break;
            case 0x24: Serial.print("Baseline E6"); break;
            case 0x25: Serial.print("Baseline E7"); break;
            case 0x26: Serial.print("Baseline E8"); break;
            case 0x27: Serial.print("Baseline E9"); break;
            case 0x28: Serial.print("Baseline E10"); break;
            case 0x29: Serial.print("Baseline E11"); break;

            case 0x2B: Serial.print("MHD Rising"); break;
            case 0x2C: Serial.print("NHD Rising"); break;
            case 0x2D: Serial.print("NCL Rising"); break;
            case 0x2E: Serial.print("FDL Rising"); break;

            case 0x2F: Serial.print("MHD Falling"); break;
            case 0x30: Serial.print("NHD Falling"); break;
            case 0x31: Serial.print("NCL Falling"); break;
            case 0x32: Serial.print("FDL Falling"); break;

            case 0x41: Serial.print("Touch Threshold E0"); break;
            case 0x42: Serial.print("Release Threshold E0"); break;

            case 0x5B: Serial.print("Debounce"); break;
            case 0x5C: Serial.print("Config1 (FFI/CDC)"); break;
            case 0x5D: Serial.print("Config2 (CDT/SFI/ESI)"); break;
            case 0x5E: Serial.print("ECR (Electrode Config)"); break;

            case 0x73: Serial.print("GPIO Control 0"); break;
            case 0x74: Serial.print("GPIO Control 1"); break;
            case 0x75: Serial.print("GPIO Data"); break;
            case 0x76: Serial.print("GPIO Direction"); break;
            case 0x77: Serial.print("GPIO Enable"); break;
            case 0x78: Serial.print("GPIO Data Set"); break;
            case 0x79: Serial.print("GPIO Data Clear"); break;
            case 0x7A: Serial.print("GPIO Data Toggle"); break;

            case 0x7B: Serial.print("AutoConfig Control 0"); break;
            case 0x7C: Serial.print("AutoConfig Control 1"); break;
            case 0x7D: Serial.print("AutoConfig USL"); break;
            case 0x7E: Serial.print("AutoConfig LSL"); break;
            case 0x7F: Serial.print("AutoConfig Target"); break;
        }

        Serial.println();
    }

    Serial.println("================================\n");
}

void MPR121::dumpRegisters()
{
    Serial.println("MPR121 Register Dump:");

    for (uint8_t reg = 0x00; reg <= 0x7F; reg++)
    {
        uint8_t val = readReg(reg);

        if (reg % 16 == 0)
        {
            Serial.println();
            Serial.print("0x");
            if (reg < 16) Serial.print("0");
            Serial.print(reg, HEX);
            Serial.print(": ");
        }

        if (val < 16) Serial.print("0");
        Serial.print(val, HEX);
        Serial.print(" ");
    }

    Serial.println("\n");
}