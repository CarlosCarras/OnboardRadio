/****************************************************************************
* Handler.cpp
*
* @about      : retreives the relevant output data or the acknowledge signal in response to a telecommand
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 4, 2021
* @modified   : May 1, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Handler.h"
#include "Actions.h"
#include "ManageHistory.h"


Handler::Handler(UHF_Transceiver* transceiver) {
    packager = new Packager(transceiver);
}

int Handler::process(command_t* inbound_command) {
    int status = identify_response(inbound_command);
    return status;
}

void Handler::sendFile(std::string filename) {
    packager->sendFile(filename);
}

void Handler::sendSignal(uint8_t signal) {
    std::string out_str;
    out_str += (char)signal;
    packager->sendString(out_str);
}

void Handler::acknowledge(void) {
    sendSignal(ACKNOWLEDGE);
}

void Handler::sendError(void) {
    sendSignal(ERROR);
}

void Handler::sendStatus(uint8_t status) {
    if (status < 0) sendError();
    else            acknowledge();
}

int Handler::identify_response(command_t* inbound_command) {
    int status = 0;
    uint8_t telecom = inbound_command->telecommand;
    std::string params = inbound_command->params;

    switch(telecom) {
        case TELECOM_UPLOAD_FILE:
            acknowledge();
            break;
        case TELECOM_UNDO_UPLOAD:
            status = undoUpload(params);
            sendStatus(status);
            break;
        case TELECOM_GET_HISTORY:
            cleanHistory();
            sendFile(HISTORY_FILENAME);
            break;
        case TELECOM_GET_HEALTH:
            sendFile("health.csv");
            break;
        case TELECOM_DEBUG_ON:
            debug_led_on(0);
            acknowledge();
            break;
        case TELECOM_DEBUG_OFF:
            debug_led_off(0);
            acknowledge();
            break;
        case TELECOM_DEBUG_TOGGLE:
            debug_led_toggle(0);
            acknowledge();
            break;
        case TELECOM_OVERRIDE_ANTENNA:
            acknowledge();
            break;
        case ERROR:
            sendError();
            break;
        case TELECOM_PACKET_LOSS:
            sendSignal(TELECOM_PACKET_LOSS);
            break;
        case TELECOM_PACKET_FORMAT_ERR:
            sendSignal(TELECOM_PACKET_FORMAT_ERR);
            break;
        case TELECOM_LAST_PACKET_RECEIVED:
            sendSignal(TELECOM_LAST_PACKET_RECEIVED);
            break;
        default:
            std::cout << "ERROR: Unknown Telecommand." << std::endl;
            sendError();
            return -1;
    }

    return status;
}

Handler::~Handler() {
    delete(packager);
}

/****************************** Test Functions *****************************/

void Handler::debug_led_on(int led) {
    packager->debug_on(led);
}

void Handler::debug_led_off(int led) {
    packager->debug_off(led);
}

void Handler::debug_led_toggle(int led) {
    packager->debug_toggle(led);
}