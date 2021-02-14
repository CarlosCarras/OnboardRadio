/****************************************************************************
* Interpreter.cpp
*
* @about      : interprets the incoming data from the UHF transceiver.
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : February 14, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Interpreter.h"


Interpreter::Interpreter(UHF_Transceiver* transceiver) {
    this->transceiver = transceiver;
}

command Interpreter::getCommand() {
    int n = transceiver->getRxBufferCount();

    if (n != 0) {
        uint8_t* data = transceiver->readNBytes(n);
        packet inbound = composePacket(data, n);
        command incoming_command = interpret_packet(inbound);
    } else {
        return command('\0', '\0');
    }

    return incoming_command;
}

packet Interpreter::composePacket(uint8_t* data_arr, int n) {
    std::string data = (char*)data_arr;

    packet inbound;
    inbound.preamble = data.substr(0,2);
    inbound.data_length = n-4;
    inbound.data = data.substr(2,inbound.data_length);
    inbound.checksum = getChecksum(inbound.data_length+3,1);

    return inbound;
}

command Interpreter::composeCommand(packet inbound) {
    command incoming_command;
    incoming_command.telecommand = inbound.data(0,1);
    incoming_command.param = inbound.data(1);

    return incoming_command;
}