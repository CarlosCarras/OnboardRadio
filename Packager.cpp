/*
 * Packager.cpp
 * @author        : Carlos Carrasquillo
 * @contact       : c.carrasquillo@ufl.edu
 * @date created  : November 11, 2020
 * @date modified : November 11, 2020
 * @description   : controls the transmitting and receiving capabilities of the UHF transceiver over the I2C bus.
 * @hardware      : URTX CPUT Transeiver (Cape Peninsula University of Technology)
 *
 * Property of ADAMUS lab, University of Florida.
 */

#include <iostream>
#include <fstream>
#include "Packager.h"

Packager::Packager() {
    transceiver = new UHF_Transceiver();
}

int Packager::getNumPackets(std::string &data) {
    return (data.length() - 1)/ 256 + 1;
}

int Packager::getChecksum(std::string &data) {
    int sum = 0;

    for (char i : data) {
        sum += (int)i;
    }

    std::stringstream ss;
    std::string out_str = ss.str();
    return sum;
}

Packager::packet Packager::composePacket(std::string &data) {
    packet outbound;
    outbound.checksum = getChecksum(data);
    outbound.data_length = data.length();
    outbound.data = data;
    outbound.preamble = TRANSMIT_PREAMBLE;

    return outbound;
}

int Packager::send256Bytes(std::string &str) {
    if (str.length() > 256) {
        return -1;
    }

    outbound = composePacket(data_substr);
    sendPacket(outbound);

    std::cout << "Outbound Data: " << outbound.data << std::endl;
    std::cout << "Outbound Data Length: " << outbound.data_length << std::endl;

    return 0;
}

int Packager::sendString(std::string &str) {
    int num_packets = getNumPackets(str);

    packet outbound;
    for (int i = 0; i < num_packets; i++) {
        std::string data_substr = str.substr(i*DATAFIELD_LEN, DATAFIELD_LEN);
        send256Bytes(data_substr)
    }
    std::cout << "Number of Packets: " << num_packets << std::endl;

    return 0;
}

int Packager::sendFile(std::string &filename) {
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

        if (i != 0){
            while (i < DATAFIELD_LEN) {
                buffer[i++] = '\0';
            }
            send256Bytes(std::string(buffer));
        }

        outFile.close();
    } else {
        return -1;
    }
    return 0;
}

int Packager::sendPacket(packet outbound) {
    transmitByte((uint8_t)(outbound.preamble >> 8));
    transmitByte((uint8_t)(outbound.preamble & 0xFF));
    transmitByte((uint8_t)outbound.data_length);

    for(char& c : str) {
        transmitByte((uint8_t)c);
    }

    transmitByte((uint8_t)outbound.checksum);
    return 0;
}

int Packager::transmitByte(uint8_t data) {
    transceiver->transmitByte(data);
}

int Packager::transmitByteTest(uint8_t data) {
    std::cout << data << std::endl;
}


