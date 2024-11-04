// header file contains shared data struct
#ifndef THREAD_DATA_STRUCT_H
#define THREAD_DATA_STRUCT_H

// shared data struct

typedef struct 
{
    int Sol[9][9];
    int Row[9];
    int Col[9];
    int Sub[3][3];
    int Counter;
    int InvalidRows[9];
    int InvalidCols[9];
    int InvalidSubsections[9];
    unsigned long SimpleTID[4]; // an array which puts threads into it and uses its index as a thread ID
    int Delay;

    // other varaiable
    int index;
} SharedData;

#endif