/*=============================================================================
 PROGRAMMER:         Yang Wang
 COURSE:             CSC460: Operating System
 MODIFIED BY:        Ali Bajwa (ab)       50% Contribution
						- Coded semephores
						- Analyzed syncronization problems
						- Solved syncronization problems using 3 semephores
						- Tested program repeatedly
						- Wrote final documentation
                     Noah Bumgardner (nb) 50% Contribution
						- Marked critical sections
						- Wrote pseudocode for binary semephore
						- Wrote partial documentation
						- Submitted hardcopy
 LAST MODIFIED DATE: 11/13/2015
 DESCRIPTION:        A producer thread receives the count of the products n
                     through a command-line argument and creates n products
                     (simulated with n non-negative integers generated
                     randomly) for the consumer thread to consume. The two
                     threads share a bounded buffer of size bufSize.
 NOTE:               Synchronization between the two threads' operations has
                     been inserted. Comments ending with 'Ab' denote lines
					 inserted by programmers ab and nb. Syncronization 
					 problems include:
					 1. Consumer attempting to consume before there is a
						product in the buffer. (Counting Semephore used)
					 2. Producer producing more products than the buffer
						can handle. (Counting Semephore used)
					 3. Producer and Consumer trying to add and consume
						products simultanously. (Binary semephore used)
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
// SRead -> Binary semephore to lock buffer read/writes - Ab
// SProduced -> Counting semephore to prevent consumer from consuming before something is produced - Ab
// SBuffer -> Counting semephore to prevent producer from producing if buffer is full - Ab
int SRead = 1, SProduced = 0, SBuffer = 5; // Ab

void waitBinary(int& S) {     // Wait function for Binary semephore - Ab
	while (S == 0);
	S = 0;
}

void signalBinary(int& S) {   // Signal function for Binary semephore - Ab
	S = 1;
}

void waitCounting(int& S) {   // Wait function for Counting semephore - Ab
	while (S <= 0);
	S--;
}

void signalCounting(int& S) { // Signal function for Counting semephore - Ab
	S++;
}

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
  waitCounting(SBuffer);		 // Lock Counting semephore SBuffer (prevent producing) - Ab
  waitBinary(SRead);			 // Lock Binary semephore SRead (prevent reading) - Ab
  // CRITICAL SECTION STARTS HERE - Ab
  buffer[in] = aProduct;         // place the product into the buffer  
  in = (in+1) % bufSize;                    
  showBuffer(Producer, buffer, bufSize, in, '#');   // '#' marks the last 
                                                    // in-product position
  // CRITICAL SECTION ENDS HERE - Ab
  signalCounting(SProduced);	 // Unlock Counting semephore SProduced (product exists) - Ab
  signalBinary(SRead);			 // Unlock Binary semephore SRead (finished writing) - Ab
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
 {
  waitCounting(SProduced);	     // Lock Counting semephore SProduced (wait for production) - Ab
  waitBinary(SRead);			 // Lock Binary semephore SRead (prevent writing) - Ab
  // CRITICAL SECTION STARTS HERE - Ab
  buffer[out] = -1;              // remove a product from the buffer
  out = (out + 1) % bufSize;
  showBuffer(Consumer, buffer, bufSize, out, '*');   // '*' marks the last
                                                     // out-product position
  // CRITICAL SECTION ENDS HERE - Ab
  signalCounting(SBuffer);	     // Unlock Counting semephore SBuffer (space available) - Ab
  signalBinary(SRead);			 // Unlock Binary semephore SRead (finished reading) - Ab
  sleep(rand()%4);               // simulate the time for consuming the product
 }

 pthread_exit(NULL);
}

