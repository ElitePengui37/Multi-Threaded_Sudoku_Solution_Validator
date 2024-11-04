#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Header file declerations
#include "cmdlValidation.h"
#include "fileIO.h"
#include "threadding.h"
#include "ThreadDataStruct.h"
#include "finalOutput.h"

#define NUM_THREADS 4 // number of child threads that will be created

pthread_mutex_t mutex; // mutex lock defined
pthread_mutex_t mutex2; // second mutex lock to prevent deadlocks

int main(int argc, char* argv[])
{
    // Theading variables
    pthread_t tid[NUM_THREADS]; // array of thread IDs
    pthread_attr_t attr;
    pthread_mutex_init(&mutex, NULL);


    SharedData shared_data; // struct that contains shared data
    int ExitFlag = 0;

    shared_data.index = 0; // index is initialized at 0




    // Command line validation
    if(argc != 3)
    {
        printf("Invalid Command Line Arguements please enter\n./mssv SolutionFile Delay(1-10)\n\n");
        ExitFlag = 1;
    }
    else
    {
        shared_data.Delay = atoi(argv[2]); // convert delay into an int so that the value passed in isnt 0

        ExitFlag = argurements(ExitFlag, &shared_data);
    }

    
    // Input File parsing
    ExitFlag = parseFile(argv[1], &shared_data, ExitFlag);


    if(ExitFlag == 0)
    {
        pthread_attr_init(&attr); // default thread attributes

        for(int i = 0; i < NUM_THREADS; i++) // creates 4 child threads
        {
            pthread_create(&tid[i], &attr, buildThread, (void*)&shared_data);
        }

        for(int i = 0; i < NUM_THREADS; i++)
        {
            pthread_join(tid[i], NULL);
        }


        printf("\n\n\nPARTITION CHECK");
        for(int i = 0; i < 4; i++)
        {
            printf("\ntid (Thread ID) array thread at location %d = %ld\n", i, shared_data.SimpleTID[i]);
        }

        summarizeData(&shared_data);
    }
    
    return 0;
}
