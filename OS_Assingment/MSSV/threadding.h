// header file arguements

#ifndef THREADDING_H
#define THREADDING_H

// fucntion prototypes
void* buildThread(void* param);
void assignTask(void* param);


// thread task assingments
void thread1(void* param); // also used by thread 2 and 3
void thread4(void* param);

void rowCheck(void* param);


#endif