/****************************************************************************
* telecommands.h
*
* @about       : a set of comiler directives outlining the telecommand codes
* @author      : Carlos Carrasquillo
* @contact     : c.carrasquillo@ufl.edu
* @date        : February 4, 2021
* @modified    : April 28, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/


#ifndef TELECOMMANDS_H
#define TELECOMMANDS_H

#include <stdint.h>
#include <string>

#define BACKUP_EXT ".backup"

/* Uplinked Commands */
#define TELECOM_UPLOAD_FILE	         0xF0
#define TELECOM_UNDO_UPLOAD          0xF5
#define TELECOM_GET_HISTORY          0x12
#define TELECOM_GET_HEALTH           0x4C
#define TELECOM_OVERRIDE_ANTENNA     0x6A
#define TELECOM_DEBUG_ON             0xE0
#define TELECOM_DEBUG_OFF            0x0F
#define TELECOM_DEBUG_TOGGLE         0xAA

/* Downlinked Commands */
#define ACKNOWLEDGE                  0x40
#define TELECOM_LAST_PACKET_RECEIVED 0x11
#define TELECOM_DOWNLINK_FILE		 0x45
#define TELECOM_DOWNLINK_STRING      0x46

/* Downlinked Errors */
#define ERROR                        0x32
#define TELECOM_PACKET_LOSS          0xE6
#define TELECOM_PACKET_LOSS_RESET    0xE7
#define TELECOM_PACKET_FORMAT_ERR    0xE8
#define TELECOM_FILE_UNAVAILABLE     0xE9

/* File Markers */
#define SOF 					     0x02     // start of text in ASCII
#define EOT		     		         0x03     // end of text in ASCII

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
	uint8_t num_packets;
	uint8_t len_dest;
	std::string dest;
	std::string data;
};

#endif //TELECOMMANDS_H