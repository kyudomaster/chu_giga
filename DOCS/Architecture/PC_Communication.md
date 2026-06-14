# PC Communication

The game is hooked into by segatools which acts as if the game is actually running in a cabinet and handles all IO etc.
Segatools has its own chuniio.dll included in chunihook.dll, which takes keyboard input and maps it to the slider.

sdfghjkl mapping to four touch zones on the slider each and space to all IR leds in the air.

A custom chuniio can be made depending on a controller which will handle communication between the game and the controller. (The raspberry pi pico in this case)

This `.dll` will check what the current state of the pico is. (buttons pressed... slider touched... airstrings played)
