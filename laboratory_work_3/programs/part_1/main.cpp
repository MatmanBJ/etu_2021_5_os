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

size_t BS;
size_t NN;

HANDLE hMutexSELECT;
HANDLE hMutexSUM;

size_t pi_blocks_i;
size_t pi_blocks_n;

const size_t BLOCKSIZE = 10 * 930824; // iteration distribution for threads
const size_t N = 100000000; // N iterations (not sigs after comma)

DWORD WINAPI piCalc1(LPVOID lpParam);
long double processPI1(const size_t N, const unsigned threadNum, const size_t blocksize, DWORD *milisec);

int main (int argc, char **argv)
{
    int numberOfThreads[] = {1, 2, 4, 8, 12, 16}; // number of threads
    int arraySize = sizeof(numberOfThreads)/sizeof(numberOfThreads[0]); // counting an array size
    DWORD milisec = -1; // milliseconds, which will take the pi counting
    long double calculatedpi; // the %pi number

    for (int i = 0; i < arraySize; i++)
    {
        calculatedpi = processPI1(N, numberOfThreads[i], BLOCKSIZE, &milisec);
        cout << "\nThreads number: " << numberOfThreads[i] << " Time: " << milisec << " ms" << setprecision(N) << " %pi: " << calculatedpi << "\n";
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

        waitError = WaitForSingleObject(hMutexSELECT, INFINITE);
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

        ReleaseMutex(hMutexSELECT);
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
            waitError = WaitForSingleObject(hMutexSUM, INFINITE);
            if (waitError != WAIT_OBJECT_0 /*Или WAIT_TIMEOUT, если отвалились по таймеру*/)
            {
                cout << "Problem with SELECT WaitForSingleObject (return = " << waitError << "). Error: " << GetLastError() << endl;
            }
            
            *((*par).globalSUM) += (*par).localSUM;

            ReleaseMutex(hMutexSUM);
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

long double processPI1(const size_t N, const unsigned threadNum, const size_t blocksize, DWORD *milisec)
{
    // initilaizing objects and variables

    NN = N;
    BS = blocksize;
    pi_blocks_i = 0;
    pi_blocks_n = NN % blocksize == 0?NN/blocksize:NN/blocksize + 1;
    long double localPI = 0.0;
    DWORD waitError;
    DWORD startTime;
    DWORD finishTime;

    // synchronizing object (mutex) creation

    hMutexSELECT = CreateMutex(NULL, FALSE, NULL); // synchronizing object for selected iterations
    if(hMutexSELECT == NULL)
    {
        cout << "Problem with creating hMutexSELECT. Error: " << GetLastError() << endl;
        return -1;
    }

    hMutexSUM = CreateMutex(NULL, FALSE, NULL); // synchronizing object for summary counting
    if(hMutexSUM == NULL)
    {
        cout << "Problem with creating hMutexSUM. Error: " << GetLastError() << endl;
        CloseHandle(hMutexSELECT);
        return -1;
    }

    // creating threads for counting pi-number (just creating and setting threads here)

    Params1 *params = new Params1[threadNum];
    HANDLE *ths = new HANDLE[threadNum];
    for (unsigned i = 0; i < threadNum; i++)
    {
        ths[i] = CreateThread(NULL, 0, piCalc1, &(params[i]), CREATE_SUSPENDED, NULL);
        params[i].h = ths[i];
        if (params[i].h == NULL)
        {
            cout << "Problem with creating thread. Error: " << GetLastError() << endl;
            for (unsigned j = 0; j < i; j++)
            {
                CloseHandle(params[j].h);
            }
            CloseHandle(hMutexSELECT);
            CloseHandle(hMutexSUM);
            delete params;
            delete ths;
            return -1;
        }
        params[i].localSUM = 0;
        params[i].globalSUM = &localPI;

        if (SetThreadPriority(params[i].h, THREAD_PRIORITY_HIGHEST) == 0)
        {
            cout << "Problem with changing thread priority. Error: " << GetLastError() << endl;
        }
    }

    // starting the timer

    startTime = GetTickCount();

    // starting threads for counting pi-number (just starting here)

    for (unsigned i = 0; i < threadNum; i++)
    {
        ResumeThread(ths[i]);
    }

    // waiting until all threads will be released

    waitError = WaitForMultipleObjects(threadNum, ths, true, INFINITE);

    localPI /= N;

    // ending the timer
    
    finishTime = GetTickCount();

    // error checking

    if (!(WAIT_OBJECT_0 >= waitError || waitError <= WAIT_OBJECT_0 + threadNum - 1)/*или WAIT_TIMEOUT, если отвал по таймеру*/)
    {
        cout << "Problem with WaitForMultipleObjects (return = " << waitError << "). Error: " << GetLastError() << endl;
    }

    // "cleaning": closing handles adn cleaning memory

    for (unsigned i = 0; i < threadNum; ++i)
    {
        CloseHandle(ths[i]);
    }
    CloseHandle(hMutexSELECT);
    CloseHandle(hMutexSUM);
    delete params;
    delete ths;

    // counting final time

    *milisec = finishTime - startTime;

    return localPI;
}