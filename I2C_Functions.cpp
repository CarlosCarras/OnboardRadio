 /****************************************************************************
 * i2c_functions.cpp
 * 
 * Hardware   : ISIS Antenna
 * Manual     : ISIS.ANTS.DS.001, version 3.0
 * About      : The class definition to interface with the UHF transceiver.
 * 
 * Author     : Carlos Carrasquillo
 * Date       : July 16, 2020
 * Modified   : August 20, 2020
 * Proprty of : ADAMUS Lab
 ****************************************************************************/

#include "I2C_Functions.h"

I2C_Functions::I2C_Functions() {
	I2CBus = 0;
	set_address(0);
}

I2C_Functions::I2C_Functions(uint8_t bus, uint8_t device_addr) {
	I2CBus = bus;
	set_address(device_addr);
	std::cout << "New Device Created! Device Address: " << device_addr << std::endl;
}

void I2C_Functions::set_address(uint8_t new_addr) {
	I2CAddr_Write = (new_addr << 1) | 0;
	I2CAddr_Read  = (new_addr << 1) | 1;
	std::cout << "Device Write Address: " << I2CAddr_Write << std::endl;
	std::cout << "Device Read Address: " << I2CAddr_Read << std::endl;
}

uint8_t I2C_Functions::get_address() {
	return (I2CAddr_Write >> 1) & 0x7F;
}

void I2C_Functions::write(uint8_t reg, uint8_t data) {
	int handle = i2c_open(I2CBus);
	uint16_t write_sequence[] = {I2CAddr_Write, reg, data};
	i2c_send_sequence(handle, write_sequence, 3, 0);
	i2c_close(handle);
}

void I2C_Functions::write2(uint8_t reg, uint16_t data) {
	int handle = i2c_open(I2CBus);
	uint8_t byte0 = (data >> 0) & 0xFF;
	uint8_t byte1 = (data >> 8) & 0xFF;
	write(reg, byte1);
	write(reg+1, byte0);
	i2c_close(handle);
}

uint8_t I2C_Functions::read(uint8_t reg) {
	int handle = i2c_open(I2CBus);
	uint16_t read_sequence[] = {I2CAddr_Write, reg, I2C_RESTART, I2CAddr_Read, I2C_READ};
	uint8_t data_received[1] = {0};
	i2c_send_sequence(handle, read_sequence, 5, &data_received[0]);
	i2c_close(handle);

	return data_received[0];
}

uint16_t I2C_Functions::read2(uint8_t reg) {
	int handle = i2c_open(I2CBus);
	uint16_t read_sequence[] = {I2CAddr_Write, reg, I2C_RESTART, I2CAddr_Read, I2C_READ, I2C_READ};
	uint8_t data_received[2] = {0, 0};
	i2c_send_sequence(handle, read_sequence, 6, &data_received[0]);
	i2c_close(handle);

	uint16_t data_read = (data_received[0]<<8) + (data_received[1]);
	return data_read;
}