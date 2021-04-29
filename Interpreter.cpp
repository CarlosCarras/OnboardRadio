/****************************************************************************
* Interpreter.cpp
*
* @about      : interprets the incoming data from the UHF transceiver.
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : April 28, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Interpreter.h"
#include <fstream>
#include <cstdio>
#include "ManageHistory.h"


Interpreter::Interpreter(UHF_Transceiver* transceiver) {
    this->transceiver = transceiver;

    last_file.telecommand = 0x00;
    last_file.num_packets = 0;
    packet_cntr = 0;
}

command_t Interpreter::getCommand() {
    int n = transceiver->getRxBufferCount();
    packet_t inbound_packet;
    command_t inbound_command;

    if (n == 0) return {0x00, "\0"};

    uint8_t data[n];
    transceiver->readNBytes(n, data);
    inbound_packet = composePacket(data, n);
    inbound_command = composeCommand(&inbound_packet);
    addToHistory(&inbound_command);

    if (incoming_command.telecommand == TELECOM_UPLOAD_FILE) {
        uploadFile(&incoming_command);
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
    inbound_command.telecommand = (uint8_t)inbound_packet->data.at(0);
    inbound_command.params = inbound_packet->data.substr(1);

    return inbound_command;
}

/*
 * First Packet Params Field:
 * Bytes:   |      1        |       1           |              1              |    1-251    |  1  |  0-250 |
 *          | packet number | number of packets | length of destination field | destination | SOF |  data  |
 *
 * NOTE: Start of File (SOF) must be in the first packet.
 *
 * Other Packets Params Field:
 * Bytes:   |      1        |  1-254 |  1  |
 *          | packet number |  data  | EOF |
 *
 * NOTE: End of File (EOF) must be in the last packet.
 */
int Interpreter::uploadFile(command_t* incoming_command) {
    bool last_packet = false;

    if (incoming_command->params.at(0) == 1) {
        /* this means that the transmission is a new transmission */

        if (packet_cntr != last_file.num_packets) {
            /* this means that not all packets from the last transmission were received */
            std::cout << "ERROR: Not all packets were received. Accepting the new telecommand." << std::endl;
            incoming_command->telecommand = TELECOM_PACKET_LOSS_RESET;
            last_file.sof = 0x00;
            remove(&last_file.dest[0]);
        }

        packet_cntr = 1;
        last_file.telecommand = incoming_command->telecommand;
        last_file.num_packets = (uint8_t)incoming_command->params.at(1);            // number of packets to be transmitted
        last_file.len_dest = (uint8_t)incoming_command->params.at(2);               // destination field
        last_file.dest = incoming_command->params.substr(3,last_file.len_dest);     // bytes is the destination field

        uint8_t startOfData = 3 + last_file.len_dest;
        if (startOfData > DATAFIELD_LEN-1) {
            std::cout << "ERROR: Improper packet format. The data field is said to start at byte " << std::to_string(startOfData) << ", which exceeds " << std::to_string(DATAFIELD_LEN-1) << ". Aborting." << std::endl;
            incoming_command->telecommand = TELECOM_PACKET_FORMAT_ERROR;
            return -1;
        }

        last_file.sof = incoming_command->params.at(startOfData);                   // location of SOF character
        if (last_file.sof != SOF) {
            /* SOF is supposed to by in the first packet but was not found */
            std::cout << "ERROR: Unable to locate the Start of File (SOF) character. Aborting." << std::endl;
            incoming_command->telecommand = TELECOM_PACKET_FORMAT_ERROR;
            return -1;
        }

        std::ofstream newFile(last_file.dest, std::ofstream::trunc);
        if (!newfile.is_open()) {
            /* The destination was not found. */
            std::cout << "Error: The file destination: '" << last_file.dest << "' was not found." << std::endl;
            incoming_command->telecommand = TELECOM_PACKET_FORMAT_ERROR;
            return -1;
        }
        newFile << incoming_command->params.substr(startOfData+1);
        newFile.close();
        return 0;
    }

    if (incoming_command->telecommand != last_file.telecommand) {
        std::cout << "ERROR: The telecommand changed without anticipation." << std::endl;
        incoming_command->telecommand = TELECOM_PACKET_LOSS;
        return -1;
    }

    if (incoming_command->params.at(0) != ++packet_cntr) {
        std::cout << "ERROR: Packet number " << packet_cntr << " was lost." << std::endl;
        incoming_command->telecommand = TELECOM_PACKET_LOSS;
        return -1;
    }

    if (packet_cntr > last_file.num_packets) {
        std::cout << "ERROR: Unexpected number of packets received. Expected " << std::to_string(last_file.num_packets) << ", Received: " << std::to_string(packet_cntr) << "." << std::endl;
        incoming_command->telecommand = ERROR;
        return -1;
    } else if (packet_cntr == last_file.num_packets) {
        if (incoming_command->params.back() != EOF) {
            std::cout << "ERROR: Unable to locate the End of File (EOF) character." << std::endl;
            incoming_command->telecommand = TELECOM_PACKET_LOSS;
            return -1;
        }
        incoming_command->params.pop_back();    // getting rid of EOF character
        last_packet = true;
    }

    std::ofstream file(last_file.dest);
    if (!file.is_open()) {
        /* this means that the destination was not found */
        std::cout << "Error: The file destination: '" << last_file.dest << "' was not found." << std::endl;
        incoming_command->telecommand = ERROR;
        return -1;
    }
    newFile << incoming_command->params.substr(1);
    newFile.close();

    if (last_packet) {
        incoming_command->telecommand = TELECOM_LAST_PACKET_RECEIVED;
    }

    return 0;
}
