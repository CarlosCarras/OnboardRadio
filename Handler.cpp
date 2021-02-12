/*
 * Handler.cpp
 * @author        : Carlos Carrasquillo
 * @contact       : c.carrasquillo@ufl.edu
 * @date created  : February 4, 2021
 * @date modified : February 4, 2021
 * @description   : retreives the relevant output data or the acknowledge signal in response to a telecommand
 * @hardware      : N/A
 *
 * Property of ADAMUS lab, University of Florida.
 */

#include "Handler.h"
#include "telecommands.h"

Handler::Handler(std::string telecom) {
    packager = new Packager();
}

void Handler::process(std::string telecom) {
    int status = identify_response(telecom);
    if (status != 0) {
        std::cout << "ERROR: Unknown Telecommand." << std::endl;
    }
}

void Handler::sendFile(std::string filename) {
    packager->sendFile(filename);
}

void Handler::acknowledge(void) {
    packager->sendString(ACKNOWLEDGE);
}

int Handler::identify_response(std::string telecom) {
    if      (telecom == TELECOM_GET_HEALTH)             sendFile("health.csv");
    else if (telecom == TELECOM_DEBUG_ON) {
        debug_led_on(0);
        debug_led_on(1);
    }
    else if (telecom == TELECOM_DEBUG_OFF) {
        debug_led_off(0);
        debug_led_off(1);
    }
    else if (telecom == TELECOM_GET_HEALTH)             sendFile("health.csv");
    else if (telecom == TELECOM_OVERRIDE_ANTENNA)       acknowledge();
    else return -1;

    return 0;
}

/******************* Functions *******************/
void debug_led_on(int led) {
    packager.transceiver->ledOn(led);
}

void debug_led_off(int led) {
    packager.transceiver->ledOff(led);
}

void debug_led_toggle(int led) {
    packager.transceiver->ledToggle(led);
}