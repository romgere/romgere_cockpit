/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef ARDUINOCONTROLCLASS_H_INCLUDED
#define ARDUINOCONTROLCLASS_H_INCLUDED

#include <Arduino.h>

#include "../Config/MainConfig.h"

// Parent class for ALL Arduino input/output control
// Virtual class
class ArduinoControl {

public:
  // Control type (Analog/digital input/outpu)
  typedef enum { AnalogControl = 0, DigitalControl } ArduinoControlType;

protected:
  // Control direction
  typedef enum {
    InputControl = 0, // switch, rotary encoder, ...
    OutputControl     // LED, Servo, ...
  } ArduinoControlDir;

private:
  // Definition of type and direction
  ArduinoControlDir ControlDir;
  ArduinoControlType ControlType;

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoControl(ArduinoControlDir dir, ArduinoControlType type,
                 bool onSlaveBoard, uint8_t boardAdresse)
      : ControlDir(dir), ControlType(type), ControleOnSlaveBoard(onSlaveBoard),
        SlaveBoardAddress(boardAdresse){};
#else
  ArduinoControl(ArduinoControlDir dir, ArduinoControlType type)
      : ControlDir(dir), ControlType(type){};
#endif

  virtual ~ArduinoControl(){};

  ArduinoControlType getType() { return ControlType; };

  // Read input state. Return true if control need to send command (value
  // changed)
  virtual bool ReadInput() = 0;

  // Write on output PIN according to it's current value
  virtual void WriteOutput() = 0;

  // Get current for control
  virtual float getValue() { return 0; };

  // Set value for control (return true if value was modified, FALSE otherwise)
  virtual bool setValue(float val) { return false; };

#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE

private:
  bool ControleOnSlaveBoard;
  uint8_t SlaveBoardAddress;

public:
  // True if control is on slave board, false otherwise
  bool isOnSlaveBoard() { return ControleOnSlaveBoard; };

  // Ici on va définir les méthode permettant de lire/ecrire les entrée Arduino
  // Permet de s'affranchir dans les classes de controles dérivée de la façon
  // dont ces actions sont faites (Par exemple pour une carte Esclave en I2C,
  //cette méthode ne va pas appelée directement les fonction Arduino classique
  //analogRead, analogWrite...)

  // These methods are used to read/write on PIN with slave/master board
  // abstraction Direct call to arduino read/write for input/output on master
  // board, and use I2C bus to write/read on slave board input/output
  void _pinMode(uint8_t p, uint8_t m);
  void _digitalWrite(uint8_t p, uint8_t v);
  int _digitalRead(uint8_t p);
  int _analogRead(uint8_t p);
  void _analogWrite(uint8_t p, int v);
#endif
};

// Don't use above methods if multi arduino board not enable
#ifndef ACTIVE_MULTI_ARDUINO_BOARD_MODE
#define _pinMode(a, b) pinMode(a, b)
#define _digitalWrite(a, b) digitalWrite(a, b)
#define _digitalRead(a) digitalRead(a)
#define _analogRead(a) analogRead(a)
#define _analogWrite(a, b) analogWrite(a, b)
#endif

// Parent class for ALL INPUT control
class ArduinoInputControl : public ArduinoControl {

public:
  // Input control type
  typedef enum {
    ITypePushButton = 0,
    ITypeToggleSwitch,
    ITypeRotarySwitch,
    ITypeRotaryEncode,
  } ArduinoInputControlType;

private:
  ArduinoInputControlType inputType;

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoInputControl(ArduinoControlType type, ArduinoInputControlType iType,
                      bool onSlaveBoard, uint8_t boardAdresse)
      : ArduinoControl(InputControl, type, onSlaveBoard, boardAdresse),
        inputType(iType){};
#else
  ArduinoInputControl(ArduinoControlType type, ArduinoInputControlType iType)
      : ArduinoControl(InputControl, type), inputType(iType){};
#endif
  ~ArduinoInputControl(){};
  void WriteOutput(){};

  ArduinoInputControlType getInputType() { return this->inputType; }
};

// Parent class for ALL OUTPUT control
class ArduinoOutputControl : public ArduinoControl {

public:
  // Output control type
  typedef enum {
    OTypeLed = 0,
    OTypeServo, // Not yet implemented
  } ArduinoOutputControlType;

private:
  ArduinoOutputControlType outputType;

public:
#ifdef ACTIVE_MULTI_ARDUINO_BOARD_MODE
  ArduinoOutputControl(ArduinoControlType type, ArduinoOutputControlType oType,
                       bool onSlaveBoard, uint8_t boardAdresse)
      : ArduinoControl(OutputControl, type, onSlaveBoard, boardAdresse),
        outputType(oType){};
#else
  ArduinoOutputControl(ArduinoControlType type, ArduinoOutputControlType oType)
      : ArduinoControl(OutputControl, type), outputType(oType){};
#endif
  ~ArduinoOutputControl(){};
  bool ReadInput() { return false; };

  ArduinoOutputControlType getOutputType() { return this->outputType; }
};

#endif // ARDUINOCONTROLCLASS_H_INCLUDED
