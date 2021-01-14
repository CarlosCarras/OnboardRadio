/*
 * Transceiver.h
 * @author        : Carlos Carrasquillo
 * @contact       : c.carrasquillo@ufl.edu
 * @date created  : November 11, 2020
 * @date modified : November 11, 2020
 * @description   : controls the transmitting and receiving capabilities of the UHF transceiver over the I2 bus.
 * @hardware      : URTX CPUT Transeiver (Cape Peninsula University of Technology)
 *
 * Property of ADAMUS lab, University of Florida.
 */

#ifndef ONBOARDRADIO_TRANSCEIVER_H
#define ONBOARDRADIO_TRANSCEIVER_H

#define TRANSMIT_PREAMBLE      0x1ACF

class Transceiver {
private:
    int getChecksum(std::string &data);

public:
    struct packet {
        int preamble;
        int data_length;
        std::string data;
        int checksum;
    };
    explicit Transceiver();
    int getNumPackets(std::string &data);
    packet composePacket(std::string &data);
    int sendData(std::string &data);
};


#endif //ONBOARDRADIO_TRANSCEIVER_H
