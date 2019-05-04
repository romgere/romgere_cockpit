
/* ****************************** */
/* FONCTION DE GESTION DES LEDs   */
/* ****************************** */

/**********/
/* XPLANE */
/**********/
//Fonction permettant de gérer le voyant "gear in transit" du B58
//Toutes les roues sont sorties (1) ou rentré (0) => on éteint / sinon en transit
float inTransitGearManageFunction( float *val){
 return (( val[0] == 1 && val[1] == 1 && val[2] == 1 ) || ( val[0] == 0 && val[1] == 0 && val[2] == 0 )) ? 0 : 1;
}

//Fonction permettant de gérer le voyant "Flap in transit" du B58
//Valeur de volet = valeur demandée => eteind / Sinon en transit
float inTransitFlapManageFunction( float *val){
 return ( val[3] == val[4] ) ? 0 : 1;
}

//Fonction permettant de gérer le voyant "APH" (flap) du B58
//Position 0.5 : opsition APH => On allume / Sinon eteind
float aphFlapManageFunction( float *val){
 return ( val[4] == 0.5 ) ? 1 : 0;
}


/**********/
/*   FSX  */
/**********/
//Fonction permettant de gérer le voyant "gear in transit" du B58
//Toutes les roues sont sorties (1) ou rentré (0) => on éteint / sinon en transit
float inTransitGearManageFunctionFSX( float *val){
 return (( val[0] == 100 && val[1] == 100 && val[2] == 100 ) || ( val[0] == 0 && val[1] == 0 && val[2] == 0 )) ? 0 : 1;
}
float gearFunctionFSX( float *val){
 return ( val[0] == val[2] && val[1] == val[2] ) ? 0 : 1;
}
//Fonction permettant de gérer le voyant "Flap in transit" du B58
//Valeur de volet = valeur demandée => eteind / Sinon en transit
float inTransitFlapManageFunctionFSX( float *val){
 return ( val[0] == val[2] && val[1] == val[2] ) ? 0 : 1;
}



//Fonction permettant de gérer le voyant "APH" (flap) du B58
//Position 0.5 : opsition APH => On allume / Sinon eteind
float aphFlapManageFunctionFSX( float *val){
 return ( val[0] == 50 && val[1] == 50 ) ? 1 : 0;
}

//Fonction permettant de gérer le voyant "DOWN" (flap) du B58
//Position 100 : position DWN => On allume / Sinon eteind
float downFlapManageFunctionFSX( float *val){
 return ( val[0] == 100 && val[1] == 100 ) ? 1 : 0;
}
