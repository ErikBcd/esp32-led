# LED Peripheral Documentation

## Technical overview

These LED peripherals are made to be as simple as possible - we have one helper class for parsing and creating messages, one class that controls the state of the led's, and the main file that contains the setup function and the loop that gets called periodically by rtOS.

The `LED_State` controls the current state of the LEDs. It also contains functions for the modes, modes are functions that directly control the colors of the LEDs. For adding a mode one must
 - Add a mode function to the `LED_State`
 - Add the mode to the enum `MODE`
 - Add it to the `supported_modes` vector (this is used for telling the central about what kind of modes this device has)
 - Add the mode to the central so it knows about it

By calling the `update()` function of the LED_State the mode function of the currently selected mode will be called and the LEDs will be updated.

The `LED_State` also holds two rather important booleans:
 - `synced`
   - Per default, all devices set their modes/colors/.. according to broadcasts from the central device. But if synced is false, this device only listens to unicasts. This is useful when you want to control LED strips independently.
 - `connected`
   - This is false as long as we didn't establish a connection to the central. A connection is established by sending a welcome message to the central, and receiving a welcome message back from the central as a unicast.
   - We are sending a welcome message every 3 seconds if we are connected to a WiFi, but not connected to the central host. (see: `check_reconnect` in `main.cpp`)

The `loop()` in `main.cpp` periodically checks for new udp packets to be parsed, checks if we are still connected, and then updates the LEDs.