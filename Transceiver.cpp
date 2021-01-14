/*
 * Transceiver.cpp
 * @author        : Carlos Carrasquillo
 * @contact       : c.carrasquillo@ufl.edu
 * @date created  : November 11, 2020
 * @date modified : November 11, 2020
 * @description   : controls the transmitting and receiving capabilities of the UHF transceiver over the I2 bus.
 * @hardware      : URTX CPUT Transeiver (Cape Peninsula University of Technology)
 *
 * Property of ADAMUS lab, University of Florida.
 */

#include <iostream>
#include <sstream>
#include "Transceiver.h"

Transceiver::Transceiver() {

}

int Transceiver::getNumPackets(std::string &data) {
    return (data.length() - 1)/ 256 + 1;
}

int Transceiver::getChecksum(std::string &data) {
    int sum = 0;

    for (int i = 0; i < data.length(); i++) {
        sum += (int)data[i];
    }

    std::stringstream ss;
    std::string out_str = ss.str();
    return sum;
}

Transceiver::packet Transceiver::composePacket(std::string &data) {
    packet outbound;
    outbound.checksum = getChecksum(data);
    outbound.data_length = data.length();
    outbound.data = data;
    outbound.preamble = TRANSMIT_PREAMBLE;

    return outbound;
}

int Transceiver::sendData(std::string &data) {
    int num_packets = getNumPackets(data);

    packet outbound;
    for (int i = 0; i < num_packets; i++) {
        std::string data_substr = data.substr(i*256, 256);
        outbound = composePacket(data_substr);

        std::cout << "Outbound Data: " << outbound.data << std::endl;
        std::cout << "Outbound Data Length: " << outbound.data_length << std::endl;
    }
    std::cout << "Number of Packets: " << num_packets << std::endl;

    return 0;
}
