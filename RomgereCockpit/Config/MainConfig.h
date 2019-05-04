/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */

#ifndef ROMGERE_COCKPIT_CONFIG_FILE
#define ROMGERE_COCKPIT_CONFIG_FILE

/*
 * Active les message de debug sur la sortie Port Serie
 * Commenter la ligne pour désactiver les message de débug
 */
//#define DEBUG_ETHERNET

//#define DEBUG_CONTROL_COMMAND_ASSOC
//#define DEBUG_CONTROL_STAT

//#define DEBUG_LIBRARY
//#define DEBUG_LIBRARY_LOOP

//#define DEBUG_SLAVE_APP

// Non conseillé ! les logs font "bugger" les transmission I²C
//#define DEBUG_I2C

/*
 * Permet d'ouvrir le port série automatiquement au lancement de l'application
*/
//#define DEBUG_SERIAL_START
//#define DEBUG_SERIAL_SPEED 115200


/*
 * Active ou non la gestion des carte esclave (Plusieurs arduino connecté via I2C)
 */
#define ACTIVE_MULTI_ARDUINO_BOARD_MODE


/*
 * Nombre de contrôle max géré par l'application
 */
#define MAX_INPUT_CONTROL_IN_APPLICATION  128
#define MAX_OUTPUT_CONTROL_IN_APPLICATION  128

/*
 * Nombre de commande max par controle (pour les rotary switch)
 * Min : 3, Max : 10 ou Position max de vos Switch rotatif
 */
#define MAX_COMMAND_FOR_ONE_CONTROLE 10



/*
 * Nombre max de donnée (group) reçues d'Xplane à gérer
 */
#define MAX_INPUT_DATA_FROM_XPLANE  32



/*
 * Port par lequel les données d'Xplane sont reçues
 */
#define XPLANE_READ_PORT 49001

/*
 * Port par lequel on envoi les donnée vers Xplane
 */
#define XPLANE_WRITE_PORT 49000





/*
 * Définit a quelle moment les données d'Xplane sont lues lors des appel  à loop()
 * 1 : Lecture à chaque loop
 * 2 : Lecture une loop sur deux
 * ...
 * 4 : Lecture toutes les 4 loop
 * Max : 255
 */
#define NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE 1


/*
 * Définit a quelle moment les données contrôle de l'arrduino (en input) sont lus lors des appel  à loop()
 * 1 : Lecture à chaque loop
 * 2 : Lecture une loop sur deux
 * ...
 * 4 : Lecture toutes les 4 loop
 * Max : 255
 */
#define NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT 1


//Permet d'utiliser un mode "securisé" pour les mouses encode (Type 3)
//Les mouse ecode n'étant pas forcement de bonne qualité, ce paramètre permet :

//lors d'un changement d'état de l'encoder, une verification de la cohérence est faite.
//Si le nouvel état n'est pas un etat suivant normalement possible, on ne prend pas en compte le changement.
//(Exemple : passage de la valeur 3 à 1 ou 2 : OK, de 3 à 0 : NOK on ignore.
#define USE_SECURE_MODE_FOR_MOUSE_ENCODER  1


//Active ou non le controle du temp min de changement d'un etat de l'encoder
#define USE_SECURE_TIME_STAT_CHANGE
//Temp en micro seconde
#define MIN_STAT_CHANGE_TIME 3000

//Active ou non le controle du temp min de changement de direction de l'encoder
#define USE_SECURE_TIME_DIR_CHANGE
//Temp en micro seconde
#define MIN_DIR_CHANGE_TIME 150000

/*
 * Ecart de valeur (tolerance) géré pour les rotary switch (en pourcentage, 10% = 0.1)
 * 0.15 conseillé
 */
#define ROTARY_SWITCH_TOLERANCE 0.15


#endif // ROMGERE_COCKPIT_CONFIG_FILE
