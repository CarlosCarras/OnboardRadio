#include <iostream>
#include <string>
#include <unistd.h>
#include "UHF_Transceiver.h"
#include "Packager.h"
#include "Handler.h"
#include "Interpreter.h"
#include "Radio.h"
#include "telecommands.h"


int main(int argc, char *argv[]) {
	int config = 0;
    UHF_Transceiver* transceiver;
    Radio radio(config);

    while(1) {
            // radio.sendString("This is a test!");
            radio.test_scan();
            sleep(1);
    }

    return 0;
}
