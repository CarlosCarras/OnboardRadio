#include <iostream>
#include <string>
#include <unistd.h>
#include "UHF_Transceiver.h"
#include "Packager.h"
#include "Handler.h"
#include "telecommands.h"


#define PROCESS 0


int main() {

    if (PROCESS == 0) {
        Handler radio;
        while(1) {
            radio.process(TELECOM_DEBUG_TOGGLE);
            sleep(1);
        }
    } else if (PROCESS == 1) {
        UHF_Transceiver* transceiver;
		transceiver = new UHF_Transceiver();
        while(1) {
            transceiver->ledToggle(0);
			sleep(1);
        }
    }

    //std::string message = "hello world!";
    //transceiver.sendString(message);

    return 0;
}
