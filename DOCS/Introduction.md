# Chu Giga Documentation

Welcome to the Chu Giga documentation.

This documentation covers the hardware, firmware, software, and assembly process of the project.

## Architecture

Documentation describing how the system is designed.

- [System Overview](Architecture/System_Overview.md)
- [Firmware Architecture](Architecture/Firmware_Architecture.md)
- [PC Communication](Architecture/PC_Communication.md)
- [Controller State](Architecture/Controller_State.md)

## Hardware

Documentation describing the electronics and the 3D prints etc.

### PCB

- [PCB Overview](Hardware/PCB_Overview.md)
- [Mainboard](Hardware/Mainboard.md)
- [Slider PCB](Hardware/Slider_PCB.md)
- [Air Strings](Hardware/Air_PCB.md)

### CAD

- [Casing](Hardware/Casing.md)
- [Light-Guide Panel](Hardware/LGP.md)

## Firmware

Documentation describing the RP2040 based firmware.

- [Sensor Drivers](Firmware/Sensor_Drivers.md)
- [Input Processing](Firmware/Input_Processing.md)
- [LED Drivers](Firmware/LED_Drivers.md)
- [HID Implementation](Firmware/HID_Implementation.md)
- [Testing](Firmware/Testing.md)

## Software

Documentation describing the PC-side software.

- [`chuniio.dll`](Software/chuniio_DLL.md)
- [Configuration tool](Software/Configuration_Tool.md)

## Build Guide

Documentation describing how to build the controller.

- [Bill of Materials](Build_Guide/BOM.md)
- [PCB Assembly](Build_Guide/PCB_Assembly.md)
- [General Assembly](Build_Guide/Controller_Assembly.md)
- [Flashing Firmware](Build_Guide/Flashing_Firmware.md)
- [First-Time Setup](Build_Guide/First_Time_Setup.md)
