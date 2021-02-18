/****************************************************************************
 * Packager.cpp
 *
 * @author      : Carlos Carrasquillo
 * @contact     : c.carrasquillo@ufl.edu
 * @created     : November 11, 2020
 * @modified    : February 12, 2021
 * @description : controls the transmitting and receiving capabilities of the UHF transceiver over the I2C bus.
 * @hardware    : URTX CPUT Transeiver (Cape Peninsula University of Technology)
 *
 * Property of ADAMUS lab, University of Florida.
 ****************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include "Packager.h"

Packager::Packager(UHF_Transceiver* transceiver) {
    this->transceiver = transceiver;
}

int Packager::getNumPackets(const std::string &data) {
    return (data.length() - 1)/ 256 + 1;
}

uint8_t Packager::getChecksum(const std::string &data) {
    uint8_t sum = 0;

    for (char i : data) {
        sum += (uint8_t)i;
    }

    return sum;
}

packet_t Packager::composePacket(const std::string &data) {
    packet_t outbound;
    outbound.checksum = getChecksum(data);
    outbound.data_length = (uint8_t)data.length();
    outbound.data = data;
    outbound.preamble = TRANSMIT_PREAMBLE;

    return outbound;
}

int Packager::send256Bytes(const std::string &str) {
    if (str.length() > 256) {
        return -1;
    }

    packet_t outbound = composePacket(str);
    sendPacket(outbound);

    std::cout << "Outbound Data: " << outbound.data << std::endl;
    std::cout << "Outbound Data Length: " << outbound.data_length << std::endl;

    return 0;
}

int Packager::sendString(const std::string &str) {
    int num_packets = getNumPackets(str);

    for (int i = 0; i < num_packets; i++) {
        std::string data_substr = str.substr(i*DATAFIELD_LEN, DATAFIELD_LEN);
        send256Bytes(data_substr);
    }
    std::cout << "Number of Packets: " << num_packets << std::endl;

    return 0;
}

int Packager::sendFile(const std::string &filename) {
    char buffer[DATAFIELD_LEN];
    std::string str;
    unsigned int i = 0;

    std::ifstream outFile(filename);
    char c;

    if (outFile.is_open()) {
        while (outFile.get(c)) {
                buffer[i++] = c;
                if (i >= DATAFIELD_LEN) {
                    send256Bytes(std::string(buffer));
                    i = 0;
                }
        }

        std::string last_packet = std::string(buffer);
        send256Bytes(last_packet.substr(0, i));

        outFile.close();
    } else {
        return -1;
    }
    return 0;
}

int Packager::sendPacket(packet_t outbound) {
    std::string data;

    data += (char)(outbound.preamble >> 8);
    data += (char)(outbound.preamble & 0xFF);
    data += (char)outbound.data_length;
    data += outbound.data;
    data += (char)outbound.checksum;

    transmitStringTest(data);       // REMOVE 'Test' AFTER TESTING
    return 0;
}

void Packager::transmitByte(uint8_t data) {
    transceiver->sendByte(data);
}

void Packager::transmitString(std::string data) {
    transceiver->sendString(data);
}

/************** Debug ***************/

void Packager::debug_toggle(int led) {
	transceiver->ledToggle(led);
}

void Packager::debug_on(int led) {
	transceiver->ledOn(led);
}

void Packager::debug_off(int led) {
	transceiver->ledOff(led);
}

void Packager::transmitStringTest(std::string data) {
    transceiver->sendStringTest(data);
}