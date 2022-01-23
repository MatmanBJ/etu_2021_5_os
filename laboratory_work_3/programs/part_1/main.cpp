/*

Program:

Saint-Petersburg ETU OS laboratory work 3 part 1

Author:

Matvey Sobolev, 2021

Compiler:

g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/

#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <list>
#include <numeric>

using namespace std;

// ---------- VARIABLES INITIALIZATION ----------

DWORD startTime = 0; // starting counting pi-number point
DWORD finishTime = 0; // ending counting pi-number point
DWORD allTime = -1; // milliseconds, which will take the pi counting
size_t blocksIterator; // current block
size_t blocksNumber; // number of all blocks
HANDLE synchIteration; // synchronizing iteration mutex
HANDLE synchSummary; // synchronizing summary mutex
const size_t BLOCKSIZE = 10 * 930824; // iteration distribution for threads
const size_t N = 100000000; // N iterations (not signs after comma)

list<long double> list1; // list of the all parts to summary
long double summaryResult = 0.0; // final pi result for each number of threads

// ---------- FUNCTION DECLARATION ----------

DWORD WINAPI countingPI (LPVOID localInThreads);
void preparingPI (int localNumberOfThreads);

// ---------- MAIN ----------

int main (int argc, char **argv)
{
    int numberOfThreads[] = {1, 2, 4, 8, 12, 16}; // number of threads
    int arraySize = sizeof(numberOfThreads)/sizeof(numberOfThreads[0]); // counting an array size
    long double piNumber; // the %pi number
    cout << "\nMultiprocessing: Win32 API.\n";

    for (int i = 0; i < arraySize; i++)
    {
    	list1.clear();
    	summaryResult = 0.0;
        preparingPI(numberOfThreads[i]);
        cout << "\nThreads number: " << numberOfThreads[i] << " Time: " << allTime << " ms" << setprecision(N) << " %pi: " << summaryResult << "\n";
    }
    return 0;
}

// ---------- FUNCTION'S BODY ----------

DWORD WINAPI countingPI(LPVOID localInThreads)
{
    int i; // iterator
    int localIndicator = 1; // end indicator
    size_t startPoint = 2;
    size_t endPoint = 1;
    long double localResult = 0.0; // local result summary

    while (localIndicator != 0)
    {
        // SYNCHRONIZING ITERATIONS -- START

        DWORD waitError = WaitForSingleObject (synchIteration, INFINITE); // while isn't released, i can't quit

        if (waitError != WAIT_OBJECT_0)
        {
            cout << "Sorry, you have error w/ sunchIteration (" << waitError << "). Last error number: " << GetLastError() << "\n";
        }

        if (blocksIterator < blocksNumber)
        {
            startPoint = blocksIterator * BLOCKSIZE; // blocksize number start (iteration*number_of_items_in_block)
            endPoint = (blocksIterator + 1) * BLOCKSIZE - 1; // blocksize number end (iteration*number) // HERE CHANGED FORMULA
            if (endPoint > N - 1) // checking for out of range error
            {
                endPoint = N - 1;
            }
            blocksIterator = blocksIterator + 1; // increasing iteration number
        }
        else
        {
        	startPoint = 2;
			endPoint = 1;
        }

        ReleaseMutex (synchIteration);

        // SYNCHRONIZING ITERATIONS -- END

        if (startPoint <= endPoint)
        {
            localResult = 0.0;

            for (i = startPoint; i++ <= endPoint; ) // formula counting
            {
                localResult = localResult + (4 / (1 + (((long double)i + 0.5) / (long double)N)*(((long double)i + 0.5) / (long double)N)));
            }

            // SYNCRONIZING SUMMARY -- BEGIN

            waitError = WaitForSingleObject (synchSummary, INFINITE);

            if (waitError != WAIT_OBJECT_0)
            {
                cout << "Sorry, you have problem w/ synchSummary (" << waitError << "). Last error number: " << GetLastError() << endl;
            }

            list1.push_back(localResult); // adding the result to the list

            ReleaseMutex (synchSummary);

            // SYNCHRONIZING SUMMARY -- END
        }
        else
        {
            localIndicator = 0;
        }
    }
    
    return 0;
}

void preparingPI (int localNumberOfThreads)
{
	// 1 -- PREPARING AND INITIALIZING

    // initilaizing objects and variables

    blocksIterator = 0; // setting to null block iterator
    blocksNumber = N % BLOCKSIZE == 0 ? (N / BLOCKSIZE) : (N / BLOCKSIZE + 1); // if div is full or not
    int i = 0; // iterator
    HANDLE *threadsArray = new HANDLE[localNumberOfThreads];
    synchIteration = CreateMutex (NULL, FALSE, NULL); // synchronizing object for selected iterations
    synchSummary = CreateMutex (NULL, FALSE, NULL); // synchronizing object for summary counting

    // 2 -- CHECKING THREADS AND CREATING THREADS

    // creating threads for counting pi-number (just creating and setting threads here)

    for (i = 0; i < localNumberOfThreads; i++)
    {
        threadsArray[i] = CreateThread (NULL, 0, countingPI, NULL, CREATE_SUSPENDED, NULL);
    }

    // 3 -- COUNTING PI-NUMBER

    // starting the timer

    startTime = GetTickCount();

    // starting threads for counting pi-number (just starting here)

    for (unsigned i = 0; i < localNumberOfThreads; i++)
    {
        ResumeThread (threadsArray[i]);
    }

    // waiting until all threads will be released

    DWORD waitError = WaitForMultipleObjects(localNumberOfThreads, threadsArray, true, INFINITE);

    // making the final result

    summaryResult = std::accumulate(std::begin(list1), std::end(list1), 0.0);
    summaryResult = summaryResult / N;

    // ending the timer
    
    finishTime = GetTickCount();

    // counting final time

    allTime = finishTime - startTime;

    // 4 -- ENDING AND CLEANING

    // "cleaning": closing handles adn cleaning memory

    for (i = 0; i < localNumberOfThreads; ++i)
    {
        CloseHandle(threadsArray[i]);
    }
    CloseHandle(synchIteration);
    CloseHandle(synchSummary);
    delete threadsArray;
}