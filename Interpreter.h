/****************************************************************************
* Interpreter.h
*
* @about      : interprets the incoming data from the UHF transceiver.
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : April 28, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#ifndef INTERPRETER_H
#define INTERPRETER_H

/************************** Includes **************************/
#include <string>
#include <stdlib.h>
#include <iostream>
#include "UHF_Transceiver.h"
#include "telecommands.h"


/************************ Interpreter *************************/
class Interpreter {
private:
    UHF_Transceiver* transceiver;
    packet_t composePacket(uint8_t* data, int n);
    command_t composeCommand(packet_t* inbound_packet);
    void backupFile(const std::string& filename);
    int uploadFile(command_t* incoming_command);

    file_t last_file;
    uint8_t packet_cntr;

public:
    explicit Interpreter(UHF_Transceiver* transceiver);
    command_t getCommand();

    /****** Testing ******/
    command_t getCommandTest();
};

#endif //INTERPRETER_H