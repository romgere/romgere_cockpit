
# Config file

All the library parameters are stored in `Config\MainConfig.h`. Feel free to edit this file or override certain parameters to fit library to your needs.


# Parameters


## Debug options

Be careful when enable these parameters. The library could be slow down if too much debug message are printing on serial output.

### DEBUG_ETHERNET
Show debug information for Ethernet communication. Print received and sent datas.

Enable by uncomment the line `#define DEBUG_ETHERNET`

### DEBUG_CONTROL_COMMAND_REGISTER
Show debug information when registering input/output control with output/input command.

Enable by uncomment the line `#define DEBUG_CONTROL_COMMAND_REGISTER`

### DEBUG_CONTROL_STAT
Show debug information about controls state change.

Enable by uncomment the line `#define DEBUG_CONTROL_STAT`

### DEBUG_LIBRARY
Show debug information about core library (init, start, ...).

Enable by uncomment the line `#define DEBUG_LIBRARY`

### DEBUG_LIBRARY_LOOP
Show debug information about loop (lopp n°, read/write action, ...).

Enable by uncomment the line `#define DEBUG_LIBRARY_LOOP`

### DEBUG_SLAVE_APP
Show debug information for slave application (init,  read/write on pin action, ...).

Enable by uncomment the line `#define DEBUG_SLAVE_APP`

### DEBUG_I2C
Show debug information for I2C read/write actions.

Enable by uncomment the line `#define DEBUG_I2C`

*Not recommended. I2C logs can sometimes create I2C communication problems*

### DEBUG_SERIAL_START
Automatically open serial port on Application Startup.

Enable by uncomment the line `#define DEBUG_SERIAL_START`

### DEBUG_SERIAL_SPEED
Defined the speed of serial port (use when "DEBUG_SERIAL_START" is enable).

- **Default value :** 115200.

Change the line `#define DEBUG_SERIAL_SPEED 115200` to change the value


## Management capacity

These settings allow you to tune the maximum number of control and X-plane's data the library can manage. They have en direct impact on library memory consumption.

### MAX_INPUT_CONTROL_IN_APPLICATION
Max input and input controls the application can manage.

- **Default value :** 128.

Change the line `#define MAX_INPUT_CONTROL_IN_APPLICATION  128` to change the value

*Decrease = Memory saving.*

### MAX_OUTPUT_CONTROL_IN_APPLICATION
Max input and output controls the application can manage.

- **Default value :** 128.

Change the line `#define MAX_OUTPUT_CONTROL_IN_APPLICATION  128` to change the value

*Decrease = Memory saving.*

### MAX_COMMAND_FOR_ONE_CONTROLE
Max number of command who can be bind to one control.

- **Default value :** 10.
- **Min recommended value :** 3 (For 3 position toggle switch).
- **Max recommended value :** 10 (For rotary switch).

Change the line `#define MAX_COMMAND_FOR_ONE_CONTROLE 10` to change the value

### MAX_INPUT_DATA_FROM_XPLANE
Max input data group received from X-Plane the application can manage. Change this value to fit your need.

- **Default value :** 30.

Change the line `#define MAX_INPUT_DATA_FROM_XPLANE  32` to change the value

*Decrease = Memory saving.*

## Read data/input control frequency

These parameters allow you to define the frequency to which the application will read/parse received data from X-Plane and read input control state.

### NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE

Defined the frequency of read data action (from X-Plane) on "loop" method calls.

- 1 : Read data at each loop
- 2 : Read a loop on two
- ...
- 4 : Read each 4 loop
- Max recommended : 255

- **Default value :** 1.

Change the line `#define NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE 1` to change the value

### NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT
Defined the frequency of read input value action on "loop" method calls.

- 1 : Read data at each loop
- 2 : Read a loop on two
- ...
- 4 : Read each 4 loop
- Max recommended : 255

- **Default value :** 1.

Change the line `#define NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT 1` to change the value


## Rotary encoder "secure mode"

The library implements some "secure mode" features for rotary encoder.

### USE_SECURE_MODE_FOR_MOUSE_ENCODER
Activate "secure“ mode (for cheap type 3 mouse encoder). This mode prevent rotary encoder changes how are not supposed to append. Please see [ArduinoRotaryEncoderControl.cpp](/lib/ArduinoControl/ArduinoRotaryEncoderControl.cpp) header's for more information.

- **Default :** Enable.

Disable by comment the line `#define USE_SECURE_MODE_FOR_MOUSE_ENCODER`

### USE_SECURE_TIME_STATE_CHANGE
Activate "secure" mode for rotary encoder state change (software debouce).

- **Default :** Enable.

Disable by comment the line `#define USE_SECURE_TIME_STATE_CHANGE`

### MIN_STATE_CHANGE_TIME
Min time to allow state change (in micro second).

- **Default value :** 3000.

Change the line `#define MIN_STATE_CHANGE_TIME 3000` to change the value

### USE_SECURE_TIME_DIR_CHANGE
Activate "secure" mode for rotary encoder direction change (software debouce).

- **Default :** Enable.

Disable by comment the line `#define USE_SECURE_TIME_DIR_CHANGE`

### MIN_DIR_CHANGE_TIME
Min time to allow direction change (in micro second).

- **Default value :** 150000.

Change the line `#define MIN_DIR_CHANGE_TIME 150000` to change the value

## Other parameter

### ROTARY_SWITCH_TOLERANCE
Gap allowed (tolerance) between read and expected value for rotary switch (in percent, 10% = 0.1).

- **Default value :** 0.15.

Change the line `#define ROTARY_SWITCH_TOLERANCE 0.15` to change the value.

*Please read [Rotary swich documentation page](/resources/doc/controls/ctrl-rotary-switch.md) for more informations on how rotary switch works*

### ACTIVE_MULTI_ARDUINO_BOARD_MODE
Enable multiple board mode (Master and Slave(s) board with I2C communication).

*Please read [Multiple boards documentation page](/resources/doc/7-multiple-board.md) for more informations on how multiple board mode works*

Enable by uncomment the line `#define ACTIVE_MULTI_ARDUINO_BOARD_MODE`
