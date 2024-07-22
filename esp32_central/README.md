# Documentation

## Technical overview

The central application for this LED system has the following tasks:
 - Providing a user interface for managing the connected LED Devices
 - Act as a access point for LED Devices
 - Set modes, colors,.. of connected devices

This mainly works by relying on three handlers:
 - LED_Manager:
   - Holds all connected LED devices
   - Sends commands to devices
   - Provides information about connected devices
 - UDP_handler:
   - Handles incoming messages from the devices (e.g. connection requests)
   - Informs LED_Manager about new devices and queries the adding/welcoming of new devices
 - WebsiteHandler:
   - Manages the user interface, aka a website that runs on the central
   - Creates new commands when the user issues one, these are sent to the LED_Manager command queue

The `loop()` in main.cpp periodically tells the LED_Manager and UDP_handler to handle any packets that arrived since the last update.

## Commands
Commands are UDP messages and can have varying lengths.
Currently, there are five command operators:
 - CHANGE_MODE ('1')
   - Tells the LED device to change it's current mode
 - CHANGE_COLOR ('2')
   - Tels the LED device to change it's 3 main colors
 - TURN_OFF ('3')
   - Tells the LED device to turn off
 - TOGGLE_SYNC ('4')
   - Tells the LED device to sync (or not sync) to broadcast (which means: if not synced, device will only listen to most commands if they are unicasted to them)
 - WELCOME ('5')
   - Can have two meanings: If a welcome message reaches the central, it is a message from a LED device that wants to connect to the central. That message contains all relevant data of that device. If the welcome message stems from the central, it only contains the operator and serves as an ACK.

### Message structures
Messages are structured depending on their types.

#### CHANGE_MODE
This changes the mode AND the colours of the device
| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '1' for changing mode |
| 1                 | The mode identifier |
| 9                 | Values for the 3 colors the device shall use as main colors (3x3byte) |

This changes ONLY the mode of the device
| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '1' for changing mode |
| 1                 | The mode identifier |

#### CHANGE_COLOR
| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '2' for changing color |
| 9                 | Values for the 3 colors the device shall use as main colors (3x3byte) |

#### TURN_OFF
| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '3' for turning the device off |

#### TOGGLE_SYNC
| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '4' for toggling sync |

#### WELCOME
The first message a new devices sends to the central. It contains information on how the device works.
Structure for messages coming **from** an LED device: 
  
| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '5' for welcome messages |
| 1-20              | Supported Device modes (terminated by an X character) |
| 2                 | Number of LEDs, most important 8 bits first |
| 1                 | Current mode this device is in |
| 9                 | Current colors |

Structure for messages being sent **to** an LED device:

| # number of bytes | Description      |
|-------------------|------------------|
| 1                 | Message operator, '5' for welcome msg ACK |

