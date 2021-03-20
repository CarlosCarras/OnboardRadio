/****************************************************************************
* Handler.cpp
*
* @about      : retreives the relevant output data or the acknowledge signal in response to a telecommand
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 4, 2021
* @modified   : March 19, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Handler.h"


Handler::Handler(UHF_Transceiver* transceiver) {
    this->transceiver = transceiver;
    packager = new Packager(transceiver);
}

int Handler::process(command_t* inbound_command) {
    int status = identify_response(inbound_command);
    if (status != 0) sendError();
    return status;
}

void Handler::createFile(file_t* inbound_file) {
    std::ofstream new_file(inbound_file.dest);
    new_file << inbound_file.data;
    new_file.close();
}

int Handler::processFile(command_t* inbound_command) {
    int status;
    int retry = 1;                                      // gives multipacket interpreter two tries to succeed before failing
    file_t inbound_file;

    inbound_file.telecommand = inbound_command->telecommand;
    inbound_file.len_dest = inbound_command->params.at(0);
    inbound_file.dest = inbound_command->params.substr(1, len_dest);
    inbound_file.sof = inbound_command->params.substr(len_dest+1, 1);
    if (inbound_file.sof != SOF) {
        std::cout << "Error: Start of File (SOF) character " << std::to_string(SOF) << " not found. Aborting." << std::endl;
        return -1;
    }
    inbound_file.data = inbound_command->params.substr(len_dest+2);

    // if the file is transmitted over multiple packets... 
    if (inbound_file.data.back() != EOF) {
        interpreter = new interpreter(transceiver);
        command_t next_command;

        // initializing timer
        double dt;
        std::clock_t start = std::clock();
        double timer_dur = 10;
        
        do {
            sleep(1);
            next_command = interpreter.getCommand();

            if (next_command.telecommand != inbound_file.telecommand) {
                if (!retry) {
                    std::cout << "Error: End of File (EOF) character " << std::to_string(EOF) << " not found. Aborting." << std::endl;
                    return -1;
                }
            } else {
                retry--;
            }

            inbound_file.data += next_command.params;

            dt = (std::clock() - start) / (double)CLOCKS_PER_SEC;
            if (dt > timer_dur) {
                std::cout << "Error: File generation timed out. Too much time elapsed. The file may be too large." << std::endl;
                return -1;
            }

        } while (inbound_file.data.back() != EOF);      // loops until EOF char is found or timer expires

        delete(interpreter);
    }

    inbound_file.data.pop_back();                       // erases EOF character

    status = createFile(&inbound_file);

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

int Handler::identify_response(command_t* inbound_command) {
    int status = 0;
    uint8_t telecom = inbound_command->telecommand;

    if      (telecom == TELECOM_UPLOAD_GUIDANCE) {
        status = processFile(inbound_command);
        acknowledge();
    }
    else if (telecom == TELECOM_GET_HEALTH)             sendFile("health.csv");
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
    else {
        std::cout << "ERROR: Unknown Telecommand." << std::endl;
        return -1;
    }

    return status;
}

Handler::~Handler() {
    delete(packager);
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