#include <iostream>
#include <string>
#include "Packager.h"



int main() {
    Packager transceiver;

    std::string message = "hello world!";
    transceiver.sendData(message);

    return 0;
}
