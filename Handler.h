/*
 * Handler.h
 * @author        : Carlos Carrasquillo
 * @contact       : c.carrasquillo@ufl.edu
 * @date created  : February 4, 2021
 * @date modified : February 4, 2021
 * @description   : retreives the relevant output data or the acknowledge signal in response to a telecommand
 * @hardware      : N/A
 *
 * Property of ADAMUS lab, University of Florida.
 */

#ifndef HANDLER_H
#define HANDLER_H

#include <string>
#include "Packager.h"

class Handler {
private:
    Packager* packager;
    int identify_response(std::string telecom);
    void sendFile(std::string filename);
    void acknowledge(void);

public:
    explicit Handler();
    void process(std::string telecom);

    /******************* Functions *******************/
    void debug_led_on(int led);
    void debug_led_off(int led);
    void debug_led_toggle(int led);
};


#endif //HANDLER_H
