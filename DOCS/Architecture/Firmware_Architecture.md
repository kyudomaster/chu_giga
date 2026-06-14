# Firmware Architecture

This section details how the firmware running on the Raspberry Pi Pico is structured.

## Responsibilities

The firmware is responsible for:

- Reading the sensors on the controller, both the capacitive touch and ToF sensors.
- Processing this sensor data into 'input'.
- Maintaining a controller state.
- Communicating this state with the PC.
- Taking LED information from the PC and providing this to the LEDs on the slider PCB.

The firmware is not responsible for:

- Anything game-specific.
- SegaTools integration.
- Input mapping in the game.

## Layers

The firmware consists of different layers which only talk to the layer beneath them.

We have the physical hardware which feeds into sensor boards; 12 VL53L0X ToF sensors and 3 MPR121 Capacitive touch sensors.

We have drivers which can talk to the sensors over i2c.

The input processing talks to the drivers and converts that information into actual inputs.

The controller state checks what inputs are given at any moment and the USB communication provides the state to the PC.

The communication also takes back in LED information from the PC and provides it to the LED Drivers.

## Controller state

The firmware maintains a hardware-agnostic Controller state.
Different communication layers may expose this state as a Chunithm-specific device, a generic game controller, or a keyboard without affecting the underlying sensor processing.

Chunithm itself only requires 32 Zones and 6 Air zones. But with our hardware, it's possible to measure when we are between touch zones on the PCB too... , meaning we can have up to 93 touch zones and a free moving air zone. (With both height and X-location information.)

Regardless, the state should contain information about which of the utility buttons are pressed, and which ground and air zones are touched.
