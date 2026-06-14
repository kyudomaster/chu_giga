# System Overview

The Chu Giga controller intends to emulate a real original Chunithm controller, with a 32-zone slider and 'Air strings'.
Instead of using interrupting IR sensors in 6 rows in the air, we are using 12 ToF sensors to emulate this behaviour instead. The main reason for this being space savings.

A completely flat controller like this is easier to store away and requires less setup when grabbing it out of storage for a quick game.

## Original hardware [ref](https://blog.pinapelz.com/blog/umiguri/)

The original Chunithm controller has a ground slider input and air input, as well as 2 utility buttons and LED lighting.

The ground slider corresponds to 16 touch zones in the game, but actually has 32 touch zones itself because some harder songs require touches while holding a note.

The 'Air strings' are 6 Infrared LEDs on one side, and 6 receivers on the other side in the air above the slider.
Interrupting the beams is how the game detects air input.

The game also reads 2 utility buttons (Test and Service) and typically has a coin slot for credits.

Furthermore, the original controller has LED lighting all over, and most importantly on the ground slider itself, Illuminating the 16 touch zones and dividers between the zones. (31 LEDs total).

We mostly include these four separate features in the Chu Giga controller.

## System Requirements

We want this controller to be to scale with the arcade version, which guides most of the slider dimensions.

Other than that we also must have:

- 32 independent slider zones.
- Some form of Air input which corresponds to 6 different zones.
- The input to be low latency enough to play the game.
- The controller to be compatible with SegaTools to play with arcade dumps.
- The controller should also be compatible with other games... so emulating a keyboard or a controller is appropriate.

## Physical Architecture

To keep costs down and assembly reasonable and more fool-proof, since I had very little experience with electronic design before this, the controller is split up into separate components.

- There is one PCB for the ground slider.
- There are two separate PCBs for the air strings.
- There is one 'mainboard' that the other PCBs connect to, and which in turn connects to the PC.
- All PCBs are kept in place with an enclosure which can be 3D-printed or partially CNC milled.
- There are several other parts like a light guide plate, diffusion film etc. inspired by the chu_pico by [WhoWeChina](https://github.com/whowechina/chu_pico)

## Electronic Architecture

The controller uses capacitive touch sensors for the ground slider. This is an easy and cheap way to get touch input like we want it, and it allows for the PCB to be quite simple.

The air strings use time of flight sensors rather than IR beam interruption. The main reason for this being that this allows for slightly more flexible input, and that the air strings can be in plane with the ground slider, meaning the entire controller can be flat and compact.

The controller uses a Raspberry Pi Pico for processing and communication (RP2040) and the ground slider has 33 RGB LEDs, one for all 16 horizontal zones, and 17 for the dividers. (With 2 extra on both ends.)

Furthermore, there are 3 buttons to emulate the Test, Service, and Coin functions of the original controller.

## Software Architecture

For the controller to communicate with the game, we must emulate a 'real controller'.

Thankfuly we do not need to do the work for this, but we can 'simply' use SegaTools.
SegaTools can either use keyboard input or a custom `.dll` for the controller.

Since I'm in this deep already, I decided to go for a custom `chuniio.dll` which is the bridge between the game and the controller.

The controller handles the USB Communication and some sensor processing.
