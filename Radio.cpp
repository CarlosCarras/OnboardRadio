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

#include "telecommands.h"
#include "Radio.h"


Radio::Radio() {
    transceiver = new UHF_Transceiver();
    handler = new Handler(transceiver);
    interpreter = new Interpreter(transceiver);

    config();
}

void Radio::config() {
    transceiver.setModemConfig(MODEM_CONFIG_VAL);
    transceiver.setPAPower(PA_POWER_VAL);
    transceiver.setRxFreq(FREQ_VAL);
    transceiver.setTxFreq(FREQ_VAL);

    resolveLock();
}

void Radio::resolveLock() {
    while (transceiver.testLocks());
}

void Radio::checkHealth() {
    if (transceiver.getModemConfig() != MODEM_CONFIG_VAL) {
        transceiver.setModemConfig(MODEM_CONFIG_VAL);
    }
    if (transceiver.getPAPower() != PA_POWER_VAL) {
        transceiver.setPAPower(PA_POWER_VAL);
    }
    if (transceiver.getTxFreq() != FREQ_VAL) {
        transceiver.setTxFreq(FREQ_VAL);
    }
    if (transceiver.getRxFreq() != FREQ_VAL) {
        transceiver.setRxFreq(FREQ_VAL);
    }


    resolveLock();
}

int Radio::scan() {
    checkHealth();
    command_t incoming_command = interpreter->getCommand();
    int status = handler->process(&incoming_command);
    return status;
}

Radio::~Radio() {
    delete(transceiver);
    delete(handler);
    delete(interpreter);
}


/********************** Test Functions **********************/

Radio::Radio(int setting) {
    transceiver = new UHF_Transceiver();
    handler = new Handler(transceiver);
    interpreter = new Interpreter(transceiver);

    test_config(setting);
}

void Radio::test_config(int setting) {
    config();

    switch(setting) {
        case 0: 
            transceiver.setPAPower(PA_LVL_27);
        case 1: 
            transceiver.setPAPower(PA_LVL_30);
        case 2:
            transceiver.setPAPower(PA_LVL_33);
    }
}

int Radio::test_scan() {
    checkHealth();
	struct command_t incoming_command = {TELECOM_DEBUG_TOGGLE, "\0"};
	int status = handler->process(&incoming_command);
    return status;
}