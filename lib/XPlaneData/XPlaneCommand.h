/**
 * Romgere Cockpit Library - https://github.com/romgere/romgere_cockpit
 * @author jerome@mestres.fr
 */

#ifndef XPLANECOMMANDCLASS_H_INCLUDED
#define XPLANECOMMANDCLASS_H_INCLUDED

#include <Arduino.h>

/******************/
/* OUTPUT COMMAND */
/******************/

// Parent class for all output command (DREF, Key, Data...)
class XPlaneOutputCommand {

public:
  typedef enum {
    TypeKeyCommand = 0,
    TypeSimpleCommand,
    TypeDATACommand,
    TypeDREFCommand,
    TypeInternalCommand,
  } XPlaneCommandType;

private:
  XPlaneCommandType CommandType;
  // float DataGroup;

public:
  XPlaneOutputCommand(XPlaneCommandType type) : CommandType(type){};
  ~XPlaneOutputCommand(){};

  XPlaneCommandType getType() { return CommandType; };

  virtual const char *toString() = 0;
  // virtual float getGroup(){return DataGroup; };
};

// Xplane command key
class XPlaneKeyCommand : public XPlaneOutputCommand {

private:
  char Key;

public:
  XPlaneKeyCommand(const char c);
  ~XPlaneKeyCommand();

  const char *toString();
};

// X-Plane simple command (like "sim/radios/com1_standy_flip")
class XPlaneSimpleCommand : public XPlaneOutputCommand {

private:
  String Command;
  String *SecondCommand;
  bool twinCommand;

public:
  XPlaneSimpleCommand(const char *cmd, const char *cmd2 = NULL);
  ~XPlaneSimpleCommand();

  const char *toString();
  const char *toStringSecond();
  bool isTwinCommand();
};

// X-Plane DATA command (NOT IMPLEMENTED)
class XPlaneDATACommand : public XPlaneOutputCommand {

public:
  XPlaneDATACommand() : XPlaneOutputCommand(TypeDATACommand){};
  ~XPlaneDATACommand(){};

  const char *toString() { return "Not implemented"; };
};

// X-Plane DREF command (NOT IMPLEMENTED)
class XPlaneDREFCommand : public XPlaneOutputCommand {
private:
  String Dref;

public:
  XPlaneDREFCommand(const char *dref)
      : XPlaneOutputCommand(TypeDREFCommand), Dref(dref){};
  ~XPlaneDREFCommand(){};

  const char *toString() { return Dref.c_str(); };
};

/*****************************/
/* SYSTEM (INTERNAL) COMMAND */
/*****************************/

// Special "system" command (internal to library)
class LibrarySpecialCommand : public XPlaneOutputCommand {

public:
  typedef enum {
    SendAll = 0, // Send all command of all (toggle switch and rotary switch)
                 // input control even if control was not modified
    ResetArduino, // Reset Arduino board
    AllLedOn,     // Turn on all LED output controls (Example : testing panel)
    AllLedOff,    // Turn off all LED output controls
  } SpecialCommand;

private:
  SpecialCommand cmd;

public:
  LibrarySpecialCommand(SpecialCommand c)
      : XPlaneOutputCommand(TypeInternalCommand) {
    cmd = c;
  };
  ~LibrarySpecialCommand(){};

  const char *toString();
  SpecialCommand getCommand() { return cmd; };
};

#endif
