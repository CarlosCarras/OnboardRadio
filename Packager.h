/****************************************************************************
 * Packager.h
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

#ifndef ONBOARDRADIO_PACKAGER_H
#define ONBOARDRADIO_PACKAGER_H


/************************** Includes **************************/
#include <string>
#include "telecommands.h"
#include "UHF_Transceiver.h"


/************************** Defines ***************************/
#define TRANSMIT_PREAMBLE      0x1ACF
#define DATAFIELD_LEN          256      // bytes
#define PACKET_OVERHEAD 	   5 		// bytes


/************************** Packager **************************/
class Packager {
private:
    UHF_Transceiver* transceiver;

    packet_t composePacket(const std::string &data);
    int sendPacket(packet_t* outbound);
    static uint8_t getChecksum(const std::string &data);
    int getNumPackets(const std::string &data);
    int sendData(uint8_t telecom, const std::string &data);
    int send256Bytes(const std::string &str);

    /* Test Functions */
	void transmitStringTest(std::string data, uint8_t str_len);

public:
    explicit Packager(UHF_Transceiver* transceiver);

    int sendString(const std::string &str);
    int sendFile(const std::string &filename);

    /* Test Functions */
	void debug_toggle(int led);
	void debug_on(int led);
	void debug_off(int led);	
};


#endif //ONBOARDRADIO_PACKAGER_H
