/* *********************** */
/* LED Management function */
/* *********************** */

// Transformation function for "Gear in transit" indicator
// All gear down (1) or up (0) => turn off (return 0), turn on (return 1)
// otherwise
float inTransitGearManageFunction(float *val) {
  return ((val[0] == 1 && val[1] == 1 && val[2] == 1) ||
          (val[0] == 0 && val[1] == 0 && val[2] == 0))
             ? 0
             : 1;
}

// Transformation function for "Flap in transit" indicator
// Current flap value = Desired flap value => turn off (return 0), turn on
// (return 1) otherwise
float inTransitFlapManageFunction(float *val) {
  return (val[3] == val[4]) ? 0 : 1;
}

// Transformation function for "APH" (approch flap position)
// Position = 0.5 => turn on (return 1), turn off (return 0) otherwise
float aphFlapManageFunction(float *val) { return (val[4] == 0.5) ? 1 : 0; }
