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

#include <cstdio>
#include "UHF_Transceiver.h"
#include "Handler.h"
#include "Interpreter.h"


#define MODEM_CONFIG_VAL            MODEM_GMSK_BOTH
#define PA_POWER_VAL                PA_LVL_27
#define FREQ_VAL		            437.08		      // MHz
#define BEACON_INIT_TIMEOUT         5                 // 5 minutes
#define BEACON_RECURRING_TIMEOUT    120               // 120 seconds
#define CHECK_HEALTH_EVERY_N_SCANS  10


class Radio {
private:
    UHF_Transceiver* transceiver;
    Handler* handler;
    Interpreter* interpreter;

    uint8_t pa_pwr_lvl;
    uint8_t cnt_since_healthcheck;

    void config();
    void configBeacon();
    int resolveLock();

public:
    explicit Radio();
    void healthCheck();
    uint8_t getPowerLevel() const;
    void setPowerLevel(uint8_t pa_pwr_lvl);
    void enableRadio();
    void disableRadio();
    int scan();
    ~Radio();

    /* Beacon Functions */
    void enableBeacon();
    void disableBeacon();
    void updateBeacon(const std::string& str);
    void updateBeaconCSV(const std::string& filename);
    uint8_t getBeaconStatus();

    /* Test Functions */
    explicit Radio(int setting);
    int test_scan();
    void test_config(int setting);
    void toggle_led(int led);
    void sendString(std::string str);
};

#endif //RADIO_H
