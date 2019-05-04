/*
 * Adresse MAC
 */
#define ARDUINO_MAC_ADDRESS { 0xDE, 0xAD, 0xBE, 0xEF, 0xEA, 0xED } //ARduino 1

/*
 * Adresse IP
 * {0,0,0,0} pour utilisation du DHCP
 */
#define ARDUINO_IP_ADDRESS { 192, 168, 1, 97 } //ARduino 1

/*
 * Adresse IP d'XPlane (mettre {0,0,0,0} pour une detection auto)
 */
#define XPLANE_IP_ADDRESS { 192, 168, 1, 21 }

/*
 * Doit-on attendre de recevoir les info d'XPlane au lancement de l'arduino
 */
#define WAIT_FOR_XPLANE_AT_STARTUP true


/*
 * Port par lequel les données d'Xplane sont reçues
 */
#define XPLANE_READ_PORT 49001

/*
 * Port par lequel on envoi les donnée vers Xplane
 */
#define XPLANE_WRITE_PORT 49000
