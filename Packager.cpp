/****************************************************************************
 * Packager.cpp
 *
 * @author      : Carlos Carrasquillo
 * @contact     : c.carrasquillo@ufl.edu
 * @created     : November 11, 2020
 * @modified    : April 29, 2021
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

int Packager::sendString(const std::string &str) {
    int status = sendData(TELECOM_DOWNLINK_STRING, str);
    return status;
}

int Packager::sendFile(const std::string &filename) {
    std::ifstream outFile(filename);

    if (outFile.is_open()) {
        /* saving the contents of the file to the string 'buffer' */
        outFile.seekg(0, std::ios::end);
        size_t size = outFile.tellg();
        std::string buffer(size, ' ');
        outFile.seekg(0);
        outFile.read(&buffer[0], size);
        outFile.close();

        sendData(TELECOM_DOWNLINK_FILE, buffer);

        return 0;
    }

    /* the file was unavailable, sending an error */
    std::string err;
    err += (char)TELECOM_FILE_UNAVAILABLE;
    sendString(err);

    return -1;
}

packet_t Packager::composePacket(const std::string &data) {
    packet_t outbound;
    outbound.checksum = getChecksum(data);
    outbound.data_length = (uint8_t)data.length()-1;
    outbound.data = data;
    outbound.preamble = TRANSMIT_PREAMBLE;

    return outbound;
}

int Packager::sendPacket(packet_t* outbound) {
    std::string data;

    data += (char)(outbound->preamble >> 8);
    data += (char)(outbound->preamble & 0xFF);
    data += (char)outbound->data_length;
    data += outbound->data;
    data += (char)outbound->checksum;

    uint8_t str_len = outbound->data_length + PACKET_OVERHEAD;
    transceiver->sendString(data, str_len);
    return 0;
}

uint8_t Packager::getChecksum(const std::string &data) {
    uint8_t sum = 0;

    for (char i : data) {
        sum += (uint8_t)i;
    }

    return sum;
}

int Packager::getNumPackets(const std::string &data) {
    int len = data.length() + 1;                // (length of data) + 1 byte (number of packets' field in packet #1)
    return (len - 1) / (DATAFIELD_LEN - 2) + 1; // 256 bytes (in AX.25 frame) - 1 byte (telecom), - 1 byte (packet number) = 254
}

/*
 * First Packet:
 * Bytes:        1      |      1        |       1           |  0-253 |
 *          telecommand | packet number | number of packets |  data  |
 *
 *
 * Other Packets:
 * Bytes:        1      |      1        |  1-254 |
 *          telecommand | packet number |  data  |
 *
 */
int Packager::sendData(uint8_t telecom, const std::string &data) {
    int num_packets = getNumPackets(data);

    /* sending the first packet */
    std::string data_out;
    int first_packet_data_len = DATAFIELD_LEN-3;
    data_out += (char)telecom;
    data_out += '1';
    data_out += (char)num_packets;
    data_out += data.substr(0, first_packet_data_len);
    send256Bytes(data_out);

    for (int i = 0; i < num_packets-1; i++) {
        data_out = data.substr(i*DATAFIELD_LEN + first_packet_data_len, DATAFIELD_LEN);
        send256Bytes(data_out);
    }

    std::cout << "Number of Packets: " << num_packets << std::endl;
    return 0;
}

int Packager::send256Bytes(const std::string &str) {
    if (str.length() > 256) {
        std::cout << "ERROR: Attempting to send more than 256 bytes." << std::endl;
        return -1;
    }

    packet_t outbound = composePacket(str);
    sendPacket(&outbound);

    std::cout << "Outbound Data: " << outbound.data << std::endl;
    std::cout << "Outbound Data Length: " << outbound.data_length << std::endl;

    return 0;
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

void Packager::transmitStringTest(std::string data, uint8_t str_len) {
    transceiver->sendStringTest(data, str_len);
}