/*

Program:

Saint-Petersburg ETU OS laboratory work 4 part 1

Author:

Matvey Sobolev, 2021

Compiler:

g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/

#include <windows.h>
#include <string>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <list>

using namespace std;

const size_t PAGE_NUMBER = 9 + 3 + 0 + 8 + 2 + 4 - 9;
const size_t READ_TIMES_NUMBER = 2;
const string LOGFILENAME("logfile.txt");
const string FILE_NAME("basicfile");
const string MAP_NAME("mappingfile");
const string LOG_MUTEX_NAME("logmutexfile");
const string IO_MUTEX_NAME("iomutexfile");

// The class for the logging all the time

class LogFile
{
private:
    ostringstream *localStream;
    fstream file; // file itself
    //ostringstream *localStream2;
    //fstream file2;
    //time_t startTime; // iocnt1
    clock_t startTime; // starting time
    string fileName; // filename
    //string fileName2;
public:
    LogFile(string fileName/*, string fileName2*/);
    ~LogFile();
    size_t getTime();
    void log(string localMessage);
    //void log2(string localMessage2);
    void log(int type, size_t id, long long pageNum, bool isRead, int what);
    void flush();
    //void flush2();
};

int main()
{
    // Initializing

    const size_t maxCharSize = 256;
    size_t processID = GetCurrentProcessId();
    size_t processPage;
    size_t pause;
    string localBuffer;
    LogFile logFile(LOGFILENAME);

    // Getting page size

    SYSTEM_INFO temporarySystemInfo; // temporary item
    GetSystemInfo(&temporarySystemInfo); // getting system info
    const DWORD PAGE_SIZE = temporarySystemInfo.dwPageSize; // page size getting from system info

    // Opening mapped file

    HANDLE hMappedFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, MAP_NAME.c_str());
    if (hMappedFile == NULL) // error check
    {
        logFile.log("READER: Something wrong with opening mapped file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush();
        return GetLastError();
    }

    // Opening mutex for the log file

    HANDLE mLogFile = OpenMutexA(MUTEX_ALL_ACCESS, false, LOG_MUTEX_NAME.c_str());
    if (mLogFile == NULL) // error check
    {
        logFile.log("READER: Something wrong with opening mutex for logging file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush();
        return GetLastError();
    }

    // Getting all mutexes for the input and output

    HANDLE mInputOutput[PAGE_NUMBER];
    for (size_t i = 0; i < PAGE_NUMBER; ++i)
    {
        localBuffer = IO_MUTEX_NAME + std::to_string(i);

        mInputOutput[i] = OpenMutexA(MUTEX_ALL_ACCESS, false, localBuffer.c_str());
        if (mInputOutput[i] == NULL) // error check
        {
            logFile.log("READER: Something wrong with opening mutex for input/output number " + std::to_string(i) +  ". Error: " + std::to_string(GetLastError()));
            logFile.flush();
            return GetLastError();
        }
    }

    // View mapping file

    void* aMappedFile = MapViewOfFile(hMappedFile, FILE_MAP_READ, 0, 0, 0); // address of map view of file
    if (aMappedFile == NULL) // error check
    {
        logFile.log("READER: Something wrong with viewing mapped file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush();
        return GetLastError();
    }

    /*
    // [MATLAB] Opening mapped file

    HANDLE hMappedLogFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, false, MAP_NAME.c_str());
    if (hMappedLogFile == NULL) // error check
    {
        logFile.log2("READER: Something wrong with opening mapped file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush2();
        return GetLastError();
    }

    // [MATLAB] View mapping file

    void* aMappedLogFile = MapViewOfFile(hMappedLogFile, FILE_MAP_READ, 0, 0, 0); // address of map view of file
    if (aMappedLogFile == NULL) // error check
    {
        logFile.log2("READER: Something wrong with viewing mapped file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush2();
        return GetLastError();
    }
    */

    // Logging

    logFile.log("reader " + std::to_string(processID) + " started. ");

    // Blocking pages in RAM with VirtualLock

    VirtualLock(aMappedFile, PAGE_SIZE * PAGE_NUMBER);
    for(size_t gi = 0; gi < READ_TIMES_NUMBER; gi++)
    {
        // Page choosing

        //processPage = rand() % PAGE_NUMBER; // rand
        processPage = -1; // 1st free

        // LogFile starting

        logFile.log(1, processID, processPage, true, -1);

        // Catching mutex

        //WaitForSingleObject(mInputOutput[processPage], INFINITE); // rand
        processPage = WaitForMultipleObjects(PAGE_NUMBER, mInputOutput, FALSE, INFINITE); // 1st free

        // Choosing random place

        unsigned mem_src = *((unsigned*)((char*)aMappedFile + PAGE_SIZE*processPage));

        // LogFile accessing or changing

        logFile.log(2, processID, processPage, true, mem_src);
        pause = (rand() % 1001) + 500;
        Sleep((DWORD)pause);

        logFile.log(3, processID, processPage, true, -1);

        // Releasing mutex

        ReleaseMutex(mInputOutput[processPage]);

        // Waiting

        Sleep(10);
    }

    // All logs commonly writing in the file (AND start sycnhronizing)

    WaitForSingleObject(mLogFile, INFINITE);
    logFile.log("READER " + std::to_string(processID) + " has been ended.");
    logFile.flush(); // synchronizing the buffer
    ReleaseMutex(mLogFile);

    // ... AND end synchronizing

    // Unlocking page sizes

    VirtualUnlock(aMappedFile, PAGE_SIZE * PAGE_NUMBER);

    // Cleaning and freeing

    for (int i = 0; i < PAGE_NUMBER; ++i)
    {
        CloseHandle(mInputOutput[i]);
    }
    CloseHandle(mLogFile);
    CloseHandle(hMappedFile);
    UnmapViewOfFile(aMappedFile);

    return 0;
}

size_t LogFile::getTime() // getting time method
{
    size_t milisecFromStart = 0;
    //time_t endTime; // iocnt1
    //time(&endTime); // iocnt1
    //milisecFromStart = (size_t)(difftime(endTime, startTime) * 1000 + 0.5); // iocnt1
    clock_t endTime = clock();
    milisecFromStart = (size_t)(((double)(endTime - startTime) / CLOCKS_PER_SEC) * 1000 + 0.5);
    return milisecFromStart;
}

void LogFile::log(int type, size_t id, long long pageNum, bool isRead, int what = -1) // isRead true -- reader, false -- writer
{
    string localState;
    string localType;
    string ID = std::to_string(id);
    switch (type)
    {
        case 1:
            localState = "BW";
            break;
        case 2:
            if (isRead == true)
            {
                localState = "RD";
            }
            else
            {
                localState = "WR";
            }
            break;
        case 3:
            localState = "RL";
            break;
        default:
            localState = "FL";
            break;
    }

    if (isRead == true)
    {
        localType = "R";
    }
    else
    {
        localType = "W";
    }

    size_t milisecFromStart = getTime(); // checking the time
    string time = std::to_string(milisecFromStart); // translating the time to the string
    //string page = pageNum == -1 ? string("the first one released") : std::to_string(pageNum);
    string page = pageNum == -1 ? string("-1") : std::to_string(pageNum);
    string swhat = what == -1 ? "" : " byte " + std::to_string(what) + (isRead == true ? " from" : " to");
    //log(localType + " " + ID + " " + localState + swhat + " page " + page + " (time = " + time + " ms). ");
    log(ID + " " + localType + " " + localState + " " + page + " " + time);
    //log2(time);
    //flush2();
}

void LogFile::log (string localMessage) // logging the message
{
    (*localStream) << localMessage << "\n";
}

/*void LogFile::log2 (string localMessage2) // logging the message
{
    (*localStream2) << localMessage2;
}*/

void LogFile::flush () // flushing the message
{
    file << (*localStream).str() << "\n"; // write the message
    file.flush(); // flushing
    (*localStream).str(""); // setting the buffer to "null"
}

/*void LogFile::flush2 ()
{
    file2 << (*localStream2).str() << ", "; // write the message
    file2.flush(); // flushing
    (*localStream2).str(""); // setting the buffer to "null"
}*/

LogFile::LogFile (string fileName/*, string fileName2*/) // constructor
{
    this->fileName = fileName;
    localStream = new ostringstream();
    file.open(fileName, std::fstream::app | std::fstream::out);
    //time(&startTime); // iocnt1
    //this->fileName2 = fileName2;
    //localStream2 = new ostringstream();
    //file2.open(fileName2, std::fstream::app | std::fstream::out);
    startTime = clock();
}

LogFile::~LogFile () // destructor
{
    delete localStream;
    file.close();
    //delete localStream2;
    //file2.close();
}