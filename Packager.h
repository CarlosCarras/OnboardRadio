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

struct packet {
    uint16_t preamble;
    int data_length;
    std::string data;
    int checksum;
};

class Packager {
private:
    UHF_Transceiver* transceiver;

    static int getChecksum(const std::string &data);
    int send256Bytes(const std::string &str);
    int transmitByte(uint8_t data);
    int transmitByteTest(uint8_t data);
	packet composePacket(const std::string &data);

public:
    explicit Packager();
    int getNumPackets(const std::string &data);
    int sendString(const std::string &str);
    int sendFile(const std::string &filename);
    int sendPacket(packet outbound);
	
	void debug_toggle(int led);
	void debug_on(int led);
	void debug_off(int led);	
};


#endif //ONBOARDRADIO_PACKAGER_H
