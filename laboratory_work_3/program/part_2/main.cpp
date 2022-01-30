/*

Program:

Saint-Petersburg ETU OS laboratory work 3 part 2

Author:

Matvey Sobolev, 2021

Compiler:

g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

ATTENTION:

WHILE COMPILING DO "g++ -fopenmp -o .\a.exe main.cpp"

*/

#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <omp.h>

using namespace std;

// ---------- VARIABLES INITIALIZATION ----------

DWORD startTime = 0; // starting counting pi-number point
DWORD finishTime = 0; // ending counting pi-number point
DWORD allTime = -1; // milliseconds, which will take the pi counting
const size_t BLOCKSIZE = 10 * 930824; // iteration distribution for threads
const size_t N = 100000000; // N iterations (not signs after comma)

// ---------- FUNCTION DECLARATION ----------

long double countingPI (size_t localIterations, size_t localBlocksize, int localNumberOfThreads);

// ---------- MAIN ----------

int main (int argc, char **argv)
{
    int numberOfThreads[] = {1, 2, 4, 8, 12, 16}; // number of threads
    int arraySize = sizeof(numberOfThreads)/sizeof(numberOfThreads[0]); // counting an array size
    long double piNumber; // the %pi number
    cout << "\nMultiprocessing: Open MP.\n";

    for (int i = 0; i < arraySize; i++)
    {
    	startTime = 0;
    	finishTime = 0;
    	allTime = 0;
        piNumber = countingPI(N, BLOCKSIZE, numberOfThreads[i]);
        cout << "\nThreads number: " << numberOfThreads[i] << " Time: " << allTime << " ms" << setprecision(N) << " %pi: " << piNumber << "\n";
    }
    return 0;
}

// ---------- FUNCTION'S BODY ----------

long double countingPI (size_t localIterations, size_t localBlocksize, int localNumberOfThreads)
{
	// 1 -- COUNTING PI-NUMBER

	int i = 0; // iterator

    // starting the timer

    startTime = GetTickCount();

    long double summaryResult = 0.0;
    #pragma omp parallel shared(startTime, finishTime, allTime) reduction (+: summaryResult) num_threads(localNumberOfThreads)
    {
        #pragma omp for schedule(dynamic, localBlocksize) nowait
        for (i = 0; i < localIterations; i++)
        {
            summaryResult = summaryResult + (4 / (1 + (((long double)i + 0.5) / (long double)localIterations)*(((long double)i + 0.5) / (long double)localIterations)));
        }
    }

    // making the final result

    summaryResult = summaryResult / localIterations;

    // ending the timer

    finishTime = GetTickCount();

    // counting final time

    allTime = finishTime - startTime;

    return summaryResult;
}