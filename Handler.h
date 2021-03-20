/****************************************************************************
* Handler.h
*
* @about      : retreives the relevant output data or the acknowledge signal in response to a telecommand
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 4, 2021
* @modified   : February 12, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#ifndef HANDLER_H
#define HANDLER_H

/************************** Includes **************************/
#include <string>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include "telecommands.h"
#include "UHF_Transceiver.h"
#include "Interpreter.h"
#include "Packager.h"


/************************** Handler ***************************/
class Handler {
private:
    UHF_Transceiver* transceiver;
    Packager* packager;

    int identify_response(command_t* inbound_command);
    void createFile(file_t* inbound_file);
    int processFile(command_t* inbound_command);
    void sendFile(std::string filename);
    void acknowledge(void);
    void sendError(void);

public:
    explicit Handler(UHF_Transceiver* transceiver);
    int process(command_t* inbound_command);
    ~Handler();

    /******************* Functions *******************/
    void debug_led_on(int led);
    void debug_led_off(int led);
    void debug_led_toggle(int led);
};


#endif //HANDLER_H
