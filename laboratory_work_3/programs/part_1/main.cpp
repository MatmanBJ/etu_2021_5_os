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

size_t BS;
size_t NN;

HANDLE synchIteration;
HANDLE synchSummary;

size_t pi_blocks_i;
size_t pi_blocks_n;

const size_t BLOCKSIZE = 10 * 930824; // iteration distribution for threads
const size_t N = 100000000; // N iterations (not sigs after comma)

DWORD WINAPI piCalc1(LPVOID lpParam);
long double processPI1(int N, int threadNum, int blocksize);

int main (int argc, char **argv)
{
    int numberOfThreads[] = {1, 2, 4, 8, 12, 16}; // number of threads
    int arraySize = sizeof(numberOfThreads)/sizeof(numberOfThreads[0]); // counting an array size
    long double piNumber; // the %pi number

    for (int i = 0; i < arraySize; i++)
    {
        piNumber = processPI1(N, numberOfThreads[i], BLOCKSIZE);
        cout << "\nThreads number: " << numberOfThreads[i] << " Time: " << allTime << " ms" << setprecision(N) << " %pi: " << piNumber << "\n";
    }
    return 0;
}

DWORD WINAPI piCalc1(LPVOID lpParam)
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
            (*par).begin = pi_blocks_i * BS;
            (*par).end = (pi_blocks_i+1) * BS;
            if((*par).end > NN-1)
            {
                (*par).end = NN - 1;
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
                xi /= (long double)NN;
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

long double processPI1(int N, int threadNum, int blocksize)
{
	// 1 -- PREPARING AND INITIALIZING

    // initilaizing objects and variables

    NN = N;
    BS = blocksize;
    pi_blocks_i = 0;
    pi_blocks_n = NN % blocksize == 0?NN/blocksize:NN/blocksize + 1;
    int i = 0;
    int j = 0;
    long double localPI = 0.0;
    DWORD waitError;
    Params1 *lpParameters = new Params1[threadNum];
    HANDLE *threadsArray = new HANDLE[threadNum];
    synchIteration = CreateMutex (NULL, FALSE, NULL); // synchronizing object for selected iterations
    synchSummary = CreateMutex (NULL, FALSE, NULL); // synchronizing object for summary counting

    // 2 -- CHECKING THREADS AND CREATING THREADS

    // synchronizing object (mutex) creation

    if (synchIteration == NULL) // checking for mutex correct creation
    {
        cout << "Mutex iteration synchronizing creation problem. Last error number is " << GetLastError() << ".\n";
        return -1;
    }
    if (synchSummary == NULL) // checking for mutex correct creation
    {
        cout << "Mutex summary synchronizing creation problem. Last error number is " << GetLastError() << ".\n";
        CloseHandle(synchIteration);
        return -1;
    }

    // creating threads for counting pi-number (just creating and setting threads here)

    for (i = 0; i < threadNum; i++)
    {
        threadsArray[i] = CreateThread (NULL, 0, piCalc1, &(lpParameters[i]), CREATE_SUSPENDED, NULL);
        lpParameters[i].h = threadsArray[i];
        if (lpParameters[i].h == NULL)
        {
            cout << "Tread creation problem. Last error number is " << GetLastError() << "\n";
            for (j = 0; j < i; j++)
            {
                CloseHandle (lpParameters[j].h);
            }
            CloseHandle (synchIteration);
            CloseHandle (synchSummary);
            delete lpParameters;
            delete threadsArray;
            return -1;
        }
        else
        {
	        lpParameters[i].localSUM = 0;
	        lpParameters[i].globalSUM = &localPI;
	        if (SetThreadPriority (lpParameters[i].h, THREAD_PRIORITY_HIGHEST) == 0)
	        {
	            cout << "Problem with changing thread priority. Error: " << GetLastError() << "\n";
	        }
        }
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

    // error checking

    if (!(WAIT_OBJECT_0 >= waitError || waitError <= WAIT_OBJECT_0 + threadNum - 1)/*или WAIT_TIMEOUT, если отвал по таймеру*/)
    {
        cout << "Problem with WaitForMultipleObjects (return = " << waitError << "). Error: " << GetLastError() << endl;
    }

    // "cleaning": closing handles adn cleaning memory

    for (unsigned i = 0; i < threadNum; ++i)
    {
        CloseHandle(threadsArray[i]);
    }
    CloseHandle(synchIteration);
    CloseHandle(synchSummary);
    delete lpParameters;
    delete threadsArray;

    return localPI;
}