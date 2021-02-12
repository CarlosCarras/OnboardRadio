 /****************************************************************************
 * i2c_functions.h
 * 
 * Dependencies : lsquaredc.c\h
 * About      	: Basic i2c read/write functions for 1 and 2 bytes.
 * 
 * Author     	: Carlos Carrasquillo
 * Date       	: August 20, 2020
 * Modified   	: August 22, 2020
 * Proprty of 	: ADAMUS Lab
 ****************************************************************************/

#ifndef I2C_FUNCTIONS
#define I2C_FUNCTIONS


/************************** Includes **************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdint.h>
#include "lsquaredc.h"


/*************************** Defines ***************************/

#define BIT_VAL(x, bit) (x & (0x0001 << bit)) >> bit  			// gets bit value of 'x' at 'bit'						
#define BIT_SET(x, bit) ((x) |= (1ULL<< bit))
#define BIT_CLEAR(x, bit) ((x) &= ~(1ULL<<(bit)))
#define BIT_TOGGLE(x, bit) (x ^ (1 << bit));

#define NO_PARAMETER 0x00

/************************** Functions **************************/

class I2C_Functions {
private:
	uint8_t I2CBus, I2CAddr_Write, I2CAddr_Read;

public:  
	I2C_Functions();
	I2C_Functions(uint8_t bus, uint8_t device_addr);
	void set_address(uint8_t new_addr);							// sets the device address
	uint8_t get_address();										// fetches the device address

	void write(uint8_t reg, uint8_t data);						// writes 1 byte of data into register
	void write2(uint8_t reg, uint16_t data);					// writes 2 bytes of data into consecutive registers
	void writen(uint8_t reg, uint8_t* data, int n);				// wrotes n bytes of data into conecutive register
	uint8_t read(uint8_t reg);									// reads 1 byte of data from register
	uint16_t read2(uint8_t reg);								// reads 2 bytes of data from consecutive registers
	uint8_t* readn(uint8_t reg, int n);							// reads n bytes of data from consecutive registers
	
	void print_uint8(std::string descriptor, uint8_t data);
};

#endif // I2C_FUNCTIONS
