/****************************************************************************
* ManageHistory.h
*
* @about      : keeps track of the last telecommands executed
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : April 29, 2021
* @modified   : April 29, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#ifndef MANAGEHISTORY_H
#define MANAGEHISTORY_H

#include "telecommands.h"

#define HISTORY_FILENAME "history.d3"
#define LEAVE_LAST_N     10

void addToHistory(command_t* command);
void cleanHistory();

#endif // MANAGEHISTORY_H
