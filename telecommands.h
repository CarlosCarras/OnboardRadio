/****************************************************************************
* telecommands.h
*
* @about       : a set of comiler directives outlining the telecommand codes
* @author      : Carlos Carrasquillo
* @contact     : c.carrasquillo@ufl.edu
* @date        : February 4, 2021
* @modified    : February 12, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/


#ifndef TELECOMMANDS_H
#define TELECOMMANDS_H

#include <stdint.h>
#include <string>

#define ACKNOWLEDGE              "11001100"
#define ERROR                    "00110010"
#define EOT		     		              4     // end of transmission in ASCII

#define TELECOM_GET_HEALTH       "01001101"
#define TELECOM_OVERRIDE_ANTENNA "01101010"
#define TELECOM_DEBUG_ON         "11110000"
#define TELECOM_DEBUG_OFF        "00001111"
#define TELECOM_DEBUG_TOGGLE     "10101010"

struct packet_t {
    uint16_t preamble;
    uint8_t data_length;
    std::string data;
    uint8_t checksum;
};

struct command_t {
    std::string telecommand;
    std::string params;
};

#endif //TELECOMMANDS_H