//this file manages the command line argurement validation

#include <stdio.h>
#include "cmdlValidation.h"
#include "ThreadDataStruct.h"

int argurements(int ExitFlag, SharedData *shared_data)
{

    if(shared_data->Delay < 1 || shared_data->Delay > 10) // checks if time delay is within the valid range
    {
        printf("Invalid Time range (1-10) exiting...\n");
        ExitFlag = 1;
    }



    return ExitFlag;
}