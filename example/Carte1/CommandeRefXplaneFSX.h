
//INDEX DES COMMANDES DANS LE TABLEAU
#define magnetos_off_1 0
#define magnetos_right_1 1
#define magnetos_left_1 2
#define magnetos_both_1 3
#define engage_starter_1 4
#define magnetos_off_2 5
#define magnetos_right_2 6
#define magnetos_left_2 7
#define magnetos_both_2 8
#define engage_starter_2 9
#define landing_lights_on 10
#define landing_lights_off 11
#define taxi_lights_on 12
#define taxi_lights_off 13
#define strobe_lights_on 14
#define strobe_lights_off 15
#define nav_lights_on 16
#define nav_lights_off 17
#define beacon_lights_on 18
#define beacon_lights_off 19
#define avionics_on 20
#define avionics_off 21
#define battery_1_on 22
#define battery_1_off 23
#define generator_1_on 24
#define generator_1_off 25
#define generator_2_on 26
#define generator_2_off 27
#define prop_sync_on 28
#define prop_sync_off 29
#define pitot_heat0_on 30
#define pitot_heat1_on 31
#define pitot_heat0_off 32
#define pitot_heat1_off 33
#define AOA_heat0_on 34
#define AOA_heat0_off 35
#define prop_heat0_on 36
#define prop_heat0_off 37
#define window_heat_on 38
#define window_heat_off 39
#define wing_heat0_on 40
#define wing_heat0_off 41
#define fuel_pump_1_on 42
#define fuel_pump_1_off 43
#define fuel_pump_2_on 44
#define fuel_pump_2_off 45


/*
 * Tableau de référence des commandes
 * [ indice de la commande] [ 0 : xplane, 1 : FSX ]
 */
const char* commandTab[][2] = {
    {"sim/magnetos/magnetos_off_1","MAGNETO1_OFF"},
    {"sim/magnetos/magnetos_right_1","MAGNETO1_RIGHT"},
    {"sim/magnetos/magnetos_left_1","MAGNETO1_LEFT"},
    {"sim/magnetos/magnetos_both_1","MAGNETO1_BOTH"},
    {"sim/starters/engage_starter_1","MAGNETO1_START"},
    {"sim/magnetos/magnetos_off_2","MAGNETO2_OFF"},
    {"sim/magnetos/magnetos_right_2","MAGNETO2_RIGHT"},
    {"sim/magnetos/magnetos_left_2","MAGNETO2_LEFT"},
    {"sim/magnetos/magnetos_both_2","MAGNETO2_BOTH"},
    {"sim/starters/engage_starter_2","MAGNETO2_START"},
    {"sim/lights/landing_lights_on","LANDING_LIGHTS_ON"},
    {"sim/lights/landing_lights_off","LANDING_LIGHTS_OFF"},
    {"sim/lights/taxi_lights_on","USR_TAXI_LIGHTS_ON"},
    {"sim/lights/taxi_lights_off","USR_TAXI_LIGHTS_OFF"},
    {"sim/lights/strobe_lights_on","STROBES_ON"},
    {"sim/lights/strobe_lights_off","STROBES_OFF"},
    {"sim/lights/nav_lights_on","USR_NAV_LIGHTS_ON"},
    {"sim/lights/nav_lights_off","USR_NAV_LIGHTS_OFF"},
    {"sim/lights/beacon_lights_on","USR_BEACON_LIGHTS_ON"},
    {"sim/lights/beacon_lights_off","USR_BEACON_LIGHTS_OFF"},
    {"sim/systems/avionics_on","AVIONICS_MASTER_SET;1"},
    {"sim/systems/avionics_off","AVIONICS_MASTER_SET;0"},
    {"sim/electrical/battery_1_on","USR_BATTERY_ON"},
    {"sim/electrical/battery_1_off","USR_BATTERY_OFF"},
    {"sim/electrical/generator_1_on","USR_LEFT_ALTERNATOR_ON"},
    {"sim/electrical/generator_1_off","USR_LEFT_ALTERNATOR_OFF"},
    {"sim/electrical/generator_2_on","USR_RIGHT_ALTERNATOR_ON"},
    {"sim/electrical/generator_2_off","USR_RIGHT_ALTERNATOR_OFF"},
    {"sim/systems/prop_sync_on","USR_PROP_SYNC_ON"},
    {"sim/systems/prop_sync_off","USR_PROP_SYNC_OFF"},
    {"sim/ice/pitot_heat0_on","PITOT_HEAT_ON"},
    {"sim/ice/pitot_heat1_on","PITOT_HEAT_ON"},
    {"sim/ice/pitot_heat0_off","PITOT_HEAT_OFF"},
    {"sim/ice/pitot_heat1_off","PITOT_HEAT_OFF"},
    {"sim/ice/AOA_heat0_on","NULL"},
    {"sim/ice/AOA_heat0_off","NULL"},
    {"sim/ice/prop_heat0_on","USR_PROP_DEICE_ON"},
    {"sim/ice/prop_heat0_off","USR_PROP_DEICE_OFF"},
    {"sim/ice/window_heat_on","NULL"},
    {"sim/ice/window_heat_off","NULL"},
    {"sim/ice/wing_heat0_on","ANTI_ICE_SET_ENG1;1"},
    {"sim/ice/wing_heat0_off","ANTI_ICE_SET_ENG1;0"},
    {"sim/fuel/fuel_pump_1_on","USR_PUMP_1_ON"},
    {"sim/fuel/fuel_pump_1_off","USR_PUMP_1_OFF"},
    {"sim/fuel/fuel_pump_2_on","USR_PUMP_2_ON"},
    {"sim/fuel/fuel_pump_2_off","USR_PUMP_2_OFF"}
};
