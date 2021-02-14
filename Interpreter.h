/****************************************************************************
* Interpreter.h
*
* @about      : interprets the incoming data from the UHF transceiver.
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : February 14, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#ifndef INTERPRETER_H
#define INTERPRETER_H

/************************** Includes **************************/
#include <string>
#include "telecommands.h"


/************************ Interpreter *************************/
class Interpreter {
private:
    UHF_Transceiver* transceiver;
    packet composePacket(uint8_t* data, int n);
    command composeCommand(packet inbound);

public:
    explicit Interpreter(UHF_Transceiver* transceiver);
    std::string getCommand();
};

#endif //INTERPRETER_H
