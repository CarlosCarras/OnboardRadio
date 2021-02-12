#include <iostream>
#include <string>
#include <unistd.h>
#incldue "UHF_Transceiver.h"
#incldue "Packager.h"
#include "Handler.h"
#include "telecommands.h"


#define PROCESS 1


int main() {

    if (PROCESS == 0) {
        Handler radio;
        while(1) {
            radio.process(TELECOM_DEBUG_TOGGLE);
            sleep(2);
        }
    } else if (PROCESS == 1) {
        UHF_Transceiver transceiver;
        while(1) {
            transceiver.ledToggle(0);
            transceiver.ledToggle(1);
            sleep(2);
        }
    }

    //std::string message = "hello world!";
    //transceiver.sendString(message);

    return 0;
}
