/*
 * Packager.h
 * @author        : Carlos Carrasquillo
 * @contact       : c.carrasquillo@ufl.edu
 * @date created  : November 11, 2020
 * @date modified : November 11, 2020
 * @description   : controls the transmitting and receiving capabilities of the UHF transceiver over the I2C bus.
 * @hardware      : URTX CPUT Transeiver (Cape Peninsula University of Technology)
 *
 * Property of ADAMUS lab, University of Florida.
 */

#ifndef ONBOARDRADIO_PACKAGER_H
#define ONBOARDRADIO_PACKAGER_H

#include "UHF_Transceiver.h"

#define TRANSMIT_PREAMBLE      0x1ACF
#define DATAFIELD_LEN          256      // bytes

class Packager {
private:
    UHF_Transceiver* transceiver;

    static int getChecksum(std::string &data);
    int send256Bytes(std::string &str);
    int transmitByte(uint8_t data);
    int transmitByteTest(uint8_t data);

public:
    struct packet {
        uint16_t preamble;
        int data_length;
        std::string data;
        int checksum;
    };
    explicit Packager();
    int getNumPackets(std::string &data);
    static packet composePacket(std::string &data);
    int sendString(std::string &str);
    int sendFile(std::string &filename);
    int sendPacket(packet outbound);
};


#endif //ONBOARDRADIO_PACKAGER_H
