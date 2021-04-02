/****************************************************************************
* Radio.h
*
* @about      : node that controls the operation of the onboard radio
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : February 14, 2021
* @modified   : March 19, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#ifndef RADIO_H
#define RADIO_H

#include "UHF_Transceiver.h"
#include "Handler.h"
#include "Interpreter.h"

#define DEBUG 1

#define MODEM_CONFIG_VAL MODEM_GMSK_BOTH
#define PA_POWER_VAL     PA_LVL_33
#define FREQ_VAL		 437.08		// MHz

class Radio {
private:
    UHF_Transceiver* transceiver;
    Handler* handler;
    Interpreter* interpreter;

    void config();
    void resolveLock();

public:
    explicit Radio();
    void checkHealth();
    int scan();
    ~Radio();

    /* Test Functions */
    explicit Radio(int setting);
    int test_scan();
    void test_config(int setting);
};

#endif //RADIO_H
