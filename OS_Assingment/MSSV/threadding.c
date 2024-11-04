// this file contains all the functions for the multithreading in this program

#include <pthread.h>
#include <unistd.h> // contains sleep function
#include <stdio.h>
#include <stdbool.h>

#include "threadding.h"
#include "ThreadDataStruct.h"


extern pthread_mutex_t mutex; // tells compiler mutex has been initialized elsewhere
extern pthread_mutex_t mutex2;

//function assingns large complex thread IDs into simple array indexes
void* buildThread(void* param)
{

    SharedData* shared_data = (SharedData*)param; // accesses shared data struct

    pthread_t tid = pthread_self(); // each thread ID is dereferenced


    pthread_mutex_lock(&mutex); // mutex lock acquired
    

    if (shared_data->index < 4) 
    {
        shared_data->SimpleTID[shared_data->index] = tid;
        shared_data->index++;


        for(int i = 0; i < 4; i++)
        {
            printf("\nthread ID stored at index %d = %ld", i, shared_data->SimpleTID[i]);
        }
        printf("\n");


        assignTask(shared_data); // moving function call inside the lock prevents potential race conditions
        
    } 

    pthread_mutex_unlock(&mutex); // mutex lock is released

    

    pthread_exit(NULL);
}


// function assigns tasks to specific threads
void assignTask(void* param)
{
    SharedData* shared_data = (SharedData*)param; // accesses shared data struct


    // this block assignes a task to each thread based on its ID maintained by the SimpleTID array index 0=1 -> 3=4
    if(pthread_self() == shared_data->SimpleTID[0] || pthread_self() == shared_data->SimpleTID[1] || pthread_self() == shared_data->SimpleTID[2]) //pthread_equal chacks if current thread is equal to thread in array in index 0
    {
        printf("\nThread 1 2 or 3 entered task assingment section \n\n");
        thread1(shared_data);
    }
    else
    {
        printf("\nThread 4 entered task assingment section \n\n");
        thread4(shared_data);
    }
}




// validating the 3x3 subsections (ALSO USED BY THREADS 2 AND 3)
void thread1(void* param)
{
    SharedData* shared_data = (SharedData*)param;
    int check[9] = {0}; // array initiliazed with 0s
    int currentValue;
    int stage = 0; // stages subsection validation
    int index = 0;
    int topBias;
    int bottomBias;
    int invalidIndexHelper;
    bool IncrementCounter = false;

    // bias is used for looping through for loops this feature helps by removing a large amount of repeat code line count reduced from 672->443
    if(pthread_self() == shared_data->SimpleTID[0])
    {
        bottomBias = 0;
        topBias = 3;
        invalidIndexHelper = -1; // variable is used as index offset for invalid array
    }
    else if(pthread_self() == shared_data->SimpleTID[1])
    {
        bottomBias = 3;
        topBias = 6;
        invalidIndexHelper = 2;
    }
    else
    {
        bottomBias = 6;
        topBias = 9;
        invalidIndexHelper = 5;
    }

    while(stage < 3) // 3 stages for each thread subsections
    {

        // checking validity of 3x3 section
        printf("Observing 3x3 grid\n");
        for(int x = bottomBias; x < topBias; x++)
        {
            for(int y = 0 + (3 * stage); y < 3 + (3 * stage); y++) // 3x3 window shifted to the right by 3 for every stage
            {
                currentValue = shared_data->Sol[x][y]; // currentValue uses Sol array index (will add to check array)
                printf("%d ", shared_data->Sol[x][y]);

                if(currentValue >= 1 && currentValue <= 9) // statement checks if value is in valid range and puts value in check array at num - 1 if the same number exists its spot remains 0
                {
                    check[currentValue - 1] = currentValue;
                }
            }
            printf("\n");
        }
        printf("\n");

        stage++; // stage incremented



        /* critical section synchronization
        Execution path of critical section
        mutex lock aquired values from check are added to
        Sub (if number != 0) i = 1
        otherwise its 0
        sub is gone over and numbers from it are added to counter
        if Sub contains any 0s access Invalid Subsections and make index of subsection be 1 to indicate invalid subsection
        reset sub back and check arrays back to 0
        release mutex lock*/
        pthread_mutex_lock(&mutex2); // mutex2 used to prevent deadlocks
        
        index = 0;

        for(int i = 0; i < 3; i++) // check array data is added to subsection array
        {
            for(int j = 0; j < 3; j++)
            {
                shared_data->Sub[i][j] = check[index++];
                
                if(shared_data->Sub[i][j] != 0) // if not invalid data becomes 1
                {
                    shared_data->Sub[i][j] = 1;
                }
                else // if Sub has 0s it is an invalid subsection
                {
                    shared_data->InvalidSubsections[stage + invalidIndexHelper] = 1;
                }
            }
        }

        IncrementCounter = true; // set to true otherwise set to false later in the check
        for(int i = 0; i < 3; i++) // this block checks the contents of Sub grid, if any 0s are found the elegibility of the incrementing the shared counter is revoked for that 3x3 grid
        {
            for(int j = 0; j < 3; j++)
            {
                if(shared_data->Sub[i][j] != 1)
                {
                    IncrementCounter = false;
                }
            }
        }

        if(IncrementCounter == true) // counter is incremented if every value in Sub array is 1
        {
            shared_data->Counter++;
        }



        // reset SubSection array back to 0
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                shared_data->Sub[i][j] = 0;
            }
        }

        //check array reinitilized to 0
        for(int i = 0; i < 9; i++)
        {
            check[i] = 0;
        }
        pthread_mutex_unlock(&mutex2);


        sleep(shared_data->Delay); // required sleep delay to look at data
    }

    rowCheck(shared_data);

}





// vertical column validation
void thread4(void* param)
{
    SharedData* shared_data = (SharedData*)param;
    int check[9] = {0}; // array initiliazed with 0s
    int currentValue;
    int stage = 0; // stages column validation
    int index = 0;
    bool IncrementCounter = false;


    while(stage < 9) // 9 stages for each column
    {

        // checking validity of 1x9 column
        printf("Observing 1x9 grid\n");
        for(int x = 0; x < 9; x++) // 3x3 window shifted to the right by 3 for every stage
        {
            currentValue = shared_data->Sol[x][stage]; // currentValue uses Sol array index (will add to check array)
            printf("%d ", shared_data->Sol[x][stage]);

            if(currentValue >= 1 && currentValue <= 9) // statement checks if value is in valid range and puts value in check array at num - 1 if the same number exists its spot remains 0
            {
                check[currentValue - 1] = currentValue;
            }
            printf("\n");
        }
        printf("\n\n");

        stage++; // stage incremented



        // critical section synchronization
        pthread_mutex_lock(&mutex2); // mutex2 used to prevent deadlocks
        
        index = 0;

        for(int i = 0; i < 9; i++) // check array data is added to Col array
        {
            shared_data->Col[i] = check[index++];
                
            if(shared_data->Col[i] != 0) // if not invalid data becomes 1
            {
                shared_data->Col[i] = 1;
            }
            else // if Sub has 0s it is an invalid subsection
            {
                shared_data->InvalidCols[stage - 1] = 1; // decremented by 1 to prevent off by 1 error
            }
        }
        

        IncrementCounter = true; // set to true otherwise set to false later in the check
        for(int i = 0; i < 9; i++) // checks shared Col array to see if the row is eligible to increment the shared counter
        {
            if(shared_data->Col[i] != 1)
            {
                IncrementCounter = false;
            }
        }

        if(IncrementCounter == true) // counter is incremented if every value in Sub array is 1
        {
            shared_data->Counter++;
        }



        // reset Col array back to 0
        for(int i = 0; i < 9; i++)
        {
            shared_data->Col[i] = 0;
        }

        //check array reinitilized to 0
        for(int i = 0; i < 9; i++)
        {
            check[i] = 0;
        }
        pthread_mutex_unlock(&mutex2);


        sleep(shared_data->Delay); // required sleep delay to look at data
    }

    printf("Thread ID-4: true thread id ---> %ld is the last thread\n", shared_data->SimpleTID[3]);
}





// horizontal row validation
void rowCheck(void* param)
{
    SharedData* shared_data = (SharedData*)param;
    int check[9] = {0}; // array initiliazed with 0s
    int currentValue;
    int stage = 0; // stages column validation
    int index = 0;
    bool IncrementCounter = false;
    int bias = 0; // bias varaible used to reduce repeat code

    // bias added to validity check
    if(pthread_self() == shared_data->SimpleTID[0]) //pthread_equal chacks if current thread is equal to thread in array in index 0
    {
        bias = 0;
    }
    else if(pthread_self() == shared_data->SimpleTID[1])
    {
        bias = 3;
    }
    else
    {
        bias = 6;
    }


    while(stage < 3) // 3 stages for each thread row
    {

        // checking validity of 9x1 section
        printf("Observing 9x1 grid\n");
        for(int y = 0; y < 9; y++)
        {
                currentValue = shared_data->Sol[stage + bias][y]; // currentValue uses Sol array index (will add to check array)
                printf("%d ", shared_data->Sol[stage + bias][y]);

                if(currentValue >= 1 && currentValue <= 9) // statement checks if value is in valid range and puts value in check array at num - 1 if the same number exists its spot remains 0
                {
                    check[currentValue - 1] = currentValue;
                }
        }
        printf("\n\n");

        stage++; // stage incremented



        // critical section synchronization
        pthread_mutex_lock(&mutex2); // mutex2 used to prevent deadlocks
        
        index = 0;

        for(int i = 0; i < 9; i++) // check array data is added to Row array
        {
            shared_data->Row[i] = check[index++];
                
                if(shared_data->Row[i] != 0) // if not invalid data becomes 1
                {
                    shared_data->Row[i] = 1;
                }
                else // if Sub has 0s it is an invalid subsection
                {
                    shared_data->InvalidRows[stage - 1 + bias] = 1; // decremented to top off by 1 errors + bias for proper positioning
                }
        }

        IncrementCounter = true; // set to true otherwise set to false later in the check
        for(int i = 0; i < 9; i++) // checks if the specific row is eligible to increment the shared counter
        {
                if(shared_data->Row[i] != 1)
                {
                    IncrementCounter = false;
                }
        }
        
        if(IncrementCounter == true) // counter is incremented if every value in Sub array is 1
        {
            shared_data->Counter++;
        }



        // reset SubSection array back to 0
        for(int i = 0; i < 9; i++)
        {
                shared_data->Row[i] = 0;
        }

        //check array reinitilized to 0
        for(int i = 0; i < 9; i++)
        {
            check[i] = 0;
        }
        pthread_mutex_unlock(&mutex2);


        sleep(shared_data->Delay); // required sleep delay to look at data */
    }
}