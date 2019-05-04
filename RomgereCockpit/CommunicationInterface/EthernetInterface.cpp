/* Romgere Cockpit Application - http://romgere-xplane-baron58-cockpit.mestres.fr/
 * Réalisation Jérôme Mestres
 * Utilisation, modification et distribution autorisé pour une utilisation personnel non commercial.
 * L'auteur se reserve le droit à une utilisation commerciale.
 * L'auteur refuse tout utilisation commercial de tout ou partie de la librairie, en l'état ou modifiée.
 * Contact : http://romgere-xplane-baron58-cockpit.mestres.fr/
 */



#include "EthernetInterface.h"


EthernetInterface::EthernetInterface(unsigned int readPort, unsigned int writePort, IPAddress arduinoIP, uint8_t arduinoMAC[6], IPAddress xplaneIP, bool waitForXPlane) {

    this->IsClassInit  = false;

    //Mise à zéro des data précédente
    for( int i =0; i < MAX_INPUT_DATA_FROM_XPLANE; i++ ){
        this->LastXPlaneDatas[i] = NULL;
    }

    //Ports Xplane
    this->XPlaneWritePort = writePort;
    this->XplaneReadPort = readPort;
    this->IsXPlaneAdressInit = false;

    //Adress pas encore connue
    if( xplaneIP[0] == 0)
        XPlaneAdress = INADDR_NONE;
    else{
        XPlaneAdress = xplaneIP;
        IsXPlaneAdressInit = true;
    }

    int res = 0;

    //DHCP
	if ( arduinoIP[0] == 0){


        #ifdef DEBUG_ETHERNET
        Serial.println("Ethernet debug : Start Ethernet using DHCP...");
        #endif // DEBUG

        res = Ethernet.begin(arduinoMAC);            // :DHCP

        #ifdef DEBUG_ETHERNET
        if( res == 0){
            Serial.println("Ethernet debug : Failed to configure Ethernet using DHCP");
        }
        else{
            Serial.print("Ethernet debug : Ehernet OK, IP address from DHCP : ");
            for (byte thisByte = 0; thisByte < 4; thisByte++) {
                Serial.print(Ethernet.localIP()[thisByte], DEC);
                Serial.print(".");
            }
            Serial.println();
        }
        #endif // DEBUG_ETHERNET

        //Erreur DHCP ! On ne fait rien de plus
        if( res == 0 ){
            for(;;){}
        }


    }
    //IP fixe
	else{
        #ifdef DEBUG_ETHERNET
            Serial.println("Ethernet debug : Start Ethernet using Static IP Address");
        #endif

        Ethernet.begin( arduinoMAC, arduinoIP);

        #ifdef DEBUG_ETHERNET
        Serial.println("Ethernet debug : Ethernet Start.");
        #endif
	}

    //On démarre l'écoute UDP
	res = this->Udp.begin( this->XplaneReadPort);

        #ifdef DEBUG_ETHERNET
        if( res == 0){
            Serial.println("Ethernet debug : Failed to start UDP");
        }
        else{
            Serial.println("Ethernet debug : UDP start");
        }
        #endif // DEBUG_ETHERNET

    //Erreur UDP On ne fait rien de plus
    if( res == 0 ){
        for(;;){}
    }

    //Class initialisé
    this->IsClassInit = true;

    //On attend un premier retour d'XPlane
	if ( waitForXPlane){

        #ifdef DEBUG_ETHERNET
        Serial.println("Ethernet debug : Start wait for Xplane...");
        #endif // DEBUG

        while ( !this->IsXPlaneAdressInit ){
            this->ReadAllInput();

            #ifdef DEBUG_ETHERNET
            if( this->IsXPlaneAdressInit ){
                Serial.print("Ethernet debug : Ethernet debug : OK, data received from Xplane, IP of Xplane is : ");
                for (byte thisByte = 0; thisByte < 4; thisByte++) {
                    Serial.print(this->XPlaneAdress[thisByte], DEC);
                    Serial.print(".");
                }
                Serial.println();
            }
            else
                Serial.println("Ethernet debug : Nothing received from Xplane still waiting...");
            #endif // DEBUG

            if( !this->IsXPlaneAdressInit )
                delay(250);

        }
	}


}

EthernetInterface::~EthernetInterface(){}

//Lit et parse les données reçues d'Xplane
uint8_t EthernetInterface::ReadAllInput(){

    //Pas initialisée
    if( ! this->IsClassInit )
        return 0;

    //Récupère les paquets UDP disponibles
    int readSize = this->Udp.parsePacket();
	if (readSize){

        //Première reception d'info, on sauvegarde l'adresse d'XPlane
        if ( !this->IsXPlaneAdressInit ){
            this->XPlaneAdress = this->Udp.remoteIP();
            this->IsXPlaneAdressInit = true;
        }


        #ifdef DEBUG_ETHERNET
        Serial.print("Ethernet debug : Received ");
        Serial.print(readSize);
        Serial.println(" byte(s) from Xplane, parsing...");
        #endif // DEBUG


        byte buffer[readSize];
        //On lit les paquet UDP
        this->Udp.read( buffer, readSize );

        //Les données en provenance d'Xplane commence par "DATA>"
        //ref : http://svglobe.com/arduino/udpdata.html
        if (buffer[0] == 'D' && buffer[1] == 'A' && buffer[2] == 'T' && buffer[3] == 'A'  ){

            //Remise à zéro des data précédente
            for( int i =0; i < MAX_INPUT_DATA_FROM_XPLANE; i++ ){
                if( this->LastXPlaneDatas[i] != NULL ){
                    delete this->LastXPlaneDatas[i];
                }

                this->LastXPlaneDatas[i] = NULL;
            }


            uint8_t index = 0;
            //On parse les données reçues (par paquet de 36 Bytes, la longueur d'une trame donnée XPlane)
            for (int i=5 ; i < readSize; i+=36) {

                //Nouvelle données reçue d'XPlane
                XPData *p = new XPData();
                //Le groupe
                p->group = buffer[i];

                #ifdef DEBUG_ETHERNET
                Serial.print("Ethernet debug : Receive data for group ");
                Serial.print(p->group);
                Serial.print(" with value [");
                #endif

                //Les données (8 x 4 bytes)
                for (int j=0; j<8; j++){

                    XPGroupData tmpData;

                    for (int k=0; k<4; k++){
                        tmpData.byteVal[k] = buffer[i + 4 + (j*4) + k];
                    }

                    p->data[j] = tmpData.floatVal;

                    #ifdef DEBUG_ETHERNET
                    Serial.print(p->data[j]);
                    Serial.print(",");
                    #endif
                }

                #ifdef DEBUG_ETHERNET
                Serial.println("].");
                #endif

                //Une fois lu on stock dans notre tableau
                this->LastXPlaneDatas[index] = p;

                index++;
            }

            //On renvoie le nombre de paquet reçus et archivés
            //Permet de ne pas lancer de modif sur les Output si aucunes données dispo...
            return index;

        }


        //Si pas "DATA>"
        #ifdef DEBUG_ETHERNET
        Serial.println("Ethernet debug : Data received not start with \"DATA>\" : ");
        Serial.println("---------------START------------------");
        for (int i=0; i < readSize; i++)
            Serial.print((char)buffer[i]);
        Serial.println("----------------END-------------------");
        #endif


	} // SI readSize > 0

	return 0;
}



//Envoi une touche à Xplane
void EthernetInterface::SendKey( const char* key) {

    //Pas initialisée
    if( ! this->IsClassInit )
        return;

	this->Udp.beginPacket( this->XPlaneAdress, this->XPlaneWritePort);

    this->Udp.write("CHAR0");

	this->Udp.write((char)*key) ;
	this->Udp.write("") ;



    #ifdef DEBUG_ETHERNET

		int res = this->Udp.endPacket();

		if( res )
			Serial.print("Ethernet debug : Send key \"");
		else
			Serial.print("Ethernet debug : Error sending key \"");

		Serial.print(key);
		Serial.println("\" to Xplane.");

	#else

    	this->Udp.endPacket();

    #endif

}


//Envoi une commande à Xplane
void EthernetInterface::SendCommand( const char* cmd) {

    //Pas initialisée
    if( ! this->IsClassInit )
        return;

	this->Udp.beginPacket( this->XPlaneAdress, this->XPlaneWritePort);

    this->Udp.write("CMND0");

	this->Udp.write(cmd) ;



    #ifdef DEBUG_ETHERNET

		int res = this->Udp.endPacket();
		if( res )
			Serial.print("Ethernet debug : Send command \"");
		else
			Serial.print("Ethernet debug : Error sending command \"");

		Serial.print(cmd);
		Serial.println("\" to Xplane.");

	#else
    	this->Udp.endPacket();
    #endif // DEBUG

}


//Envoi une commande DREF à Xplane
/*
void EthernetInterface::SendDrefCommand( const char *dref, byte data[]){

    //Pas initialisée
    if( ! this->IsClassInit )
        return;

	this->Udp.beginPacket( this->XPlaneAdress, this->XPlaneWritePort);
    this->Udp.write("DREF0");

    #ifdef DEBUG_ETHERNET
        Serial.print("Send DREF command [");
    #endif
    //Envoi des données
    for (int i=0; i<4; i++){
        this->Udp.write( data[i]);

        #ifdef DEBUG_ETHERNET
            Serial.print( data[i]);
            if( i < 3 ) Serial.print(",");
        #endif
    }

    //Donnée à envoyer
    char DataOut[500];

    //"sim/"
    DataOut[0] = 's';
    DataOut[1] = 'i';
    DataOut[2] = 'm';
    DataOut[3] = '/';

    int i = 0;
    while( dref[i] != 0 && i < 499 ){
        DataOut[i+4] = dref[i];
        i++;
    }
    for(; i < 498; i++  ){
        DataOut[i] = char(32);
    }
    DataOut[499] = 0;

    #ifdef DEBUG_ETHERNET
    Serial.print("] with command \"");
    Serial.print(DataOut);
    Serial.print("\" : ");
    #endif

    int res = this->Udp.write(DataOut);

    #ifdef DEBUG_ETHERNET
    if( res == 500 )
        Serial.println("succes");
    else
        Serial.println("error");
    #endif // DEBUG


    this->Udp.endPacket();
}

*/

XPData* EthernetInterface::GetData( float group ){

    for( int i =0; i < MAX_INPUT_DATA_FROM_XPLANE; i++ ){
        if( this->LastXPlaneDatas[i] != NULL &&  this->LastXPlaneDatas[i]->group == group ){
            return this->LastXPlaneDatas[i];
        }
    }

    return NULL;
}
