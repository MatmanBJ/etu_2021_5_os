#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>
#include <list>
#include <numeric>

using namespace std;

struct Params1
{
public:
    HANDLE handle;
    long double localSUM;
    long double *globalSUM;
    size_t begin;
    size_t end;
};

DWORD startTime = 0; // starting counting pi-number point
DWORD finishTime = 0; // ending counting pi-number point
DWORD allTime = -1; // milliseconds, which will take the pi counting
size_t pi_blocks_i;
size_t pi_blocks_n;
HANDLE synchIteration; // synchronizing iteration mutex
HANDLE synchSummary; // synchronizing summary mutex
const size_t BLOCKSIZE = 10 * 930824; // iteration distribution for threads
const size_t N = 100000000; // N iterations (not signs after comma)

list<long double> list1;
long double summaryResult = 0.0; // final pi result for each number of threads

DWORD WINAPI countingPI(LPVOID lpParam);
long double preparingPI(int threadNum);

int main (int argc, char **argv)
{
    int numberOfThreads[] = {1, 2, 4, 8, 12, 16}; // number of threads
    int arraySize = sizeof(numberOfThreads)/sizeof(numberOfThreads[0]); // counting an array size
    long double piNumber; // the %pi number

    for (int i = 0; i < arraySize; i++)
    {
    	list1.clear();
    	summaryResult = 0.0;
        piNumber = preparingPI(numberOfThreads[i]);
        cout << "\nThreads number: " << numberOfThreads[i] << " Time: " << allTime << " ms" << setprecision(N) << " %pi: " << piNumber << "\n";
        cout << summaryResult;
    }
    return 0;
}

DWORD WINAPI countingPI(LPVOID lpParam)
{
    Params1 *par = (Params1*)lpParam;
    int i; // iterator
    int isuicide = 1; // indicator
    long double localResult = 0.0; // local result summary

    while (isuicide != -1)
    {
        //==========GetIterBlock==========BEGIN

        DWORD waitError;

        //=====critical SELECT block=====BEGIN

        // SYNCHRONIZING ITERATIONS -- START

        waitError = WaitForSingleObject (synchIteration, INFINITE); // while isn't released, i can't quit

        if (waitError != WAIT_OBJECT_0)
        {
            cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Last error number: " << GetLastError() << "\n";
        }

        if (pi_blocks_i < pi_blocks_n)
        {
            (*par).begin = pi_blocks_i * BLOCKSIZE; // blocksize number start (iteration*number_of_items_in_block)
            (*par).end = (pi_blocks_i + 1) * BLOCKSIZE - 1; // blocksize number end (iteration*number) // HERE CHANGED FORMULA
            if ((*par).end > N - 1) // checking for out of range error
            {
                (*par).end = N - 1;
            }
            pi_blocks_i = pi_blocks_i + 1; // increasing iteration number
        }
        else
        {
            (*par).begin = 2;
            (*par).end = 1;
        }

        ReleaseMutex(synchIteration);

        // SYNCHRONIZING ITERATIONS -- END

        if ((*par).begin <= (*par).end)
        {
            localResult = 0.0;

            for (i = (*par).begin; i <= (*par).end; i++) // formula counting
            {
                localResult = localResult + (4 / (1 + (((long double)i + 0.5) / (long double)N)*(((long double)i + 0.5) / (long double)N)));
            }

            // SYNCRONIZING SUMMARY -- BEGIN

            waitError = WaitForSingleObject (synchSummary, INFINITE);

            if (waitError != WAIT_OBJECT_0)
            {
                cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Error: " << GetLastError() << endl;
            }

            list1.push_back(localResult); // adding the result to the list

            ReleaseMutex (synchSummary);

            // SYNCHRONIZING SUMMARY -- END
        }
        else
        {
            isuicide = -1;
        }
    }
    
    return 0;
}

long double preparingPI(int threadNum)
{
	// 1 -- PREPARING AND INITIALIZING

    // initilaizing objects and variables

    pi_blocks_i = 0;
    pi_blocks_n = N % BLOCKSIZE == 0?N/BLOCKSIZE:N/BLOCKSIZE + 1;
    int i = 0;
    long double localPI = 0.0;
    DWORD waitError;
    Params1 *lpParameters = new Params1[threadNum];
    HANDLE *threadsArray = new HANDLE[threadNum];
    synchIteration = CreateMutex (NULL, FALSE, NULL); // synchronizing object for selected iterations
    synchSummary = CreateMutex (NULL, FALSE, NULL); // synchronizing object for summary counting

    // 2 -- CHECKING THREADS AND CREATING THREADS

    // creating threads for counting pi-number (just creating and setting threads here)

    for (i = 0; i < threadNum; i++)
    {
        threadsArray[i] = CreateThread (NULL, 0, countingPI, &(lpParameters[i]), CREATE_SUSPENDED, NULL);
        lpParameters[i].handle = threadsArray[i];
    }

    // 3 -- COUNTING PI-NUMBER

    // starting the timer

    startTime = GetTickCount();

    // starting threads for counting pi-number (just starting here)

    for (unsigned i = 0; i < threadNum; i++)
    {
        ResumeThread (threadsArray[i]);
    }

    // waiting until all threads will be released

    waitError = WaitForMultipleObjects(threadNum, threadsArray, true, INFINITE);

    // making the final result

    summaryResult = std::accumulate(std::begin(list1), std::end(list1), 0.0);
    summaryResult = summaryResult / N;

    // ending the timer
    
    finishTime = GetTickCount();

    // counting final time

    allTime = finishTime - startTime;

    // 4 -- ENDING AND CLEANING

    // "cleaning": closing handles adn cleaning memory

    for (i = 0; i < threadNum; ++i)
    {
        CloseHandle(threadsArray[i]);
    }
    CloseHandle(synchIteration);
    CloseHandle(synchSummary);
    delete lpParameters;
    delete threadsArray;

    return localPI;
}