/****************************************************************************
* Radio.cpp
*
* @about      : node that controls the operation of the onboard radio
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : March 19, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Radio.h"
#include "telecommands.h"


Radio::Radio() {
    transceiver = new UHF_Transceiver();
    handler = new Handler(transceiver);
    interpreter = new Interpreter(transceiver);

    config();
    configBeacon();
}

void Radio::config() {
    pa_pwr_lvl = PA_POWER_VAL;
    cnt_since_healthcheck = 0;

    transceiver->setModemConfig(MODEM_CONFIG_VAL);
    transceiver->setPAPower(pa_pwr_lvl);
    transceiver->setRxFreq(FREQ_VAL);
    transceiver->setTxFreq(FREQ_VAL);
    transceiver->setMode(AX25_MODE);

    // resolveLock();
}

void Radio::configBeacon() {
    transceiver->setInitialTimeout(BEACON_INIT_TIMEOUT);
    transceiver->setRecurringTimeout(BEACON_RECURRING_TIMEOUT);
}

int Radio::resolveLock() {
    int cntr = 0;
    while (!transceiver->testLocks() && cntr < 10) {
        sleep(0.15);
        cntr++;
    }
    if (cntr >= 10) return -1;  // unable to establish frequency lock
    return 0;                   // frequency locked
}

void Radio::healthCheck() {
    if (transceiver->getModemConfig() != MODEM_CONFIG_VAL) {
        transceiver->setModemConfig(MODEM_CONFIG_VAL);
    }
    if (transceiver->getPAPower() != pa_pwr_lvl) {
        transceiver->setPAPower(pa_pwr_lvl);
    }
    if (transceiver->getTxFreq() != FREQ_VAL) {
        transceiver->setTxFreq(FREQ_VAL);
    }
    if (transceiver->getRxFreq() != FREQ_VAL) {
        transceiver->setRxFreq(FREQ_VAL);
    }
    if (transceiver->getInitialTimeout() != BEACON_INIT_TIMEOUT) {
        transceiver->setInitialTimeout(BEACON_INIT_TIMEOUT);
    }
    if (transceiver->getRecurringTimeout() != BEACON_RECURRING_TIMEOUT) {
        transceiver->setInitialTimeout(BEACON_RECURRING_TIMEOUT);
    }

    cnt_since_healthcheck = 0;
    // resolveLock();
}

uint8_t Radio::getPowerLevel() {
    return pa_pwr_lvl;
}

void Radio::setPowerLevel(uint8_t pa_pwr_lvl) {
    this->pa_pwr_lvl = pa_pwr_lvl;
}

void Radio::enableRadio() {
    pa_pwr_lvl = PA_POWER_VAL;
}

void Radio::disableRadio() {
    pa_pwr_lvl = PA_LVL_INHIBIT;
}

int Radio::scan() {
    if (cnt_since_healthcheck++ > CHECK_HEALTH_EVERY_N_SCANS) healthCheck();

    command_t incoming_command = interpreter->getCommand();
    int status = handler->process(&incoming_command);

    std::string beacon_msg = "TEST!";
    updateBeacon(beacon_msg);
    return status;
}

Radio::~Radio() {
    delete(transceiver);
    delete(handler);
    delete(interpreter);
}

/********************* Beacon Functions *********************/

void Radio::enableBeacon() {
    transceiver->enableBeacon();
}

void Radio::disableBeacon() {
    transceiver->disableBeacon();
}

void Radio::updateBeacon(const std::string& str) {
    transceiver->setBeaconOutput(str);
}

uint8_t Radio::getBeaconStatus() {
    return transceiver->getBeaconCtrl();
}

/********************** Test Functions **********************/

Radio::Radio(int setting) {
    transceiver = new UHF_Transceiver();
    handler = new Handler(transceiver);
    interpreter = new Interpreter(transceiver);

    config();
    test_config(setting);
    configBeacon();
}

void Radio::test_config(int setting) {
    switch(setting) {
        case 0: 
            transceiver->setPAPower(PA_LVL_27);
            break;
        case 1: 
            transceiver->setPAPower(PA_LVL_30);
            break;
        case 2:
            transceiver->setPAPower(PA_LVL_33);
            break;
    }
}

int Radio::test_scan() {
    if (cnt_since_healthcheck++ > CHECK_HEALTH_EVERY_N_SCANS) healthCheck();

	command_t incoming_command = interpreter->getCommandTest();
	int status = handler->process(&incoming_command);
    
    std::string beacon_msg = "TEST!";
    updateBeacon(beacon_msg);
    return status;
}

void Radio::toggle_led(int led) {
    transceiver->ledToggle(led);
}

void Radio::sendString(std::string str) {
    transceiver->sendString(str, str.length());
}