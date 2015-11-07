/*=============================================================================
 PROGRAMMER:         Yang Wang
 COURSE:             CSC460: Operating System
 MODIFIED BY:        Ali Bajwa (ab)       __% Contribution
                     Noah Bumgardner (nb) __% Contribution
 LAST MODIFIED DATE: 11/6/2015
 DESCRIPTION:        A producer thread receives the count of the products n
                     through a command-line argument and creates n products
                     (simulated with n non-negative integers generated
                     randomly) for the consumer thread to consume. The two
                     threads share a bounded buffer of size bufSize.
 NOTE:               Synchronization between the two threads' operations has
                     been inserted as commented pseudo-code. Changes are needed.
                     Comments beginning with 'B.Sync' denote lines inserted by
                     programmers ab and nb.
					 Problem: We do not need two semaphores, just one binary
                     semaphore. hw5 instructions require at least one of each
                     kind of semaphore.
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
// DECLARE 2 SEMAPHORES // B.Sync

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

//--------------------------------------
// 3. Create the consumer thread
//--------------------------------------
 if (pthread_create(& threadId, NULL, consumerThread,  & productCount) != 0)
 {cout <<"\nError: cannot create the consumer thread.\n";
  exit(0);
 }

//--------------------------------------
// 4. Producer produces productCount items
//--------------------------------------
 for (int i=0; i<productCount; i++)
 {sleep(rand()%4);               // simulate the time for producing a product
  aProduct = rand()%1000;        // products are integers: 0~999
  // Lock binary semaphore // B.Sync
  buffer[in] = aProduct;         // place the product into the buffer  
  in = (in+1) % bufSize;                    
  showBuffer(Producer, buffer, bufSize, in, '#');   // '#' marks the last 
                                                    // in-product position
  // Unlock binary semaphore // B.Sync
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
 return 0;
}

//*****************************************************************************
void* consumerThread(void* arg)
{int productCount = * (int*) arg, // retrieve the count of the products
     out = 0;                     // index for the next to-be-consumed product

 for (int i=0; i<productCount; i++)
 {// Lock binary semaphore // B.Sync
  buffer[out] = -1;              // remove a product from the buffer
  out = (out + 1) % bufSize;
  showBuffer(Consumer, buffer, bufSize, out, '*');   // '*' marks the last
                                                     // out-product position
  // Unlock binary semaphore // B.Sync
  sleep(rand()%4);               // simulate the time for consuming the product
 }

 pthread_exit(NULL);
}

