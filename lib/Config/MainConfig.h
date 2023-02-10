/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ROMGERE_COCKPIT_CONFIG_FILE
#define ROMGERE_COCKPIT_CONFIG_FILE

/*
 * Automatically open serial port on Application Startup
 */
// #define DEBUG_SERIAL_START
#define DEBUG_SERIAL_SPEED 115200

/*
 * Enable multiple board mode (Master and Slave(s) board with I2C communication)
 */
#define ACTIVE_MULTI_ARDUINO_BOARD_MODE

/*
 * Max input and output controls the application can manage.
 * Decrease = Memory saving.
 */
#define MAX_INPUT_CONTROL_IN_APPLICATION 128
#define MAX_OUTPUT_CONTROL_IN_APPLICATION 128

/*
 * Max number of command who can be bind to one control.
 * Min : 3 (For 3 position toggle switch)
 * Max : 10 (For rotary switch)
 */
#define MAX_COMMAND_FOR_ONE_CONTROLE 10

/*
 * Max input data group received from X-Plane the application can manage
 * Adapt for your need.
 * Decrease = Memory saving.
 */
#define MAX_INPUT_DATA_FROM_XPLANE 32

/*
 * Defined the frequency of read data action (from X-Plane) on "loop" method
 * calls 1 : Read data at each loop 2 : Read a loop on two
 * ...
 * 4 : Read each 4 loop
 * Max recommended : 255
 */
#define NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE 1

/*
 * Defined the frequency of read input value action on "loop" method calls
 * 1 : Read data at each loop
 * 2 : Read a loop on two
 * ...
 * 4 : Read each 4 loop
 * Max recommended : 255
 */
#define NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT 1

// Activate "secure“ mode (for cheap type 3 mouse encoder)
#define USE_SECURE_MODE_FOR_MOUSE_ENCODER

// Activate "secure" mode for rotary encoder state change (software debouce)
#define USE_SECURE_TIME_STATE_CHANGE
// Min time to allow state change (in micro second)
#define MIN_STATE_CHANGE_TIME 3000

// Activate "secure" mode for rotary encoder direction change (software debouce)
#define USE_SECURE_TIME_DIR_CHANGE
// Min time to allow direction change (in micro second)
#define MIN_DIR_CHANGE_TIME 150000

/*
 * Gap allowed (tolerance) between read and expected value for rotary switch (in
 * percent, 10% = 0.1) 0.15 recommended
 */
#define ROTARY_SWITCH_TOLERANCE 0.15

#endif // ROMGERE_COCKPIT_CONFIG_FILE
