/****************************************************************************
* Radio.cpp
*
* @about      : node that controls the operation of the onboard radio
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : February 14, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "telecommands.h"
#include "Radio.h"


Radio::Radio() {
    transceiver = new UHF_Transceiver();
    handler = new Handler(transceiver);
    interpreter = new Interpreter(transceiver);
}

int Radio::scan() {
    command_t incoming_command = interpreter->getCommand();
    int status = handler->process(&incoming_command);
    return status;
}

int Radio::test_scan() {
	struct command_t incoming_command = {TELECOM_DEBUG_TOGGLE, "\0"};
	int status = handler->process(&incoming_command);
    return status;
}

Radio::~Radio() {
	delete(transceiver);
	delete(handler);
	delete(interpreter);
}