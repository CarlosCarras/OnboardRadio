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
#include "telecommands.h"
#include "UHF_Transceiver.h"
#include "Packager.h"


/************************** Handler ***************************/
class Handler {
private:
    Packager* packager;
    int identify_response(command incoming_command);
    void sendFile(std::string filename);
    void acknowledge(void);

public:
    explicit Handler(UHF_Transceiver* transceiver);
    void process(command incoming_command);

    /******************* Functions *******************/
    void debug_led_on(int led);
    void debug_led_off(int led);
    void debug_led_toggle(int led);
};


#endif //HANDLER_H
