 /****************************************************************************
 * UHF_transceiver.cpp
 * 
 * Hardware   : UHF Transceiver
 * Manual     : USM-01-00097 User Manual Rev. C
 * About      : The class definition to interface with the UHF transceiver.
 * 
 * Author     : Carlos Carrasquillo
 * Date       : August 20, 2020
 * Modified   : August 22, 2020
 * Proprty of : ADAMUS Lab
 ****************************************************************************/

#include "UHF_transceiver.h"


UHF_Transceiver::UHF_Transceiver(uint8_t bus) {
	i2c = I2C_Functions(bus, TRANSCEIVER_I2C_ADDR);
}

uint8_t UHF_Transceiver::getModemConfig() {
	uint8_t config = i2c.read(MODEM_CONFIG);
	config &= 0b11;

	std::string out_str = "Modulation Scheme: ";

	if      (config == 0b01) out_str += "9600 bps GMSK downlink, 1200 bps AFSK uplink.\n";
	else if (config == 0b10) out_str += "1200 bps AFSK downlink, 9600 bps GMSK uplink.\n";
	else if (config == 0b11) out_str += "9600 bps GMSK downlink and uplink.\n";
	else					 out_str += "ERROR: There was an error in the reception of the data.\n";

	std::cout << out_str << std::endl;
	return config;
}

void UHF_Transceiver::setModemConfig(uint8_t config) {
	uint8_t command;

	std::string out_str = "Modulation scheme switched to: ";

	switch (config) {
		case 0:  
			out_str += "9600 bps GMSK downlink, 1200 bps AFSK uplink.\n";
			command = config;
			break;
		case 1: 
			out_str += "1200 bps AFSK downlink, 9600 bps GMSK uplink.\n";
			command = config; 
			break;
		case 2: 
			out_str += "600 bps GMSK downlink and uplink.\n";
			command = config; 
			break;
		default: 
			std::cout << "ERROR: Modulation scheme was not appropriate.\n";
			return;
	}

	i2c.write(MODEM_CONFIG, command);
	std::cout << out_str << std::endl;
}

void UHF_Transceiver::setTransmissionDelay(uint8_t delay) {
	i2c.write(AX25_TX_DELAY, delay);
}

uint8_t UHF_Transceiver::getTransmissionDelay() {
	return i2c.read(AX25_TX_DELAY);
}

void UHF_Transceiver::setSyncBytes(uint8_t val) {
	i2c.write(SYNC_BYTES, val);
}

uint8_t UHF_Transceiver::getSyncBytes() {
	return i2c.read(SYNC_BYTES);
}

void UHF_Transceiver::transmitByte(uint8_t data) {
	while(!transmitReady());
	i2c.write(TX_DATA, data);
}

uint8_t UHF_Transceiver::getBeaconCtrl() {
	uint8_t status = i2c.read(BEACON_CTRL);
	std::string out_str = "Beacon Status:\n";

	if (BIT_VAL(status, 0))  out_str += "\tBeacon is currently enabled.\n";
					    else out_str += "\tBeacon is currently disabled.\n";

	std::cout << out_str << std::endl;
	return status;
}

void UHF_Transceiver::clearBeaconData() {
	uint8_t status = getBeaconCtrl();
	uint8_t config = BIT_SET(status, 1);
	i2c.write(BEACON_CTRL, config);				// automatically cleared after data is cleared
}

void UHF_Transceiver::beaconEnable(bool enable) {
	uint8_t status = getBeaconCtrl();
	uint8_t config; 

	if (enable) config = BIT_SET(status, 0);
	else 	    config = BIT_CLEAR(status, 0);

	i2c.write(BEACON_CTRL, config);
}

void UHF_Transceiver::enableBeacon() {
	beaconEnable(1);
}

void UHF_Transceiver::disableBeacon() {
	beaconEnable(0);
}

void UHF_Transceiver::setBeaconData(uint8_t data) {
	i2c.write(BEACON_DATA, data);
}

uint8_t UHF_Transceiver::getPAPower() {
	uint8_t power = i2c.read(PA_POWER_LVL);
	power &= 0b11;

	std::string out_str = "Power Amplifier Power Level: ";

	if      (power == 0b00) out_str+= "27 dBm (0.5 W).\n";
	else if (power == 0b01) out_str += "30 dBm (1 W).\n";
	else if (power == 0b10) out_str += "33 dBm (2 W).\n";
	else 	  			 	out_str += "Inhibit (0 dBm/W).\n";

	std::cout << out_str << std::endl;
	return power;
}

void UHF_Transceiver::setPAPower(uint8_t config) {
	uint8_t command;

	std::string out_str = "Power Amplifier power level switched to: ";

	switch (config) {
		case 0: 
			out_str += "27 dBm (0.5 W).\n";
			command = config; 
			break;
		case 1: 
			out_str += "30 dBm (1 W).\n";
			command = config; 
			break;
		case 2: 
			out_str += "33 dBm (2 W).\n";
			command = config; 
			break;
		case 3:
			out_str += "Inhibit (0 dBm/W).\n";
			command = config; 
		default: 
			std::cout << "ERROR: Modulation scheme was not appropriate.\n" << std::endl;
			return;
	}

	i2c.write(PA_POWER_LVL, command);
	std::cout << out_str << std::endl;
}

uint16_t UHF_Transceiver::getRxFreqOffset() {
	uint16_t offset = i2c.read2(RX_OFFSET);
	offset &= 0x3FF;
	return offset;
}

void UHF_Transceiver::setRxFreqOffset(uint16_t offset) {
	if (offset > 511) {
		std::cout << "ERROR: Rx frequency offset is larger than 511." << std::endl;
	}
	i2c.write2(RX_OFFSET, offset);
}

void UHF_Transceiver::setRxFreq(uint16_t freq) {
	if (freq > 440 || freq < 430) {
		std::cout <<  "ERROR: The desired receiving frequency is outside of the bounds of 430 MHHz and 440 MHz." << std::endl;
	}
	uint16_t offset = (freq - 430) * 80;		// pp. 22
	setRxFreqOffset(offset);
}

uint16_t UHF_Transceiver::getRxFreq() {
	uint16_t offset = getRxFreqOffset();
	uint16_t freq = (offset + 430) * 0.0125;											// pp. 22
	return freq;
}

uint16_t UHF_Transceiver::getTxFreqOffset() {
	uint16_t offset = i2c.read2(TX_OFFSET);
	offset &= 0x1FF;
	return offset;
}

void UHF_Transceiver::setTxFreqOffset(uint16_t offset) {
	if (offset > 511) {
		std::cout << "ERROR: Tx frequency offset is larger than 511." << std::endl;
	}
	i2c.write2(TX_OFFSET, offset);
}

void UHF_Transceiver::setTxFreq(uint16_t freq) {
	if (freq > 440 || freq < 430) {
		std::cout <<  "ERROR: The desired transmission frequency is outside of the bounds of 430 MHHz and 440 MHz." << std::endl;
	}
	uint16_t offset = (freq - 430) * 40;												// pp. 22
	setTxFreqOffset(offset);
}

uint16_t UHF_Transceiver::getTxFreq() {
	uint16_t offset = getTxFreqOffset();
	uint16_t freq = (offset + 430) * 0.025;												// pp. 22
	return freq;
}

void UHF_Transceiver::setInitialTimeout(uint8_t timeout) {
	i2c.write(INITIAL_I2C_TIMEOUT, timeout);
}

uint8_t UHF_Transceiver::getInitialTimeout() {
	return i2c.read(INITIAL_I2C_TIMEOUT);
}

void UHF_Transceiver::setRecurringTimeout(uint8_t timeout) {
	i2c.write(RECURRING_I2C_TIMEOUT, timeout);
}

uint8_t UHF_Transceiver::getRecurringTimeout() {
	return i2c.read(RECURRING_I2C_TIMEOUT);
}

uint8_t UHF_Transceiver::getDebug() {
	uint8_t status = i2c.read(DEBUG);
	status &= 0x7;

	std::string out_str = "Debug Status:\n";

	if (BIT_VAL(status, 2)) out_str += "\tThe lock status of the receive and transmit synthesizers is ENABLED.\n";
					   else out_str += "\tThe lock status of the receive and transmit synthesizers is DISABLED.\n";
	if (BIT_VAL(status, 1)) out_str += "\tLED 1 is ON.\n";
					   else out_str += "\tLED 1 is OFF.\n";
	if (BIT_VAL(status, 0)) out_str += "\tLED 0 is ON.\n";
					   else out_str += "\tLED 0 is OFF.\n";

	std::cout << out_str << std::endl;
	return status;
}

void UHF_Transceiver::ledOn(int led) {
	if (led == 1 || led == 0) {
		uint8_t status = getDebug();
		BIT_SET(status, led);
		i2c.write(DEBUG, status);
		return;
	}
	std::cout << "ERROR: Invalid LED." << std::endl;
}

void UHF_Transceiver::ledOff(int led) {
	if (led == 1 || led == 0) {
		uint8_t status = getDebug();
		BIT_CLEAR(status, led);
		i2c.write(DEBUG, status);
		return;
	}
	std::cout << "ERROR: Invalid LED." << std::endl;
}

void UHF_Transceiver::ledToggle(int led) {
	if (led == 1 || led == 0) {
		uint8_t status = getDebug();
		BIT_TOGGLE(status, led);
		i2c.write(DEBUG, status);
		return;
	}
	std::cout << "ERROR: Invalid LED." << std::endl;
}

void UHF_Transceiver::reset() {
	i2c.write(RESET, 0x00);
}

uint8_t UHF_Transceiver::getMode() {
	uint8_t config = i2c.read(TRANSPARENT_MODE);
	config &= 0xF;

	std::string out_str = "Operation Mode: ";

	if      (config == 0x06) out_str += "AX.25.\n";
	else if (config == 0x0D) out_str += "Transparent: convolutional encoder enabled.\n";
	else if (config == 0x05) out_str += "Transparent: convolutional encoder disabled.\n";
	else					 out_str += "ERROR: There was an error in the reception of the data.\n";

	std::cout << out_str << std::endl;
	return config;
}

void UHF_Transceiver::setMode(uint8_t config) {
	uint8_t command;

	std::string out_str = "Operation Mode set to: ";

	switch (config) {
		case 0: 
			out_str += "AX.25.\n";
			command = 0x06; 
			break;
		case 1: 
			out_str += "Transparent: convolutional encoder enabled.\n";
			command = 0x0D; 
			break;
		case 2: 
			out_str += "Transparent: convolutional encoder disabled.\n";
			command = 0x05; 
			break;
		default: 
			std::cout << "ERROR: Selected operation mode was not appropriate.\n";
			return;
	}

	i2c.write(TRANSPARENT_MODE, command);
	std::cout << out_str << std::endl;
}

uint16_t UHF_Transceiver::getTxThreshold() {
	return i2c.read2(ALMOST_EMPTY_THRESHOLD);
}

void UHF_Transceiver::setTxThreshold(uint16_t threshold) {
	if (threshold > 8191) {
		std::cout << "ERROR: The desired transmit ready threshold must be less than 8191.\n" << std::endl;
		return;
	}

	i2c.write2(ALMOST_EMPTY_THRESHOLD, threshold);
}

uint8_t UHF_Transceiver::getPAOffDelayGMSK() {
	return i2c.read(PTT_OFF_DELAY_GMSK);
}

void UHF_Transceiver::setPAOffDelayGMSK(uint8_t delay) {
	i2c.write(PTT_OFF_DELAY_GMSK, delay);
}

uint8_t UHF_Transceiver::getPAOffDelayAFSK() {
	return i2c.read(PTT_OFF_DELAY_AFSK);
}

void UHF_Transceiver::setPAOffDelayAFSK(uint8_t delay) {
	i2c.write(PTT_OFF_DELAY_AFSK, delay);
}

uint8_t UHF_Transceiver::getFirmware() {
	uint8_t version = i2c.read(FIRMWARE_VERSION);
	uint8_t major = (version >> 4) & 0xF;
	uint8_t minor = (version >> 0) & 0xF;

	std::cout << "Software Version: " << major << "." << minor << std::endl;
	return version;
}

uint8_t UHF_Transceiver::getReadySignals() {
	return i2c.read(READY_SIGNALS);
}

bool UHF_Transceiver::transmitReady() {
	uint8_t status = getReadySignals();
	bool ready = BIT_VAL(status, 0);

	std::string out_str = "Tx Status: ";
	if (ready) out_str += "Ready to transmit.\n";
	else 	   out_str += "Not ready to transmit.\n";

	std::cout << out_str << std::endl;
	return ready;
}

bool UHF_Transceiver::receiveReady() {
	uint8_t status = getReadySignals();
	bool ready = BIT_VAL(status, 1);
	
	std::string out_str = "Rx Status: ";
	if (ready) out_str += "Ready to receive.\n";
	else 	   out_str += "Not ready to receive.\n";

	std::cout << out_str << std::endl;
	return ready;
}

uint16_t UHF_Transceiver::getRxBufferCount() {
	uint16_t cnt = i2c.read2(RX_BUFFER_CNT);

	std::string out_str = "Rx Queue Status: ";
	if (!cnt) out_str += "No bytes in the receive queue.\n"; 							// pp. 24
	else 	  out_str += std::to_string(cnt) + "bytes in the receive queue.\n";

	std::cout << out_str << std::endl;
	return cnt;
}

uint8_t	UHF_Transceiver::getRxData() {
	uint8_t data = i2c.read(RX_DATA);

	if (data == 0xFF) {
		data = 0x00;		// returns null character
		std::cout << "WARNING: No data available in the receive buffer.\n" << std::endl; // pp. 24
	}

	return data;
}

uint16_t UHF_Transceiver:: getTxFreeSlots() {
	return i2c.read2(TX_BUFFER_FREE_SLOTS);
}

uint16_t UHF_Transceiver::getRxCRCFailCnt() {
	return i2c.read2(RX_CRC_FAIL_CNTR);
}

uint16_t UHF_Transceiver::getRxPacketCnt() {
	return i2c.read2(RX_PACKET_CNTR);
}

uint8_t UHF_Transceiver::getDroppedPackets() {
	return i2c.read(RX_FULL_FAIL_CNTR);
}

uint16_t UHF_Transceiver::getTxBufferOverrunCnt() {
	return i2c.read2(TX_BUFFER_OVERRUN);
}

uint8_t UHF_Transceiver::getFreqLockInfo() {
	return i2c.read(FREQUENCY_LOCK);
}

bool UHF_Transceiver::getRxLock() {
	uint8_t status = getReadySignals();
	bool is_locked = BIT_VAL(status, 0);

	std::string out_str = "Rx Frequency Lock Status: ";
	if (is_locked) out_str += "Locked.\n";
	else           out_str += "Not locked.\n";

	std::cout << out_str << std::endl;
	return is_locked;
}

bool UHF_Transceiver::getTxLock() {
	uint8_t status = getReadySignals();
	bool is_locked = BIT_VAL(status, 1);

	std::string out_str = "Tx Frequency Lock Status: ";
	if (is_locked) out_str += "Locked.\n";
	else           out_str += "Not locked.\n";

	std::cout << out_str << std::endl;
	return is_locked;
}

uint8_t UHF_Transceiver::getDTMFInfo() {
	return i2c.read(DTMF);
}

uint8_t UHF_Transceiver::getLastDTMFTone() {
	uint8_t info = getDTMFInfo();
	info &= 0xF;
	return info;
}

uint8_t UHF_Transceiver::getDTMFToneCnt() {
	uint8_t info = getDTMFInfo();
	info = (info >> 4) & 0xF;
	return info;
}

float UHF_Transceiver::getRSSI() {
	uint16_t val = i2c.read2(RSSI);
	float rssi = val * 0.00073242187;	// val * (3/4096) [V]	(see pp. 26)
	return rssi;
}

int UHF_Transceiver::getSMPSTemp() {
	uint8_t temp = i2c.read(SMPS_TEMP);
	return (int)temp;
}

int UHF_Transceiver::getPATemp() {
	uint8_t temp = i2c.read(PA_TEMP);
	return (int)temp;
}

float UHF_Transceiver::getCurrent3V3() {
	uint16_t val = i2c.read2(CURRENT_3V3);
	float current = val * 0.000003;		// val * (3e-6) [A]   (see pp. 27) 
	return current;
}

float UHF_Transceiver::getVoltage3V3() {
	uint16_t val = i2c.read2(VOLTAGE_3V3);
	val &= 0x1FFF;
	float voltage = val * 0.004;		// val * (4e-3) [V]   (see pp. 27) 
	return voltage;
}

float UHF_Transceiver::getCurrent5V() {
	uint16_t val = i2c.read2(CURRENT_5V);
	float current = val * 0.000062;		// val * (62e-6) [A]   (see pp. 27) 
	return current;
}

float UHF_Transceiver::getVoltage5V() {
	uint16_t val = i2c.read2(VOLTAGE_5V);
	val &= 0x1FFF;
	float voltage = val * 0.004;		// val * (4e-3) [V]   (see pp. 27) 
	return voltage;
}

uint16_t UHF_Transceiver::getPAForwardPower() {
	uint16_t val = i2c.read2(PA_POWER_FORWARD) & 0xFFF;
	return val;
}

float UHF_Transceiver::getCoupledPAForwardPower() {
	uint16_t val = getPAForwardPower();
	float x = val * 0.00073242187;		// val * (3/4096) [V] (see pp. 28)
	float y = -68838*pow(x,6) + 228000*pow(x,5) - 308831*pow(x,4) + 218934*pow(x,3) - 85741*pow(x,2) + 17660*val - 1511.8;	// in dB, pp. 28
	return y;
}

float UHF_Transceiver::getActualPAForwardPower() {
	return getCoupledPAForwardPower() + 32.5;		// in dB  (see pp. 28)
}

uint16_t UHF_Transceiver::getPAReversePower() {
	uint16_t val = i2c.read2(PA_POWER_REVERSE) & 0xFFF;
	return val;
}

float UHF_Transceiver::getCoupledPAReversePower() {
	uint16_t val = getPAReversePower();
	float x = val * 0.00073242187;		// val * (3/4096) [V] (see pp. 28)
	float y = -68838*pow(x,6) + 228000*pow(x,5) - 308831*pow(x,4) + 218934*pow(x,3) - 85741*pow(x,2) + 17660*val - 1511.8;	// in dB, pp. 28
	return y;
}

float UHF_Transceiver::getActualPAReversePower() {
	return getCoupledPAReversePower() + 32.5;		// in dB  (see pp. 28)
}

float UHF_Transceiver::getPAReverseLoss() {
	return getActualPAReversePower() - getActualPAForwardPower();	// in dB  (see pp. 28)
}