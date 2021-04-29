/****************************************************************************
* ManageHistory.cpp
*
* @about      : keeps track of the last telecommands executed
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : April 29, 2021
* @modified   : April 29, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "ManageHistory.h"
#include <time.h>
#include "telecommands.h"


void addToHistory(command_t* command) {
    time_t my_time = time(NULL);

    std::ofstream history(HISTORY_FILENAME, ios:app);
    history << "Telecommand: " << std::hex << static_cast<int>(command->telecommand) << ", Params: " << command->params << ", Time: " << ctime(&my_time) << \n";
}

void cleanHistory() {
    int num_lines = 0;
    int line_cntr = 0;
    std::string line;

    /* counting the number of lines */
    std::ifstream history;
    history.open(HISTORY_FILENAME);
    while (std::getline(history, line)) ++num_lines;
    history.close();

    /* deleting all but the last LEAVE_LAST_N files */
    std::ifstream history;
    history.open(HISTORY_FILENAME);
    std::ofstream tempFile;
    tempFile.open("temp.txt", ofstream::out);

    while(std::getline(history,line)) {
        if (num_lines - ++line_cntr < LEAVE_LAST_N) {
            tempFile << line;
        }
    }

    history.close();
    tempFile.close();

    remove(HISTORY_FILENAME);
    rename("temp.txt", HISTORY_FILENAME);
}