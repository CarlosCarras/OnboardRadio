/****************************************************************************
* Radio.h
*
* @about      : node that controls the operation of the onboard radio
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : February 14, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#ifndef RADIO_H
#define RADIO_H

#include "UHF_Transceiver.h"
#include "Handler.h"
#include "Interpreter.h"

class Radio {
private:
    UHF_Transceiver* transceiver;
    Handler* handler;
    Interpreter* interpreter;

public:
    explicit Radio();
    int scan();
    int test_scan(); // for testing ONLY
    ~Radio();
};

#endif //RADIO_H
