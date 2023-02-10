/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ROMGERECOCKPITCLASS_H_INCLUDED
#define ROMGERECOCKPITCLASS_H_INCLUDED

#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>

#include "../Config/MainConfig.h"

#include "../CommunicationInterface/BaseCommunicationInterface.h"

#include "../ArduinoControl/ArduinoControl.h"
#include "../XPlaneData/XPlaneCommand.h"
#include "../XPlaneData/XPlaneInputData.h"

// Type for "transformation function". "Transformation function" are used to
// "convert" an x-plane DREF group (float table) in a single float send to ouput
// control. Example : convert gear indicator group (67) for "in transit" led
// indicator
typedef float (*inputDataTransformFunction)(float *);

// Main Application
class CockpitMainApplication {

private:
// loop number to deal with send/receive action jump
#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1 ||                             \
    NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
  uint8_t LoopNumber;
#endif

  // Communication interface between ArduinoBoard and X-Plane (Serial, NetWork,
  // Debug)
  BaseCommunicationInterface *CommunicationInterface;

  // Association between input control and output command
  typedef struct {
    ArduinoInputControl *Control;
    XPlaneOutputCommand *OutputCommand[MAX_COMMAND_FOR_ONE_CONTROLE];
  } InputControlAssociation;
  InputControlAssociation *InputControlList[MAX_INPUT_CONTROL_IN_APPLICATION];
  unsigned int InputControlCount;

  // Association between output control and input data
  typedef struct {
    ArduinoOutputControl *Control;
    XPlaneInputData *IntputData;
    inputDataTransformFunction TransformationFunction;
  } OutputControlAssociation;
  unsigned int OutputControlCount;
  OutputControlAssociation
      *OutputControlList[MAX_OUTPUT_CONTROL_IN_APPLICATION];

  // Update output control if needed with value of binded input command
  void updateControlWithCommand(OutputControlAssociation *oca);

  // Read input control and send command to X-plane if needed
  void doControlCommandProcess(InputControlAssociation *ica,
                               bool forceSendingCommand = false);

  // Deal with library internal command
  void doInternalCommande(LibrarySpecialCommand *cmd);

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  // I2C bus already init
  bool I2CInit;
  // Init I2C interface if control is defined on slave arduno board (All
  // registered control are passed to this methode)
  void CheckAndInitI2CIfNeed(ArduinoControl *ctrl);
#endif

public:
  CockpitMainApplication(BaseCommunicationInterface *comInterface);
  ~CockpitMainApplication();

  // Register an input control (switch, encode, ...) and bind it to one or more
  // output command(s)
  void RegisterInputControl(ArduinoInputControl *ctrl, XPlaneOutputCommand *cmd1
#if MAX_COMMAND_FOR_ONE_CONTROLE > 1
                            ,
                            XPlaneOutputCommand *cmd2 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 2
                            ,
                            XPlaneOutputCommand *cmd3 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 3
                            ,
                            XPlaneOutputCommand *cmd4 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 4
                            ,
                            XPlaneOutputCommand *cmd5 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 5
                            ,
                            XPlaneOutputCommand *cmd6 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 6
                            ,
                            XPlaneOutputCommand *cmd7 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 7
                            ,
                            XPlaneOutputCommand *cmd8 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 8
                            ,
                            XPlaneOutputCommand *cmd9 = NULL
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 9
                            ,
                            XPlaneOutputCommand *cmd10 = NULL
#endif
  );

  // Register an output control (LED, servo, ...) and bind it to one intput
  // data(s) (with optional "transformation“ function)
  void RegisterOutputControl(ArduinoOutputControl *ctrl, XPlaneInputData *data,
                             inputDataTransformFunction fct = NULL);

  // Main app loop :
  //  - read xplane Datas
  //  - update outpput controls
  //  - read input controls
  //  - send commands to X-plane
  void Loop();
};

#endif
