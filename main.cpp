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
	if (argc > 3) {
		std::cout << "Error: Too many command line arguements." << std::endl;
	} else if (argc == 2) {
		config = atoi(argv[1]);
	}

    UHF_Transceiver* transceiver;
    Radio radio(config);

    while(1) {
            radio.test_scan();
            sleep(2);
    }

    delete(transceiver);
    //std::string message = "hello world!";
    //transceiver.sendString(message);

    return 0;
}
