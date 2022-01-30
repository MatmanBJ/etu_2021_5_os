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

#define IOcount 0

using namespace std;

const size_t PAGE_NUM = 20;
const size_t N_R = 10;
const size_t N_W = 10;
const size_t N_TIMES = 12;
const string LOGFILENAME("loggerino.txt");
const string FILE_NAME("lab4_FILE");
const string MAP_NAME("lab4_FILEMAPPING");
const string LOG_MUTEX_NAME("lab4_LOG_MUTEX");
const string IO_MUTEX_NAME("lab4_IO_MUTEX");

class LogFile
{
private:
    ostringstream *ss;
    fstream file;
    #if IOcount == 1
    time_t startTime;
    #else
    clock_t startTime;
    #endif
    string fileName;
public:
    LogFile(string fileName);
    ~LogFile();
    size_t getTime();
    void log(string msg);
    void log(int type, size_t id, long long pageNum, bool r_or_w, int what);
    void flush();
};

int main()
{
    // Initializing

    string buffS;
    const size_t nBuffC = 256;
    char buffC[nBuffC];
    LogFile logFile(LOGFILENAME);

    // Getting page size

    SYSTEM_INFO temporarySystemInfo; // temporary item
    GetSystemInfo(&temporarySystemInfo); // getting system info
    const DWORD PAGE_SIZE = temporarySystemInfo.dwPageSize; // page size getting from system info

    // Opening mapped file

    HANDLE hMappedFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, MAP_NAME.c_str());
    if (hMappedFile == NULL) // error check
    {
        logFile.log("WRITER: Something wrong with opening mapped file. Last error code: " + std::to_string(GetLastError())); // RW CHANGE = {reader, writer}
        logFile.flush();
        return GetLastError();
    }

    // Opening mutex for the log file

    HANDLE mLogFile = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, LOG_MUTEX_NAME.c_str());
    if (mLogFile == NULL) // error check
    {
        logFile.log("WRITER: Something wrong with opening mutex for logging file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush();
        return GetLastError();
    }

    // Getting all mutexes for the input and output

    HANDLE mInputOutput[PAGE_NUM];
    for (size_t i = 0; i < PAGE_NUM; ++i)
    {
        buffS = IO_MUTEX_NAME + std::to_string(i);

        mInputOutput[i] = OpenMutexA(MUTEX_ALL_ACCESS, FALSE, buffS.c_str());
        if(mInputOutput[i] == NULL) // error check
        {
            logFile.log("WRITER: Something wrong with opening mutex for input/output №" + std::to_string(i) +  ". Last error code: " + std::to_string(GetLastError()));
            logFile.flush();
            return GetLastError();
        }
    }

    // View mapping file

    void* addr = MapViewOfFile(hMappedFile, FILE_MAP_WRITE, 0, 0, 0);
    if (addr == NULL) // error check
    {
        logFile.log("WRITER: Something wrong with viewing mapped file. Last error code: " + std::to_string(GetLastError()));
        logFile.flush();
        return GetLastError();
    }

    //===============ENV_INIT===============
    GetEnvironmentVariableA("PR_ID", buffC, nBuffC);
    size_t prID = atoll(buffC);
    srand(time(NULL) + prID*13);
    //===============ENV_INIT===============

    logFile.log("WRITER №" + std::to_string(prID) + " has been started!"); // RW CHANGE = {reader, writer}

    size_t page_i;
    size_t pause;

    VirtualLock(addr, PAGE_SIZE * PAGE_NUM);
    for(size_t gi = 0; gi < N_TIMES; ++gi)
    {
        #if RND_CHOOSE == 1
        page_i = rand() % PAGE_NUM;
        #else
        page_i = -1;
        #endif

        //===============LOG_BEGIN_WAIT===============
        logFile.log(1, prID, page_i, false, -1);
        //===============LOG_BEGIN_WAIT===============

        #if RND_CHOOSE == 1
        WaitForSingleObject(mInputOutput[page_i], INFINITE);
        //Если неудачно, то залозинить это и выйти... //too much close and free=/
        #else
        page_i = WaitForMultipleObjects(PAGE_NUM, mInputOutput, FALSE, INFINITE);
        //Если неудачно, то залозинить это и выйти... //too much close and free=/
        #endif

        unsigned mem_src = rand() % 256;
        *((unsigned*)((char*)addr + PAGE_SIZE*page_i)) = mem_src; /* RW CHANGE = {unsigned mem_src = *((unsigned*)((char*)addr + PAGE_SIZE*page_i));, 
                                                                   unsigned mem_src = rand() % 256; *((unsigned*)((char*)addr + PAGE_SIZE*page_i)) = mem_src; }*/

        //===============LOG_READING/WRITING===============
        logFile.log(2, prID, page_i, false, mem_src); // RW CHANGE = {true, false}
        //===============LOG_READING/WRITING===============
        pause = (rand() % 1001) + 500;

        Sleep((DWORD)pause);

        //===============LOG_RELEASING===============
        logFile.log(3, prID, page_i, false, -1); // RW CHANGE = {true, false}
        //===============LOG_RELEASING===============

        ReleaseMutex(mInputOutput[page_i]);

        Sleep(10);
    }

    // All logs commonly writing in the file (AND start sycnhronizing)

    WaitForSingleObject(mLogFile, INFINITE);
    logFile.log("WRITER №" + std::to_string(prID) + " has been ended!"); // RW CHANGE = {reader, writer}
    logFile.flush();
    ReleaseMutex(mLogFile);

    // ... AND end synchronizing

    // Unlocking page sizes

    VirtualUnlock(addr, PAGE_SIZE * PAGE_NUM);

    // Cleaning and freeing

    for(size_t i = 0; i < PAGE_NUM; i++)
    {
        CloseHandle(mInputOutput[i]);
    }
    CloseHandle(mLogFile);
    CloseHandle(hMappedFile);
    UnmapViewOfFile(addr);

    return 0;
}

size_t LogFile::getTime()
{
    size_t milisecFromStart = 0;
    #if IOcount == 1
    time_t endTime;
    time(&endTime);
    milisecFromStart = (size_t)(difftime(endTime, startTime) * 1000 + 0.5);
    #else
    clock_t endTime = clock();
    milisecFromStart = (size_t)(((double)(endTime - startTime) / CLOCKS_PER_SEC) * 1000 + 0.5);
    #endif
    return milisecFromStart;
}

/*
type:
1 - начало  ожидания,  
2 - запись  или  чтение, 
3 - переход  к освобождению
r_or_w:
true = reader
flase = writer
*/
void LogFile::log(int type, size_t id, long long pageNum, bool r_or_w, int what = -1)
{
    string reason;
    switch(type)
    {
        case 1:
            reason = "begin wait";
            break;
        case 2:
            if(r_or_w)
            {
                reason = "reading";
            }
            else
            {
                reason = "writing";
            }
            break;
        case 3:
            reason = "releasing";
            break;
        default:
            reason = "Failed successfully=/";
            break;
    }

    string ID = std::to_string(id);

    string rw = "";
    if(r_or_w)
    {
        rw = "reader";
    }
    else
    {
        rw = "writer";
    }

    size_t milisecFromStart = getTime();
    string time = std::to_string(milisecFromStart);
    
    string page = pageNum==-1 ? string("the first one released") : std::to_string(pageNum);

    string swhat = what==-1 ? "" : " byte " + std::to_string(what) + (r_or_w==true?" from":" to");

    log(  rw + " " + ID + " " + reason + swhat + " page " + page + " (time = " + time + " ms). " );
}

void LogFile::log(string msg)
{
    (*ss) << msg << endl;
}

void LogFile::flush()
{
    file << (*ss).str() << endl;
    file.flush();
    (*ss).str("");
}

LogFile::LogFile(string fileName)
{
    this->fileName = fileName;
    ss = new ostringstream();
    file.open(fileName, std::fstream::app | std::fstream::out);
    #if IOcount == 1
    time(&startTime);
    #else
    startTime = clock();
    #endif
}

LogFile::~LogFile()
{
    delete ss;
    file.close();
}