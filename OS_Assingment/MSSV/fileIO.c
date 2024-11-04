// this file is responsible of reading data from a file and putting the data into a solution shared array
#include <stdio.h>

#include "fileIO.h"
#include "ThreadDataStruct.h"

int parseFile(const char* FileName, SharedData *shared_data, int ExitFlag)
{
    if(ExitFlag == 0)
    {
        FILE* InputFile = fopen(FileName, "r"); // open input file in READ mode

        if(InputFile != NULL) // if file is found
        {
            for(int x = 0; x < 9; x++) // look at file and add numbers into the Sol array of ints
            {
                for(int y = 0; y < 9; y++)
                {
                    fscanf(InputFile, "%d", &shared_data->Sol[x][y]);
                }
            }

            fclose(InputFile); // close file after Sol is filled
        }
        else // display error and exit if file is not found
        {
            printf("unable to find file exiting...\n");
            ExitFlag = 1;
        }
    }

    return ExitFlag;
}