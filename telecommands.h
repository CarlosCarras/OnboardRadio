/****************************************************************************
* telecommands.h
*
* @about       : a set of comiler directives outlining the telecommand codes
* @author      : Carlos Carrasquillo
* @contact     : c.carrasquillo@ufl.edu
* @date        : February 4, 2021
* @modified    : March 19, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/


#ifndef TELECOMMANDS_H
#define TELECOMMANDS_H

#include <stdint.h>
#include <string>

#define ACKNOWLEDGE              0x40
#define ERROR                    0x32

#define TELECOM_UPLOAD_GUIDANCE	 0xC9
#define TELECOM_GET_HEALTH       0x4C
#define TELECOM_OVERRIDE_ANTENNA 0x6A
#define TELECOM_DEBUG_ON         0xF0
#define TELECOM_DEBUG_OFF        0x0F
#define TELECOM_DEBUG_TOGGLE     0xAA

#define SOF 					 0x02     // start of text in ASCII
#define EOT		     		     0x03     // end of text in ASCII

struct packet_t {
    uint16_t preamble;
    uint8_t data_length;
    std::string data;
    uint8_t checksum;
};

struct command_t {
    uint8_t telecommand;
    std::string params;
};

struct file_t {
	uint8_t telecommand;
	uint8_t len_dest;
	std::string dest;
	char sof;					// start of file
	std::string data;
	char eof;					// end of file
};

#endif //TELECOMMANDS_H