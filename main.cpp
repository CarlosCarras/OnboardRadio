#include <iostream>
#include <string>
#include <unistd.h>
#include "UHF_Transceiver.h"
#include "Packager.h"
#include "Handler.h"
#include "Interpreter.h"
#include "Radio.h"
#include "telecommands.h"


#define PROCESS 2


int main() {
    UHF_Transceiver* transceiver;
    Radio radio;

    transceiver = new UHF_Transceiver();

    if (PROCESS == 0) {
        Handler handler(transceiver);
        struct command_t telecom = {TELECOM_DEBUG_TOGGLE, "\0"};

        while(1) {
            handler.process(&telecom);
            sleep(1);
        }

    } else if (PROCESS == 1) {
        while(1) {
            transceiver->ledToggle(0);
			sleep(1);
        }

    } else if (PROCESS == 2) {
        while(1) {
            radio.test_scan();
            sleep(2);
        }

    }

    delete(transceiver);
    //std::string message = "hello world!";
    //transceiver.sendString(message);

    return 0;
}
