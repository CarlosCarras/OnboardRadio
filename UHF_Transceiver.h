/****************************************************************************
* UHF_Transceiver.hs
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

#ifndef UHF_TRANSCEIVER
#define UHF_TRANSCEIVER


/************************** Includes **************************/
#include <string>
#include <string.h>
#include <math.h>
#include "I2C_Functions.h"


/*************************** Defines ***************************/
#define TRANSCEIVER_I2C_ADDR  	 	 0x25

// Registers (pp. 19 of 31)                DESCRIPTION
#define MODEM_CONFIG			0x00	// select uplink and downlink modulation scheme
#define AX25_TX_DELAY			0x01	// AX.25 transmission delay
#define SYNC_BYTES				0x02	// configure the sync byte value
#define TX_DATA 				0x03	// data to be transmitted
#define BEACON_CTRL				0x04 	// beacon control
#define BEACON_DATA				0x05	// write data for the custom beacon portion
#define PA_POWER_LVL			0x06	// power amplifier level
#define RX_OFFSET				0x07	// select an alternative receive frequency
#define TX_OFFSET				0x09	// select an alternative transmit frequency
#define INITIAL_I2C_TIMEOUT		0x0B	// beacon’s initial I2C timeout (1–7 min)
#define RECURRING_I2C_TIMEOUT	0x0C	// beacon’s recurring I2C timeout (10–127 sec)
#define DEBUG_REG				0x0D	// LED control for debugging
#define RESET 					0x0E	// resets all registers
#define TRANSPARENT_MODE		0x10 	// select between AX.25 and convolutional encoder
#define ALMOST_EMPTY_THRESHOLD 	0x11	// define transmit ready threshold
#define PTT_OFF_DELAY_AFSK		0x13	// time following a transmit sequence before PA is turned off (default = 10ms)
#define PTT_OFF_DELAY_GMSK		0x14	// time following a transmit sequence before PA is turned off (default = 1ms)
#define FIRMWARE_VERSION		0x19	// firmware version number (bits 7:4 - major, 3:0 - minor)
#define READY_SIGNALS			0x1A	// transmit ready (TR) and receive ready (RR) signals
#define RX_BUFFER_CNT			0x1B	// number of bytes to be read from the receive buffer
#define RX_DATA					0x1D	// data receivied
#define TX_BUFFER_FREE_SLOTS	0x1E	// number of free slots in the transmit buffer
#define RX_CRC_FAIL_CNTR		0x21    // number of AX.25 packets dropped as a result of AX.25 frame check sequence
#define RX_PACKET_CNTR			0x23	// number of successfully received packets	
#define RX_FULL_FAIL_CNTR		0x25 	// packets dropped as a result of insufficient space in receive buffer
#define TX_BUFFER_OVERRUN   	0x26	// counts times when byte is written to transmit buffer when no free slots available
#define FREQUENCY_LOCK			0x28	// frequency lock detect signals for both transmitter (TXL) and receiver (RXL)
#define DTMF					0x29 	// DTMF tone (3:0) and tone received  counter (7:4)
#define RSSI					0x2A 	// received signal strength indicator (RSSI)
#define SMPS_TEMP				0x2C 	// switched-mode power supply temperature reading of most recent conversion
#define PA_TEMP 				0x2D 	// power amplifier temperature reading of most recent conversion
#define CURRENT_3V3 			0x2E 	// most recent reading from the 3.3 V current sensor
#define VOLTAGE_3V3             0x30  	// most recent reading of bus voltage on 5 V supply
#define CURRENT_5V  			0x32 	// most recent reading from the 5 V current sensor
#define VOLTAGE_5V           	0x34  	// most recent reading of bus voltage on 3.3 V supply
#define PA_POWER_FORWARD 		0x36  	// value used to compute actual forward power
#define PA_POWER_REVERSE 		0x38 	// value used to compute actual reverse power

/* 13.4.1 Register 0x00: Modem configuration register */
#define MODEM_GMSK_DOWN			0b01
#define MODEM_GMSK_UP			0b10
#define MODEM_GMSK_BOTH			0b11

/* 13.4.7 Register 0x06: PA power level register */
#define PA_LVL_27				0b00
#define PA_LVL_30				0b01
#define PA_LVL_33				0b10
#define PA_LVL_INHIBIT			0b11


/********************* UHF Transceiver  **********************/
class UHF_Transceiver {

private:
	I2C_Functions i2c;

	void beaconEnable(bool enable);							// enables or disables the beacon functionality
	uint16_t getRxFreqOffset();								// gets data in the receiving frequency offset register
	void setRxFreqOffset(uint16_t offset);					// sets the data in the receiving frequency offset register
	uint16_t getTxFreqOffset();								// gets data in the transmission frequency offset register
	void setTxFreqOffset(uint16_t offset);					// sets the data in the transmission frequency offset register
	uint8_t getReadySignals();								// gets the transmit ready and receive ready signals
	uint8_t getFreqLockInfo();								// gets the frequency lock detect signals for both the transmitter and receiver
	uint8_t getDTMFInfo();									// gets relevant Dual-Tone Multi-Frequency (DTMF) information
	uint16_t getPAForwardPower();							// gets raw, unconverted PA forward power reading
	uint16_t getPAReversePower();							// gets raw, unconverted PA reverse power reading	

public: 
    explicit UHF_Transceiver(uint8_t bus = 2);
	uint8_t getModemConfig();								// reports modulation scheme
	void setModemConfig(uint8_t config);					// sets modulation scheme
	void setTransmissionDelay(uint8_t delay);				// sets AX.25 transmission delay (1-255)
	uint8_t getTransmissionDelay();							// gets AX.25 transmission delay (1-255)
	void setSyncBytes(uint8_t val);							// configure the sync byte value
	uint8_t getSyncBytes();									// read the sync byte value
	void sendByte(uint8_t data);							// transmits a byte of data
	void sendNBytes(uint8_t* data, int n);					// transmits 'n' bytes of data
	void sendString(const std::string &data, uint8_t n);	// transmits a string of data
	uint8_t getBeaconCtrl();								// reads the beacon control register
	void clearBeaconData();									// clears the beacon data
	void enableBeacon();									// enables the beacon functionality
	void disableBeacon();									// disables the beacon functionality
	void setBeaconData(uint8_t data);						// sets the beacon data
	uint8_t getPAPower();									// gets the Power Amplifier power level
	void setPAPower(uint8_t config);						// sets the Power Amplifier power level
	void setRxFreq(float freq);								// set receiving requency
	float getRxFreq();										// get receiving requency
	void setTxFreq(float freq);								// set transmission requency
	float getTxFreq();									    // get transmission requency
	void setInitialTimeout(uint8_t timeout);				// set beacon’s initial I2C timeout
	uint8_t getInitialTimeout();							// get beacon’s initial I2C timeout
	void setRecurringTimeout(uint8_t timeout);				// set beacon’s recurring I2C timeout
	uint8_t getRecurringTimeout();							// get beacon’s recurring I2C timeout
	void reset();											// resets all registers to default values
	uint8_t getMode();										// gets the transceiver's operation mode
	void setMode(uint8_t config);							// sets the transceiver's operation mode
	uint16_t getTxThreshold();								// gets the transmit ready (TR) threshold
	void setTxThreshold(uint16_t threshold);				// sets the transmit ready (TR) threshold
	uint8_t getPAOffDelayGMSK();							// gets the time after a transmit until the PA is turned off (GMSK)
	void setPAOffDelayGMSK(uint8_t delay);					// gets the time after a transmit until the PA is turned off (GMSK)
	uint8_t getPAOffDelayAFSK();							// gets the time after a transmit until the PA is turned off (AFSK)
	void setPAOffDelayAFSK(uint8_t delay);					// gets the time after a transmit until the PA is turned off (AFSK)
	uint8_t getFirmware();									// gets and prints the firmware version
	bool transmitReady();									// determines whether data can be transmitted
	bool receiveReady();									// determines whether data can be received
	uint16_t getRxBufferCount();							// determines number of bytes to be read from the receive buffer
	uint8_t readByte();										// fetches the data from the received data buffer
	uint8_t* readNBytes(int n, uint8_t* data);				// fetches 'n' bytes from the received data buffer
	std::string readString(int n, uint8_t* data);			// fetches 'n' bytes from the received data buffer, returns string
	std::string readUntilDelimiter(char delimiter);			// NOT MEANT FOR USE: reads data until a selected delimiter (e.g. EOF) is read
	uint16_t getTxFreeSlots();								// determines number of free slots in the transmit buffer
	uint16_t getRxCRCFailCnt();								// determines total number of AX.25 packets dropped due to AX.25 frame check sequence (FCS) failing
	uint16_t getRxPacketCnt();								// determines total number of times an AX.25 packet is successfully received
	uint8_t getDroppedPackets(); 							// number of AX.25 packets dropped due to insufficient space in receive buffer
	uint16_t getTxBufferOverrunCnt();						// number of bytes written to transmit buffer when there are no free slots available.
	bool getRxLock();										// determines whether receiving frequency lock has been achieved
	bool getTxLock();										// determines whether transmission frequency lock has been achieved
	bool testLocks();										// determines whether both the rx and tx frequency locks have been achieved
	uint8_t getLastDTMFTone();								// get the last Dual-Tone Multi-Frequency (DTMF) [see table 6, pp. 18]
	uint8_t getDTMFToneCnt();								// get the count of DTMF tones received and decoded
	float getRSSI();										// get the received signal strength indicator (RSSI) in Volts, pp. 26
	int getSMPSTemp();										// gets the temperature data from the switched-mode power supply (SMPS) in Celcius, pp. 26
	int getPATemp();										// gets the temperature data from the power amplifier (PA) in Celcius, pp. 27
	float getCurrent3V3();									// gets the most recent reading from the 3.3 V current sensor
	float getVoltage3V3();									// gets the most recent reading of the bus voltage on the 3.3 V supply
	float getCurrent5V();									// gets the most recent reading from the 5 V current sensor
	float getVoltage5V();									// gets the most recent reading of the bus voltage on the 5 V supply
	float getCoupledPAForwardPower();						// coupled forward power reading in dB
	float getActualPAForwardPower();						// actual forward power reading in dB
	float getCoupledPAReversePower();						// coupled reverse power reading in dB
	float getActualPAReversePower();						// actual reverse reading in dB
	float getPAReverseLoss();								// power amplifier reverse loss in dB

	/**** Debug Functions ****/ 
	uint8_t getDebug();
	void ledOn(int led);
	void ledOff(int led);
	void ledToggle(int led);
	void sendStringTest(const std::string &data, uint8_t n);
};

/**** Test Functions ****/
void transmitByteTest(uint8_t data);

#endif // UHF_TRANSCEIVER