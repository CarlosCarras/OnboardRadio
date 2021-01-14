#include <iostream>
#include <string>
#include "Transceiver.h"

int main() {
    Transceiver transceiver;

    std::string message = "hello world!";
    transceiver.sendData(message);

    return 0;
}
