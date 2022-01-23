#include <iostream>
#include <windows.h>
#include <string>
#include <iomanip>

using namespace std;

struct Params1
{
    HANDLE h;
    long double localSUM;
    long double *globalSUM;
    std::size_t begin;
    std::size_t end;
};

DWORD startTime = 0; // starting counting pi-number point
DWORD finishTime = 0; // ending counting pi-number point
DWORD allTime = -1; // milliseconds, which will take the pi counting

size_t pi_blocks_i;
size_t pi_blocks_n;

HANDLE synchIteration;
HANDLE synchSummary;

const size_t BLOCKSIZE = 10 * 930824; // iteration distribution for threads
const size_t N = 100000000; // N iterations (not sigs after comma)

DWORD WINAPI countingPI(LPVOID lpParam);
long double preparingPI(int threadNum);

int main (int argc, char **argv)
{
    int numberOfThreads[] = {1, 2, 4, 8, 12, 16}; // number of threads
    int arraySize = sizeof(numberOfThreads)/sizeof(numberOfThreads[0]); // counting an array size
    long double piNumber; // the %pi number

    for (int i = 0; i < arraySize; i++)
    {
        piNumber = preparingPI(numberOfThreads[i]);
        cout << "\nThreads number: " << numberOfThreads[i] << " Time: " << allTime << " ms" << setprecision(N) << " %pi: " << piNumber << "\n";
    }
    return 0;
}

DWORD WINAPI countingPI(LPVOID lpParam)
{
    Params1 *par = (Params1*)lpParam;
    int isuicide;
    do
    {
        //==========GetIterBlock==========BEGIN

        DWORD waitError;

        //=====critical SELECT block=====BEGIN

        waitError = WaitForSingleObject(synchIteration, INFINITE);
        if (waitError != WAIT_OBJECT_0 /*Или WAIT_TIMEOUT, если отвалились по таймеру*/)
        {
            cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Error: " << GetLastError() << endl;
        }

        if(pi_blocks_i < pi_blocks_n)
        {
            (*par).begin = pi_blocks_i * BLOCKSIZE;
            (*par).end = (pi_blocks_i+1) * BLOCKSIZE;
            if((*par).end > N - 1)
            {
                (*par).end = N - 1;
            }
            ++pi_blocks_i;
        }
        else
        {
            (*par).begin = 10;
            (*par).end = 1;
        }

        ReleaseMutex(synchIteration);
        //=====critical SELECT block=====END

        //==========GetIterBlock==========END

        if((*par).begin <= (*par).end)
        {
            //pi!!!!!!!!!!!!!!!!!!!!!!
            long double xi;
            (*par).localSUM = 0;
            for(size_t i = (*par).begin; i <= (*par).end; ++i)
            {
                xi = ((long double)i + 0.5);
                xi /= (long double)N;
                (*par).localSUM += (4 / (1 + xi*xi));
            }
            
            //=====critical SUM block=====BEGIN
            waitError = WaitForSingleObject(synchSummary, INFINITE);
            if (waitError != WAIT_OBJECT_0 /*Или WAIT_TIMEOUT, если отвалились по таймеру*/)
            {
                cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Error: " << GetLastError() << endl;
            }
            
            *((*par).globalSUM) += (*par).localSUM;

            ReleaseMutex(synchSummary);
            //=====critical SUM block=====END
        }
        else
        {
            isuicide = -1;
        }
    }
    while(isuicide != -1);
    
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
        lpParameters[i].h = threadsArray[i];
        lpParameters[i].localSUM = 0;
        lpParameters[i].globalSUM = &localPI;
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

    localPI = localPI/N;

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