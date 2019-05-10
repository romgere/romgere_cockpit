/*
 * MAC Adress
 */
#define ARDUINO_MAC_ADDRESS { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xEB } //ARduino 2

/*
 * IP Adress
 * {0,0,0,0} for DHCP
 */
#define ARDUINO_IP_ADDRESS { 192, 168, 1, 98 } //Arduino 2

/*
 * XPlane IP adresse ({0,0,0,0} for auto detection)
 */
#define XPLANE_IP_ADDRESS { 192, 168, 1, 21 }

/*
 * Wait for the first reception of X-Plane data when starting the library
 */
#define WAIT_FOR_XPLANE_AT_STARTUP false


/*
 * Read port (receive data from X-Plane)
 */
#define XPLANE_READ_PORT 49001

/*
 * Write port (send data to X-Plane)
 */
#define XPLANE_WRITE_PORT 49000
