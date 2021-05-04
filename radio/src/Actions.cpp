/****************************************************************************
* Actions.cpp
*
* @about      : contains all of the functions that can be called by a telecommand
* @author     : Carlos Carrasquillo
* @contact    : c.carrasquillo@ufl.edu
* @date       : April 29, 2021
* @modified   : May 1, 2021
*
* Property of ADAMUS lab, University of Florida.
****************************************************************************/

#include "Actions.h"
#include <fstream>
#include <iostream>
#include "telecommands.h"

int undoUpload(const std::string& filename) {
    std::string backupname = filename + BACKUP_EXT;

    std::ifstream file;
    file.open(backupname.c_str());
    if (file.is_open()) {
        remove(filename.c_str());
        rename(backupname.c_str(), filename.c_str());
        return 0;
    }

    return -1;
}
