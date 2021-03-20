/****************************************************************************
* Interpreter.cpp
*
* @about      : interprets the incoming data from the UHF transceiver.
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : February 18, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Interpreter.h"


Interpreter::Interpreter(UHF_Transceiver* transceiver) {
    this->transceiver = transceiver;
}

command_t Interpreter::getCommand() {
    int n = transceiver->getRxBufferCount();
    packet_t inbound_packet;
    command_t inbound_command;

    if (n != 0) {
        uint8_t* data = transceiver->readNBytes(n);
        inbound_packet = composePacket(data, n);
        inbound_command = composeCommand(&inbound_packet);
    } else {
        return {0x00, "\0"};
    }

    return inbound_command;
}

packet_t Interpreter::composePacket(uint8_t* data_arr, int n) {
    std::string data = (char*)data_arr;

    packet_t inbound_packet;
    inbound_packet.preamble = (data_arr[0] << 8) | (data_arr[1] & 0xFF);
    inbound_packet.data_length = n-5;
    inbound_packet.data = data.substr(3,inbound_packet.data_length);
    inbound_packet.checksum = data_arr[n-1];

    return inbound_packet;
}

command_t Interpreter::composeCommand(packet_t* inbound_packet) {
    command_t inbound_command;
    inbound_command.telecommand = (uint8_t)(inbound_packet->data.substr(0,1));
    inbound_command.params = inbound_packet->data.substr(1);

    return inbound_command;
}