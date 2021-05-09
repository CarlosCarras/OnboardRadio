 /****************************************************************************
 * UHF_Transceiver.cpp
 * 
 * @hardware    : UHF Transceiver
 * @manual      : USM-01-00097 User Manual Rev. C
 * @about       : The class definition to interface with the UHF transceiver.
 * @author      : Carlos Carrasquillo
 * @contact     : c.carrasquillo@ufl.edu
 * @date        : August 20, 2020
 * @modified    : March 19, 2021
 *
 * Property of ADAMUS lab, University of Florida.
 ****************************************************************************/

#include "UHF_Transceiver.h"


UHF_Transceiver::UHF_Transceiver(bool debug, uint8_t bus) {
	this->debug = debug;
	i2c = I2C_Functions(bus, TRANSCEIVER_I2C_ADDR);
}

uint8_t UHF_Transceiver::getModemConfig() {
	uint8_t config = i2c.read(MODEM_CONFIG);
	config &= 0b11;

	std::string out_str = "Modulation Scheme: ";

	if      (config == MODEM_GMSK_DOWN) out_str += "9600 bps GMSK downlink, 1200 bps AFSK uplink.";
	else if (config == MODEM_GMSK_UP)   out_str += "1200 bps AFSK downlink, 9600 bps GMSK uplink.";
	else if (config == MODEM_GMSK_BOTH) out_str += "9600 bps GMSK downlink and uplink.";
	else {
		printe("There was an error in the reception of the data.");
		return 0;
	}

	printi(out_str);
	return config;
}

void UHF_Transceiver::setModemConfig(uint8_t config) {
	uint8_t command;

	std::string out_str = "Modulation scheme switched to: ";

	switch (config) {
		case MODEM_GMSK_DOWN:  
			out_str += "9600 bps GMSK downlink, 1200 bps AFSK uplink.";
			command = config;
			break;
		case MODEM_GMSK_UP: 
			out_str += "1200 bps AFSK downlink, 9600 bps GMSK uplink.";
			command = config; 
			break;
		case MODEM_GMSK_BOTH: 
			out_str += "9600 bps GMSK downlink and uplink.";
			command = config; 
			break;
		default: 
			printe("Modulation scheme was not appropriate.");
			return;
	}

	i2c.write(MODEM_CONFIG, command);
	printi(out_str);
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

void UHF_Transceiver::sendByte(uint8_t data) {
	while(!transmitReady());
	i2c.write(TX_DATA, data);
}

void UHF_Transceiver::sendNBytes(uint8_t* data, int n) {
	while(!transmitReady());
	i2c.writen(TX_DATA, data, n);
}

void UHF_Transceiver::sendString(const std::string &data, uint8_t n) {
	char* data_arr = new char[n+1];
	 for (int i = 0; i < n; i++) {
	 	data_arr[i] = data.at(i);
	 }
	uint8_t* data_out = (uint8_t*)data_arr;
	sendNBytes(data_out, n);

	delete[] data_arr;
}

uint8_t UHF_Transceiver::getBeaconCtrl() {
	uint8_t status = i2c.read(BEACON_CTRL);
	std::string out_str = "Beacon Status: ";

	if (BIT_VAL(status, 0))  out_str += "Beacon is currently enabled.";
					    else out_str += "Beacon is currently disabled.";

	printi(out_str);
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

void UHF_Transceiver::setBeaconOutput(std::string str) {
	int str_len = str.length();
	if (str_len > BEACON_DATA_BUFFER_LEN) {
		str = str.substr(0, BEACON_DATA_BUFFER_LEN);
	}

	for (int i = 0; i < str_len; i++) {
		setBeaconData(str.at(i));
	}
}

uint8_t UHF_Transceiver::getPAPower() {
	uint8_t power = i2c.read(PA_POWER_LVL);
	power &= 0b11;

	std::string out_str = "Power Amplifier Power Level: ";

	if      (power == PA_LVL_27) out_str+= "27 dBm (0.5 W).";
	else if (power == PA_LVL_30) out_str += "30 dBm (1 W).";
	else if (power == PA_LVL_33) out_str += "33 dBm (2 W).";
	else 	  			 	out_str += "Inhibit (0 dBm/W).";

	printi(out_str);
	return power;
}

void UHF_Transceiver::setPAPower(uint8_t config) {
	uint8_t command;

	std::string out_str = "Power Amplifier power level switched to: ";

	switch (config) {
		case PA_LVL_27: 
			out_str += "27 dBm (0.5 W).";
			command = PA_LVL_27; 
			break;
		case PA_LVL_30: 
			out_str += "30 dBm (1 W).";
			command = PA_LVL_30; 
			break;
		case PA_LVL_33: 
			out_str += "33 dBm (2 W).";
			command = PA_LVL_33; 
			break;
		case PA_LVL_INHIBIT:
			out_str += "Inhibit (0 dBm/W).";
			command = PA_LVL_INHIBIT; 
		default: 
			printe("Modulation scheme was not appropriate.");
			return;
	}

	i2c.write(PA_POWER_LVL, command);
	printi(out_str);
}

uint16_t UHF_Transceiver::getRxFreqOffset() {
	uint16_t offset = i2c.read2(RX_OFFSET);
	offset &= 0x3FF;
	return offset;
}

void UHF_Transceiver::setRxFreqOffset(uint16_t offset) {
	printf("Frequency Offset: %d\n", offset);
	if (offset > 1023) {
		printe("Rx frequency offset is larger than 1023.");
	}
	i2c.write2(RX_OFFSET, offset);
}

void UHF_Transceiver::setRxFreq(float freq) {
	if (freq > 440 || freq < 430) {
		printe("The desired receiving frequency is outside of the bounds of 430 MHHz and 440 MHz.");
	}
	uint16_t offset = (uint16_t)((freq - 430) * 80);									// pp. 22
	setRxFreqOffset(offset);
}

float UHF_Transceiver::getRxFreq() {
	uint16_t offset = getRxFreqOffset();
	float freq = (offset * 0.0125) + 430;												// pp. 22
	return freq;
}

uint16_t UHF_Transceiver::getTxFreqOffset() {
	uint16_t offset = i2c.read2(TX_OFFSET);
	offset &= 0x1FF;
	return offset;
}

void UHF_Transceiver::setTxFreqOffset(uint16_t offset) {
	if (offset > 511) {
		printe("Tx frequency offset is larger than 511.");
	}
	i2c.write2(TX_OFFSET, offset);
}

void UHF_Transceiver::setTxFreq(float freq) {
	if (freq > 440 || freq < 430) {
		printe("The desired transmission frequency is outside of the bounds of 430 MHHz and 440 MHz.");
	}
	uint16_t offset = (uint16_t)((freq - 430) * 40);									// pp. 22
	setTxFreqOffset(offset);
}

float UHF_Transceiver::getTxFreq() {
	uint16_t offset = getTxFreqOffset();
	float freq = (offset * 0.025) + 430;												// pp. 22
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
	uint8_t status = i2c.read(DEBUG_REG);
	status &= 0x07;

	std::string out_str = "Debug Status:\n";

	if (BIT_VAL(status, 2)) out_str += "\tThe lock status of the receive and transmit synthesizers is ENABLED.\n";
					   else out_str += "\tThe lock status of the receive and transmit synthesizers is DISABLED.\n";
	if (BIT_VAL(status, 1)) out_str += "\tLED 1 is ON.\n";
					   else out_str += "\tLED 1 is OFF.\n";
	if (BIT_VAL(status, 0)) out_str += "\tLED 0 is ON.";
					   else out_str += "\tLED 0 is OFF.";

	printi(out_str);
	return status;
}

void UHF_Transceiver::ledOn(int led) {
	if (led == 1 || led == 0) {
		uint8_t status = getDebug();
		BIT_SET(status, led);
		i2c.write(DEBUG_REG, status);
		return;
	}
	printe("Invalid LED.");
}

void UHF_Transceiver::ledOff(int led) {
	if (led == 1 || led == 0) {
		uint8_t status = getDebug();
		BIT_CLEAR(status, led);
		i2c.write(DEBUG_REG, status);
		return;
	}
	printe("Invalid LED.");
}

void UHF_Transceiver::ledToggle(int led) {
	if (led == 1 || led == 0) {
		uint8_t status = getDebug();
		status = BIT_TOGGLE(status, led);
		i2c.write(DEBUG_REG, status);
		return;
	}
	printe("Invalid LED.");
}

void UHF_Transceiver::reset() {
	i2c.write(RESET, 0x00);
}

uint8_t UHF_Transceiver::getMode() {
	uint8_t config = i2c.read(TRANSPARENT_MODE);
	config &= 0xF;

	std::string out_str = "Operation Mode: ";

	if      (config == AX25_MODE)               out_str += "AX.25.";
	else if (config == TRANS_MODE_CONV_ENABLE)  out_str += "Transparent: convolutional encoder enabled.";
	else if (config == TRANS_MODE_CONV_DISABLE) out_str += "Transparent: convolutional encoder disabled.";
	else					                    out_str += "ERROR: There was an error in the reception of the data.";

	printi(out_str);
	return config;
}

void UHF_Transceiver::setMode(uint8_t config) {
	std::string out_str = "Operation Mode set to: ";

	switch (config) {
		case AX25_MODE: 
			out_str += "AX.25.";
			break;
		case TRANS_MODE_CONV_ENABLE: 
			out_str += "Transparent: convolutional encoder enabled.";
			break;
		case TRANS_MODE_CONV_DISABLE: 
			out_str += "Transparent: convolutional encoder disabled.";
			break;
		default: 
			printe("Selected operation mode was not appropriate.");
			return;
	}

	i2c.write(TRANSPARENT_MODE, config);
	printi(out_str);
}

uint16_t UHF_Transceiver::getTxThreshold() {
	return i2c.read2(ALMOST_EMPTY_THRESHOLD);
}

void UHF_Transceiver::setTxThreshold(uint16_t threshold) {
	if (threshold > 8191) {
		printe("The desired transmit ready threshold must be less than 8191.");
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

	std::string out_str = "Software Version: " + std::to_string(major) + "." + std::to_string(minor);
	printi(out_str);

	return version;
}

uint8_t UHF_Transceiver::getReadySignals() {
	return i2c.read(READY_SIGNALS);
}

bool UHF_Transceiver::transmitReady() {
	uint8_t status = getReadySignals();
	bool ready = BIT_VAL(status, 0);

	std::string out_str = "Tx Status: ";
	if (ready) out_str += "Ready to transmit.";
	else 	   out_str += "Not ready to transmit.";

	printi(out_str);
	return ready;
}

bool UHF_Transceiver::receiveReady() {
	uint8_t status = getReadySignals();
	bool ready = BIT_VAL(status, 1);
	
	std::string out_str = "Rx Status: ";
	if (ready) out_str += "Ready to receive.";
	else 	   out_str += "Not ready to receive.";

	printi(out_str);
	return ready;
}

uint16_t UHF_Transceiver::getRxBufferCount() {
	uint16_t cnt = i2c.read2(RX_BUFFER_CNT);

	std::string out_str = "Rx Queue Status: ";
	if (!cnt) out_str += "No bytes in the receive queue."; 							// pp. 24
	else 	  out_str += std::to_string(cnt) + "bytes in the receive queue.";

	printi(out_str);
	return cnt;
}

uint8_t	UHF_Transceiver::readByte() {
	while(!receiveReady());
	uint8_t data = i2c.read(RX_DATA);

	if (data == 0xFF) {
		data = 0x00;		// returns null character
		printi("No data available in the receive buffer."); // pp. 24
	}

	return data;
}

uint8_t* UHF_Transceiver::readNBytes(int n, uint8_t* data) {
	/* requires {uint8_t data[n];} prior to call. the values are returned in the 'data' variable. */
	while(!receiveReady());	// loop until ready to receive
	i2c.readn(RX_DATA, n, data);
	return data;
}

std::string UHF_Transceiver::readString(int n, uint8_t* data) {
	/* requires {uint8_t data[n];} prior to call. the values are returned in the 'data' variable. */
	uint8_t* incoming_raw = readNBytes(n, data);
	std::string str = (char*)incoming_raw;
	return str;
}

/* NOT MEANT FOR USE */
std::string UHF_Transceiver::readUntilDelimiter(char delimiter) {
	std::string data;
	char incoming = (char)readByte();
	while (incoming != delimiter) {
		data += incoming;
		incoming = (char)readByte();
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

	std::string    out_str  = "Rx Frequency Lock Status: ";
	if (is_locked) out_str += "Locked.";
	else           out_str += "Not locked.";

	printi(out_str);
	return is_locked;
}

bool UHF_Transceiver::getTxLock() {
	uint8_t status = getReadySignals();
	bool is_locked = BIT_VAL(status, 1);

	std::string out_str = "Tx Frequency Lock Status: ";
	if (is_locked) out_str += "Locked.";
	else           out_str += "Not locked.";

	printi(out_str);
	return is_locked;
}

bool UHF_Transceiver::testLocks() {
	return getTxLock() && getRxLock();
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
	float rssi = val * 0.00073242187;								// val * (3/4096) [V]	(see pp. 26)
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
	float current = val * 0.000003;									// val * (3e-6) [A]   (see pp. 27)
	return current;
}

float UHF_Transceiver::getVoltage3V3() {
	uint16_t val = i2c.read2(VOLTAGE_3V3);
	val &= 0x1FFF;
	float voltage = val * 0.004;									// val * (4e-3) [V]   (see pp. 27)
	return voltage;
}

float UHF_Transceiver::getCurrent5V() {
	uint16_t val = i2c.read2(CURRENT_5V);
	float current = val * 0.000062;									// val * (62e-6) [A]   (see pp. 27)
	return current;
}

float UHF_Transceiver::getVoltage5V() {
	uint16_t val = i2c.read2(VOLTAGE_5V);
	val &= 0x1FFF;
	float voltage = val * 0.004;									// val * (4e-3) [V]   (see pp. 27)
	return voltage;
}

uint16_t UHF_Transceiver::getPAForwardPower() {
	uint16_t val = i2c.read2(PA_POWER_FORWARD) & 0xFFF;
	return val;
}

float UHF_Transceiver::getCoupledPAForwardPower() {
	uint16_t val = getPAForwardPower();
	float x = val * 0.00073242187;									// val * (3/4096) [V] (see pp. 28)
	float y = -68838*pow(x,6) + 228000*pow(x,5) - 308831*pow(x,4) + 218934*pow(x,3) - 85741*pow(x,2) + 17660*val - 1511.8;	// in dB, pp. 28
	return y;
}

float UHF_Transceiver::getActualPAForwardPower() {
	return getCoupledPAForwardPower() + 32.5;						// in dB  (see pp. 28)
}

uint16_t UHF_Transceiver::getPAReversePower() {
	uint16_t val = i2c.read2(PA_POWER_REVERSE) & 0xFFF;
	return val;
}

float UHF_Transceiver::getCoupledPAReversePower() {
	uint16_t val = getPAReversePower();
	float x = val * 0.00073242187;									// val * (3/4096) [V] (see pp. 28)
	float y = -68838*pow(x,6) + 228000*pow(x,5) - 308831*pow(x,4) + 218934*pow(x,3) - 85741*pow(x,2) + 17660*val - 1511.8;	// in dB, pp. 28
	return y;
}

float UHF_Transceiver::getActualPAReversePower() {
	return getCoupledPAReversePower() + 32.5;						// in dB  (see pp. 28)
}

float UHF_Transceiver::getPAReverseLoss() {
	return getActualPAReversePower() - getActualPAForwardPower();	// in dB  (see pp. 28)
}

/************************** Testing **************************/

 void UHF_Transceiver::sendStringTest(const std::string &data, uint8_t n) {
	 char* data_arr = new char[n+1];
	 for (int i = 0; i < n; i++) {
	 	data_arr[i] = data.at(i);
	 }
	 uint8_t* data_out = (uint8_t*)data_arr;

	 for (int i = 0; i < n; i++) {
		 i2c.print_uint8("", data_out[i]);
	 }
 }