# Chu Giga Project Overview

## Project goal

This project aims to create an open-source Chunithm controller using a Raspberry Pi Pico.
The hardware and overall design is heavily inspired by [WhoWeChina's chu_pico controller](https://github.com/whowechina/chu_pico).

This project also includes software that can be used with SegaTools-Compatible game dumps.

We try to fully document the design and include instructions on how to build this controller yourself.

## Current status

We have barely even gotten started lmao:

### PCB

![25%](https://progress-bar.xyz/25?title=PCB)
There are first prototypes of the PCBs already, the ToF boards seem to be great, but the slider currently has the wrong pinout for LEDs so a new version needs to be made and tested.
The mainboard has not been properly tested yet and may need more power input to handle all the LEDs.

### Firmware

![5%](https://progress-bar.xyz/5?title=Firmware)
Basic sensor drivers exist, but require some more work.
There is no firmware for LEDs or actual input handling yet.

### Software

![0%](https://progress-bar.xyz/0?title=Software)
We only figured out that we need our own `chuniio.dll` today.

### CAD
![10%](https://progress-bar.xyz/10?title=CAD)
There is a first version of the LGP panel for the slider, but nothing has been made or tested yet.
The actual case and other components also do not exist yet.

## Features

The controller uses:

- A 32-zone capacitive PCB slider. (MPR121)
- 12 ToF Air sensors. (VL53L0X)
- 3 utility buttons. (Test, Service and Coin)
- RGB Lighting for the slider. (Slider only)

## Project Scope

This repository aims to document not only the final implementation, but also the reasoning behind the design decisions. (Or lack of reasoning when that is applicable :3)

The goal is that a future contributor should be able to understand:

- How the controller works.
- Why the specific hardware was chosen.
- How the firmware is structured.
- How communication with the game works.
- How to build your own controller.

## Supported Software

The controller is intended to support:

- Chunithm cabinet dumps through segatools.
- Umiguri.
- Maybe other games... (simple keyboard output)

Additional software may be supported in the future. (The ToF sensors technically allow for 2 Axis movement.)

## Major Components

The project consists of 4 major components:

### 1. Hardware

Documentation covering:

- PCB Designs.
- Schematics.
- Component choices.
- Assembly instructions.

### 2. Mechanical Design

Documentation covering:

- CAD Files.
- Printed/CNCd parts.
- Additional parts.
- Assembly instructions.

### 3. Firmware

Firmware running on a Raspberry Pi Pico.

Responsibilities include:

- Reading sensors.
- Processing inputs.
- Managing calibration.
- Communicating with the PC.

### 4. PC Interface Software

A custom `chuniio.dll` implementation to be used with SegaTools.

Responsibilities include:

- Receiving controller input.
- Translating controller state into SegaTools-compatible input.
- Providing an interface between the controller and the game.

## Documentation

The documentation is organized into the separate major components and build, assembly and installation guides.
You can find said documentation in the [`docs`](docs/Introduction.md)
