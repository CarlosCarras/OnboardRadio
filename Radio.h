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

class Radio {
private:
    Handler* handler;
    Interpreter* interpreter;

public:
    explicit Radio();
    int scan();
};

#endif //RADIO_H
