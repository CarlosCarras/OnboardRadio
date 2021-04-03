/****************************************************************************
 * Packager.h
 *
 * @author      : Carlos Carrasquillo
 * @contact     : c.carrasquillo@ufl.edu
 * @created     : November 11, 2020
 * @modified    : March 19, 2021
 * @description : controls the transmitting and receiving capabilities of the UHF transceiver over the I2C bus.
 * @hardware    : URTX CPUT Transeiver (Cape Peninsula University of Technology)
 *
 * Property of ADAMUS lab, University of Florida.
 ****************************************************************************/

#ifndef ONBOARDRADIO_PACKAGER_H
#define ONBOARDRADIO_PACKAGER_H


/************************** Includes **************************/
#include <string>
#include "telecommands.h"
#include "UHF_Transceiver.h"


/************************** Defines ***************************/
#define TRANSMIT_PREAMBLE      0x1ACF
#define DATAFIELD_LEN          256      // bytes


/************************** Packager **************************/
class Packager {
private:
    UHF_Transceiver* transceiver;

    static uint8_t getChecksum(const std::string &data);
	packet_t composePacket(const std::string &data);
    int send256Bytes(const std::string &str);
	void transmitString(std::string data, uint8_t str_len);

	void transmitStringTest(std::string data, uint8_t str_len);

public:
    explicit Packager(UHF_Transceiver* transceiver);
    int getNumPackets(const std::string &data);
    int sendString(const std::string &str);
    int sendFile(const std::string &filename);
    int sendPacket(packet_t* outbound);
	
	void debug_toggle(int led);
	void debug_on(int led);
	void debug_off(int led);	
};


#endif //ONBOARDRADIO_PACKAGER_H
