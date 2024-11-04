// the purpose of this file is to display summirized and well structured data to the user after all the threading has been complete
#include <stdio.h>
#include <stdbool.h>

#include "ThreadDataStruct.h"
#include "finalOutput.h"

/*
Thread 1 validates: Subsection 1, 2, 3 and row 1, 2, 3
Thread 2 validates: Subsection 4, 5, 6 and row 4, 5, 6
Thread 3 validates: Subsection 7, 8, 9 and row 7, 8, 9
Thread 4 validates: Columns 1, 2, 3, 4, 5, 6, 7, 8, 9
*/



// summarize data
void summarizeData(void* param)
{
    SharedData* shared_data = (SharedData*)param;

    if(shared_data->Counter == 27) // if all parts valid
    {
        printf("\n\nThread ID-1: valid\n");
        printf("Thread ID-2: valid\n");
        printf("Thread ID-3: valid\n");
        printf("Thread ID-4: valid\n\n");
        printf("There are 27 valid sub-grids, and thus the solution is valid.\n\n");

    }
    else // if invalid parts have been found
    {
        validityFind(shared_data);
    }
}


// function checks invlid arrays to find exactly what is invalid and displays it in a simple, easy to understand way
void validityFind(void* param)
{
    SharedData* shared_data = (SharedData*)param;
    int LowerBoundary = 0, UpperBoundary = 3;
    bool ValidCheck = true; // set to false if any invalid subsections are found (per thread)


    printf("\n\n\n");
    for(int i = 0; i < 3; i++) // loop checks all subsections and rows for threads 1, 2 and 3
    {
        printf("Thread ID-%d: ", i + 1); // +1 is used to offset starting from 0

        for(int i = LowerBoundary; i < UpperBoundary; i++) // loop verifies subsections
        {
            if(shared_data->InvalidSubsections[i] == 1)
            {
                printf("Subgrid %d ", i + 1);
                ValidCheck = false;
            }
        }

        
        for(int i = LowerBoundary; i < UpperBoundary; i++) // loop verifies rows
        {
            if(shared_data->InvalidRows[i] == 1)
            {
                printf("Row %d ", i + 1);
                ValidCheck = false;
            }
        }

        
        
        if(ValidCheck == true)
        {
            printf("Valid\n");
        }
        else
        {
            printf("is/are invalid\n");
        }

        LowerBoundary = LowerBoundary + 3;
        UpperBoundary = UpperBoundary + 3;
        ValidCheck = true; // valid check is reset for the next thread to be checked
    }



    printf("Thread ID-4: ");
    for(int i = 0; i < 9; i++) // loop validates all columns for thread 4
    {
        if(shared_data->InvalidCols[i] == 1)
        {
            printf("Column %d ", i + 1);
            ValidCheck = false;
        }
    }

    if(ValidCheck == true)
    {
        printf("Valid\n");
    }
    else
    {
        printf("is/are invalid\n");
    }



    printf("\nThere are in total %d valid rows, columns, and sub-grids, and the solution is invalid.\n", shared_data->Counter);

}

