// header file arguements

#ifndef FILEIO_H
#define FILEIO_H

#include "ThreadDataStruct.h"

//#include "ThreadDataStruct.h" // otherwise unknown datatype error

// function prototypes
int parseFile(const char* FileName, SharedData *shared_data, int ExitFlag);

#endif