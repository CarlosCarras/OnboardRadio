/****************************************************************************
* Handler.cpp
*
* @about      : retreives the relevant output data or the acknowledge signal in response to a telecommand
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 4, 2021
* @modified   : February 12, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Handler.h"


Handler::Handler(UHF_Transceiver* transceiver) {
    packager = new Packager(transceiver);
}

int Handler::process(command_t* incoming_command) {
    int status = identify_response(incoming_command);
    if (status != 0) {
        std::cout << "ERROR: Unknown Telecommand." << std::endl;
        sendError();
    }
    return status;
}

void Handler::sendFile(std::string filename) {
    packager->sendFile(filename);
}

void Handler::acknowledge(void) {
    packager->sendString(ACKNOWLEDGE);
}

void Handler::sendError(void) {
    packager->sendString(ERROR);
}

int Handler::identify_response(command_t* incoming_command) {
    std::string telecom = incoming_command->telecommand;

    if      (telecom == TELECOM_GET_HEALTH)             sendFile("health.csv");
    else if (telecom == TELECOM_DEBUG_ON) {
        debug_led_on(0);
    }
    else if (telecom == TELECOM_DEBUG_OFF) {
        debug_led_off(0);
    }
    else if (telecom == TELECOM_DEBUG_TOGGLE) {
        debug_led_toggle(0);
    }
    else if (telecom == TELECOM_GET_HEALTH)             sendFile("health.csv");
    else if (telecom == TELECOM_OVERRIDE_ANTENNA)       acknowledge();
    else return -1;

    return 0;
}

/******************* Functions *******************/
void Handler::debug_led_on(int led) {
    packager->debug_on(led);
}

void Handler::debug_led_off(int led) {
    packager->debug_off(led);
}

void Handler::debug_led_toggle(int led) {
    packager->debug_toggle(led);
}