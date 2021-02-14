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

#include "UHF_Transceiver.h"
#include "Handler.h"
#incldue "Interpreter.h"
#include "Radio.h"


Radio::Radio() {
    UHF_Transceiver* transceiver = new UHF_Transceiver();
    handler = new Handler(transceiver);
    interpreter = new Interpreter(transceiver);
}

int Radio::scan() {
    command incoming_command = interpreter->scan();
    int status = handler->process(incoming_command);
    return status;
}