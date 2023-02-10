/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#include "CockpitMainApplication.h"

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
// Include I2C support if needed
#include <Wire.h>
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE

CockpitMainApplication::CockpitMainApplication(
    BaseCommunicationInterface *comInterface) {

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  I2CInit = false;
#endif // ACTIVE_MULTI_ARDUINO_BOARD_MODE

  // Communication interface from/to XPlane (Ethernet/Serial/Debug)
  CommunicationInterface = comInterface;

#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1 ||                             \
    NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
  LoopNumber = 0;
#endif

  InputControlCount = 0;
  OutputControlCount = 0;

  // NULL Initialisation of control/command association (input)
  for (int i = 0; i < MAX_INPUT_CONTROL_IN_APPLICATION; i++)
    InputControlList[i] = NULL;

  // NULL Initialisation of control/command association (output)
  for (int i = 0; i < MAX_OUTPUT_CONTROL_IN_APPLICATION; i++)
    OutputControlList[i] = NULL;
}

CockpitMainApplication::~CockpitMainApplication() {

  for (int i = 0; i < MAX_INPUT_CONTROL_IN_APPLICATION; i++)
    if (InputControlList[i] != NULL)
      delete InputControlList[i];

  for (int i = 0; i < MAX_OUTPUT_CONTROL_IN_APPLICATION; i++)
    if (OutputControlList[i] != NULL)
      delete OutputControlList[i];

  delete CommunicationInterface;
}

// Register an input control (switch, encode, ...) and bind it to one or more
// output command(s)
void CockpitMainApplication::RegisterInputControl(ArduinoInputControl *ctrl,
                                                  XPlaneOutputCommand *cmd1
#if MAX_COMMAND_FOR_ONE_CONTROLE > 1
                                                  ,
                                                  XPlaneOutputCommand *cmd2
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 2
                                                  ,
                                                  XPlaneOutputCommand *cmd3
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 3
                                                  ,
                                                  XPlaneOutputCommand *cmd4
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 4
                                                  ,
                                                  XPlaneOutputCommand *cmd5
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 5
                                                  ,
                                                  XPlaneOutputCommand *cmd6
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 6
                                                  ,
                                                  XPlaneOutputCommand *cmd7
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 7
                                                  ,
                                                  XPlaneOutputCommand *cmd8
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 8
                                                  ,
                                                  XPlaneOutputCommand *cmd9
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 9
                                                  ,
                                                  XPlaneOutputCommand *cmd10
#endif
) {

  if (InputControlCount >= MAX_INPUT_CONTROL_IN_APPLICATION)
    return;

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  CheckAndInitI2CIfNeed(ctrl);
#endif

  /*
      TODO: Check args according to ArduinoInputControl (analog/digital)
      Analog : First command = DREF (and other NULL or ignored)
      Digital : One to three (Key, Simple, Data or Internal) command
  */

  InputControlAssociation *assoc = new InputControlAssociation();

  assoc->Control = ctrl;
  assoc->OutputCommand[0] = cmd1;

#if MAX_COMMAND_FOR_ONE_CONTROLE > 1
  if (cmd2 != NULL) {
    assoc->OutputCommand[1] = cmd2;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 2
  if (cmd3 != NULL) {
    assoc->OutputCommand[2] = cmd3;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 3
  if (cmd4 != NULL) {
    assoc->OutputCommand[3] = cmd4;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 4
  if (cmd5 != NULL) {
    assoc->OutputCommand[4] = cmd5;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 5
  if (cmd6 != NULL) {
    assoc->OutputCommand[5] = cmd6;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 6
  if (cmd7 != NULL) {
    assoc->OutputCommand[6] = cmd7;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 7
  if (cmd8 != NULL) {
    assoc->OutputCommand[7] = cmd8;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 8
  if (cmd9 != NULL) {
    assoc->OutputCommand[8] = cmd9;
  }
#endif
#if MAX_COMMAND_FOR_ONE_CONTROLE > 9
  if (cmd10 != NULL) {
    assoc->OutputCommand[9] = cmd10;
  }
#endif

  InputControlList[InputControlCount] = assoc;

  InputControlCount++;
}

// Register an output control (LED, servo, ...) and bind it to one intput
// data(s) (with optional "transformation“ function)
void CockpitMainApplication::RegisterOutputControl(
    ArduinoOutputControl *ctrl, XPlaneInputData *data,
    inputDataTransformFunction fct) {

  if (OutputControlCount >= MAX_OUTPUT_CONTROL_IN_APPLICATION)
    return;

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  CheckAndInitI2CIfNeed(ctrl);
#endif

  OutputControlAssociation *assoc = new OutputControlAssociation();

  assoc->Control = ctrl;
  assoc->IntputData = data;
  assoc->TransformationFunction = fct;

  OutputControlList[OutputControlCount] = assoc;
  OutputControlCount++;
}

// Main app loop :
//  - read xplane Datas
//  - update outpput controls
//  - read input controls
//  - send commands to X-plane
void CockpitMainApplication::Loop() {

#if NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
  if (LoopNumber % NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE == 0) {
#endif

    // Read data from X-plane
    uint8_t nbDataRead = CommunicationInterface->ReadAllInput();

    // update ouput controls
    if (nbDataRead > 0) {
      for (unsigned int i = 0; i < OutputControlCount; i++) {
        this->updateControlWithCommand(OutputControlList[i]);
      }
    }
#if NUMBER_LOOP_SKIP_FOR_READ_DATA_FROM_XPLANE > 1
  }
#endif

#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1
  if (LoopNumber % NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT == 0) {
#endif

    // Read input controls and send datas to X-plane
    for (unsigned int i = 0; i < InputControlCount; i++) {
      this->doControlCommandProcess(InputControlList[i]);
    }
#if NUMBER_LOOP_SKIP_FOR_READ_ARDUINO_INPUT > 1
  }
  LoopNumber++;
#endif
}

// Update output control if needed with value of binded input command
void CockpitMainApplication::updateControlWithCommand(
    OutputControlAssociation *oca) {

  XPlaneInputData *inputData = oca->IntputData;
  ArduinoOutputControl *outputControl = oca->Control;

  // get group data of control
  XPGroupDatas *dataXplane =
      CommunicationInterface->GetData(inputData->getGroup());

  // update control with data received from X-plane
  if (dataXplane != NULL) {

    float val = 0;

    if (oca->TransformationFunction == NULL)
      val = dataXplane->data[inputData->getIndex()];
    else
      val = oca->TransformationFunction(dataXplane->data);

    // Update control only if value changed ( setValue == true )
    if (outputControl->setValue(val)) {

      outputControl->WriteOutput();
    }
  }
}

// Read input control and send command to X-plane if needed
void CockpitMainApplication::doControlCommandProcess(
    InputControlAssociation *ica, bool forceSendingCommand) {

  XPlaneOutputCommand *outputCmd = NULL;
  ArduinoControl *inputControl = ica->Control;

  // Read input stat and send command if value changed (ReadInput == true) or if
  // force sending ask (forceSendingCommand == true)
  if (forceSendingCommand || inputControl->ReadInput()) {

    float val = inputControl->getValue();

    if (inputControl->getType() == ArduinoControl::DigitalControl) {

      // Command is defined from control value and InputControlAssociation's
      // command tab
      if (val < MAX_COMMAND_FOR_ONE_CONTROLE)
        outputCmd = ica->OutputCommand[(uint8_t)val];
    } else {
      // Always use first command for analog inputs
      outputCmd = ica->OutputCommand[0];
    }

    // We got a command to send
    if (outputCmd != NULL) {

      switch (outputCmd->getType()) {

      // Key
      case XPlaneOutputCommand::TypeKeyCommand:
        CommunicationInterface->SendKey(outputCmd->toString());
        break;

      // Simple command
      case XPlaneOutputCommand::TypeSimpleCommand:

        CommunicationInterface->SendCommand(outputCmd->toString());

        // Twin command (send 2 commands for one input)
        if (((XPlaneSimpleCommand *)outputCmd)->isTwinCommand()) {
          CommunicationInterface->SendCommand(
              ((XPlaneSimpleCommand *)outputCmd)->toStringSecond());
        }
        break;

      // DREF / DATA command
      case XPlaneOutputCommand::TypeDREFCommand:
        CommunicationInterface->SendDrefCommand(outputCmd->toString(), val);
        break;

      case XPlaneOutputCommand::TypeDATACommand:
        break;

      // System command : internal to library
      case XPlaneOutputCommand::TypeInternalCommand:
        this->doInternalCommande((LibrarySpecialCommand *)outputCmd);
        break;
      }
    }
  }
}

// Reset arduino (function at 0 address = cause arduino reset)
void (*resetArduinoFunction)(void) = 0;

// Deal with library internal command
void CockpitMainApplication::doInternalCommande(LibrarySpecialCommand *cmd) {

  switch (cmd->getCommand()) {

  // Send all command of all (toggle switch and rotary switch) input control
  // even if control was not modified
  case LibrarySpecialCommand::SendAll:
    for (unsigned int i = 0; i < InputControlCount; i++) {

      if (InputControlList[i] != NULL) {

        if (InputControlList[i]->Control->getInputType() ==
                ArduinoInputControl::ITypeRotarySwitch ||
            InputControlList[i]->Control->getInputType() ==
                ArduinoInputControl::ITypeToggleSwitch) {
          this->doControlCommandProcess(InputControlList[i], true);
        }
      }
    }
    break;

  // Reset Arduino board
  case LibrarySpecialCommand::ResetArduino:
    resetArduinoFunction();
    break;

  // Turn on/off all LED output controls (Example : testing panel)
  case LibrarySpecialCommand::AllLedOn:
  case LibrarySpecialCommand::AllLedOff:

    for (unsigned int i = 0; i < OutputControlCount; i++) {

      if (OutputControlList[i] != NULL) {

        ArduinoOutputControl *outputControl = OutputControlList[i]->Control;

        if (outputControl->getOutputType() == ArduinoOutputControl::OTypeLed &&
            outputControl->setValue(
                cmd->getCommand() == LibrarySpecialCommand::AllLedOn ? 1 : 0)) {
          outputControl->WriteOutput();
        }
      }
    }
    break;
  }
}

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

// Init I2C interface if control is defined on slave arduno board (All
// registered control are passed to this methode)
void CockpitMainApplication::CheckAndInitI2CIfNeed(ArduinoControl *ctrl) {

  if (ctrl->isOnSlaveBoard() && !I2CInit) {
    Wire.begin(); // join i2c bus as master
  }
}

#endif
