/*=============================================================================
 PROGRAMMER:         Yang Wang
 COURSE:             CSC460: Operating System
 MODIFIED BY:        Ali Bajwa (ab)       50% Contribution
                        - Analyzed synchronization problems
                        - Coded second draft: 3 semaphores
                        - Tested program
                        - Wrote partial documentation
                     Noah Bumgardner (nb) 50% Contribution
					    - Coded first draft: pseudocode
						- Coded third draft: create & destroy semaphores
                        - Corrected typos
                        - Marked critical sections
                        - Submitted hard copy
                        - Wrote partial documentation
 LAST MODIFIED DATE: 11/13/2015
 DESCRIPTION:        A producer thread receives the count of the products n
                     through a command-line argument and creates n products
                     (simulated with n non-negative integers generated
                     randomly) for the consumer thread to consume. The two
                     threads share a bounded buffer of size bufSize.
 NOTE:               Synchronization between the two threads' operations has
                     been inserted. Comments ending with 'Ab' denote lines
					 inserted by programmers ab and nb. Synchronization 
					 problems include:
					 1. Consumer attempting to consume before there is a
						product in the buffer. (Counting Semaphore used)
					 2. Producer producing more products than the buffer
						can handle. (Counting Semaphore used)
					 3. Producer and Consumer trying to add and consume
						products simultaneously. (Binary Semaphore used)
					 The critical sections of both the Producer and Consumer
					 are clearly indicated in the code using comments.
 FILES:              ProducerConsumer.cpp, showBuf.cpp, pc.h, makefile
 COMPILER:           GNU g++
 INSTRUCTION FOR COMPILATION AND EXECUTION:
     1. type: make                                      to COMPILE 
     2. type: ./pc  productCount                        to EXECUTE
==============================================================================*/
#include "pc.h"

// global data shared by both producer and consumer threads
int const bufSize = 5;
int       buffer[bufSize];         // space to store the products
// DECLARE 3 SEMAPHORES - Ab
// SemRead -> Binary semaphore to lock buffer read/writes - Ab
// SemProduced -> Counting semaphore to prevent consumer from consuming before something is produced - Ab
// SemBuffer -> Counting semaphore to prevent producer from producing if buffer is full - Ab
sem_t SemBuffer;
sem_t SemProduced;
sem_t SemRead;
// DECLARATIONS END HERE - Ab

//*****************************************************************************
void* consumerThread(void* arg);

//*****************************************************************************
int main(int argCount, char* argList[]) 
{pthread_t  threadId;
 int        productCount,     // the number of the products to be produced
            aProduct,         // a product is an integer: 0~999
            in = 0;           // index for the next generated product

//--------------------------------------
// 1. Validate the command-line arguments
//--------------------------------------
 if (argCount != 2)
 {cout <<"\n************************************************\n"
       <<"The correct useage should be: " <<argList[0] <<" productCount"
       <<"\n************************************************\n\n";
  exit(0);
 }
 productCount = atoi(argList[1]);
 if (productCount < 1)
 {cout <<"\nSince the productCount " <<argList[1]
                                     <<" < 1, no product will be produced.\n\n";
  exit(0);
 }

//--------------------------------------
// 2. Other preparations
//--------------------------------------
 for (int j=0; j<bufSize; j++)   // A "-1" placed in the buffer is used as only
   buffer[j] = -1;               // a visual clue for an "empty" slot of the
                                 // buffer in the output, and it should not be
                                 // used for any condition test in the program
 showBuffer(Init, buffer, bufSize, 0, ' ');

 srand(time(NULL));              // for simulating time laps

// INITIALIZE SEMAPHORES USED FOR SYNCRONIZATION - Ab
 if (sem_init(&SemBuffer, 0, bufSize) < 0)
 {
  cout <<"\nError: cannot create semaphore 'SemBuffer'.\n";
  exit(0);
 }
 if (sem_init(&SemProduced, 0, 0) < 0)
 {
  cout <<"\nError: cannot create semaphore 'SemProduced'.\n";
  // Destroy 1 initialized semaphore
  if (sem_destroy(&SemBuffer) < 0)
   cout <<"\nError: cannot destroy semaphore 'SemBuffer'.\n";
  exit(0);
 }
 if (sem_init(&SemRead, 0, 1) < 0)
 {
  cout <<"\nError: cannot create semaphore 'SemRead'.\n";
  // Destroy 2 initialized semaphores
  if (sem_destroy(&SemBuffer) < 0)
   cout <<"\nError: cannot destroy semaphore 'SemBuffer'.\n";
  if (sem_destroy(&SemRead) < 0)
   cout <<"\nError: cannot destroy semaphore 'SemRead'.\n";
  exit(0);
 }
// INITIALIZATIONS END HERE - Ab

//--------------------------------------
// 3. Create the consumer thread
//--------------------------------------
 if (pthread_create(& threadId, NULL, consumerThread,  & productCount) != 0)
 {cout <<"\nError: cannot create the consumer thread.\n";
 
  // SEMAPHORE DESTRUCTION STARTS HERE - Ab
  // Destroy all semaphores before exiting - Ab
  if (sem_destroy(&SemBuffer) < 0)
   cout <<"\nError: cannot destroy semaphore 'SemBuffer'.\n";
  if (sem_destroy(&SemProduced) < 0)
   cout <<"\nError: cannot destroy semaphore 'SemProduced'.\n";
  if (sem_destroy(&SemRead) < 0)
   cout <<"\nError: cannot destroy semaphore 'SemRead'.\n";
  // SEMAPHORE DESTRUCTION ENDS HERE - Ab
  exit(0);
 }

//--------------------------------------
// 4. Producer produces productCount items
//--------------------------------------
 for (int i=0; i<productCount; i++)
 {sleep(rand()%4);               // simulate the time for producing a product
  aProduct = rand()%1000;        // products are integers: 0~999
  sem_wait(&SemBuffer);		 // Lock Counting semaphore SemBuffer (prevent producing) - Ab
  sem_wait(&SemRead);			 // Lock Binary semaphore SemRead (prevent reading) - Ab
  // CRITICAL SECTION STARTS HERE - Ab
  buffer[in] = aProduct;         // place the product into the buffer  
  in = (in+1) % bufSize;                    
  showBuffer(Producer, buffer, bufSize, in, '#');   // '#' marks the last 
                                                    // in-product position
  // CRITICAL SECTION ENDS HERE - Ab
  sem_post(&SemProduced);	 // Unlock Counting semaphore SemProduced (product exists) - Ab
  sem_post(&SemRead);			 // Unlock Binary semaphore SemRead (finished writing) - Ab
 }

//--------------------------------------
// 5. Wait for the consumer thread to finish
//--------------------------------------
 if (pthread_join(threadId, NULL) != 0)
  cout <<"Failed to wait for the consumer thread.\n";

//--------------------------------------
// 6. Wrap-up
//--------------------------------------
 cout <<"\nDone.\n";
// SEMAPHORE DESTRUCTION STARTS HERE - Ab
// Destroy all semaphores before exiting - Ab
 if (sem_destroy(&SemBuffer) < 0)
  cout <<"\nError: cannot destroy semaphore 'SemBuffer'.\n";
 if (sem_destroy(&SemProduced) < 0)
  cout <<"\nError: cannot destroy semaphore 'SemProduced'.\n";
 if (sem_destroy(&SemRead) < 0)
  cout <<"\nError: cannot destroy semaphore 'SemRead'.\n";
// SEMAPHORE DESTRUCTION ENDS HERE - Ab
 return 0;
}

//*****************************************************************************
void* consumerThread(void* arg)
{int productCount = * (int*) arg, // retrieve the count of the products
     out = 0;                     // index for the next to-be-consumed product

 for (int i=0; i<productCount; i++)
 {
  sem_wait(&SemProduced);	     // Lock Counting semaphore SemProduced (wait for production) - Ab
  sem_wait(&SemRead);			 // Lock Binary semaphore SemRead (prevent writing) - Ab
  // CRITICAL SECTION STARTS HERE - Ab
  buffer[out] = -1;              // remove a product from the buffer
  out = (out + 1) % bufSize;
  showBuffer(Consumer, buffer, bufSize, out, '*');   // '*' marks the last
                                                     // out-product position
  // CRITICAL SECTION ENDS HERE - Ab
  sem_post(&SemBuffer);	     // Unlock Counting semaphore SemBuffer (space available) - Ab
  sem_post(&SemRead);			 // Unlock Binary semaphore SemRead (finished reading) - Ab
  sleep(rand()%4);               // simulate the time for consuming the product
 }

 pthread_exit(NULL);
}

