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

#include <windows.h>
#include <string>
#include <iostream>
#include <malloc.h>

using namespace std;

const size_t PAGE_NUM = 20;
const size_t N_R = 10;
const size_t N_W = 10;
const size_t N_TIMES = 12;
const string LOGFILE("loggerino.txt");
const string FILE_NAME("lab4_FILE");
const string MAP_NAME("lab4_FILEMAPPING");
const string LOG_MUTEX_NAME("lab4_LOG_MUTEX");
const string IO_MUTEX_NAME("lab4_IO_MUTEX");

const size_t MAT_FILE_SIZE_BLOCK = N_TIMES*3*2*sizeof(size_t);

DWORD getPageSize();

char* parse4Env(string s_id, string s_matshift);

int main()
{
    const DWORD PAGE_SIZE = getPageSize();

    string buffS;

    HANDLE hFile = CreateFileA(FILE_NAME.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, 0, NULL);
    
    if(hFile == INVALID_HANDLE_VALUE)
    {
        cout << "Problem with create file: \"" << FILE_NAME << "\" (error " << GetLastError() << "). " << endl;
        return GetLastError();
    }
    cout << "* CreateFileA - OK" << endl;

    SetFilePointer(hFile, PAGE_SIZE * PAGE_NUM, 0, FILE_BEGIN);
    SetEndOfFile(hFile);

    HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, 0, MAP_NAME.c_str());
    if(hMap == NULL)
    {
        cout << "Problem in CreateFileMapping. Error: " << GetLastError() << endl;
        CloseHandle(hFile);
        return GetLastError();
    }
    cout << "* CreateFileMapping - OK" << endl;

    HANDLE logMutex = CreateMutexA(NULL, FALSE, LOG_MUTEX_NAME.c_str());
    if(logMutex == NULL)
    {
        cout << "Problem with creating logMutex. Error: " << GetLastError() << endl;
        //too much close and free=/
        return GetLastError();
    }
    HANDLE io_mutexs[PAGE_NUM];
    for(size_t i = 0; i < PAGE_NUM; ++i)
    {
        buffS = IO_MUTEX_NAME + std::to_string(i);
        io_mutexs[i] = CreateMutexA(NULL, FALSE, buffS.c_str());
        if(io_mutexs[i] == NULL)
        {
            cout << "Problem with creating io_mutexs " << i << ". Error: " << GetLastError() << endl;
            //too much close and free=/
            return GetLastError();
        }
    }

    char* envs[N_R + N_W];
    
    HANDLE prs_rw[N_R + N_W];
    HANDLE ths_rw[N_R + N_W];
    size_t li = 0;
    size_t MATSHIfT = 0;

    for (size_t i = 0; i < N_R; ++i, ++li, MATSHIfT+=(MAT_FILE_SIZE_BLOCK/sizeof(size_t)))
    {
        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(STARTUPINFOA));
        si.cb = sizeof(STARTUPINFOA);
        PROCESS_INFORMATION pi;

        //cout << "writer " << i + 1 << " not commited. " << endl;

        string s_id = "PR_ID=" + std::to_string(i);
        string smatshift = "matshift=" + std::to_string(MATSHIfT);
        envs[li] = parse4Env(s_id, smatshift);

        WINBOOL resCreate = CreateProcessA("writer.exe", NULL, NULL, NULL, FALSE, 0, envs[li], NULL, &si, &pi);
        if(resCreate)
            cout << "writer " << i << " started. " << endl;
        else
        {
            cout << "Problem with creating writer " << i << " process (error " << GetLastError() << "). " << endl;
            //too much close and free=/
            return GetLastError();
        }

        prs_rw[li] = pi.hProcess;
        ths_rw[li] = pi.hThread;
        Sleep(3);
    }

    for (size_t i = 0; i < N_W; ++i, ++li, MATSHIfT+=(MAT_FILE_SIZE_BLOCK/sizeof(size_t)))
    {
        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(STARTUPINFOA));
        si.cb = sizeof(STARTUPINFOA);
        PROCESS_INFORMATION pi;

        string s_id = "PR_ID=" + std::to_string(i);
        string smatshift = "matshift=" + std::to_string(MATSHIfT);
        envs[li] = parse4Env(s_id, smatshift);

        WINBOOL resCreate = CreateProcessA("reader.exe", NULL, NULL, NULL, FALSE, 0, envs[li], NULL, &si, &pi);
        if(resCreate)
            cout << "reader " << i << " started. " << endl;
        else
        {
            cout << "Problem with creating reader " << i << " process (error " << GetLastError() << "). " << endl;
            //too much close and free=/
            return GetLastError();
        }

        prs_rw[li] = pi.hProcess;
        ths_rw[li] = pi.hThread;
        Sleep(3);
    }

    WaitForMultipleObjects(N_R + N_W, prs_rw, TRUE, INFINITE);
    // if(неудачно), то сообщить и выйти... //too much close and free=/

    cout << "All process finished. " << endl;

    //===============Cleaning===============
    for(size_t i = 0; i < N_R + N_W; ++i)
    {
        CloseHandle(prs_rw[i]);
        CloseHandle(ths_rw[i]);
    }
    for(size_t i = 0; i < PAGE_NUM; ++i)
        CloseHandle(io_mutexs[i]);
    CloseHandle(logMutex);
    CloseHandle(hMap);
    CloseHandle(hFile);
    //UnmapViewOfFile(map);
    //===============Cleaning===============

    cout << "=====done!=====" << endl;
    return 0;
}

char* parse4Env(string s_id, string s_matshift)
{
    const size_t npath = 4096*8;
    char path[npath];
    size_t real_npath = GetEnvironmentVariableA("Path", path, npath); // Когда в CreateProcessA параметр lpEnvironment != NULL, то по умолчанию все остальные переменные среды стираются.
    real_npath += 5;                                                 // Поэтому приходится делать так, чтобы "процессы-дети" смогли "подтянуть" библиотеки
                                /*
                                Зачем вообще это нужно? Зачем вообще lpEnvironment? 
                                Есть 3 возможных варианта:
                                1) Если нужно просто узнать "id" процесса, то можно обойтись вызовом функции GetCurrentProcessId().
                                2) В первый аргумент CreateProcessA можно сразу передавать аргументы процессу, а main() сделать таким "int main(int argc, char **argv)". 
                                    Например, так: CreateProcessA("reader.exe arg1 arg2 arg3 et al", ...)
                                    После этого "arg1 arg2 arg3 et al" можно получить из argv.
                                3) С помощью переменных среды. Самый неудачный и сложный способ. Он как раз тут и используется...
                                Наверное, можно передавать ещё с помощью:
                                    буфферного файла (куда диспетчер запишет аргументы, а reader считает).
                                    сделать водопровод. Пайпинг (но это lab 4 work 2).
                                */

    size_t n = s_id.length()+1 + s_matshift.length()+1 + real_npath+1 + 1;
    char *res = (char*)malloc(n*sizeof(char));

    size_t i, j;
    for(i = 0, j = 0; j < s_id.length(); ++j, ++i)
        res[i] = s_id[j];
    res[i++] = '\0';

    for(j = 0; j < s_matshift.length(); ++j, ++i)
        res[i] = s_matshift[j];
    res[i++] = '\0';

    res[i++] = 'P';res[i++] = 'a';res[i++] = 't';res[i++] = 'h';res[i++] = '=';
    for(j = 0; j < real_npath; ++j, ++i)
        res[i] = path[j];
    res[i++] = '\0';

    res[i++] = '\0';

    /*for(i = 0; i < n; ++i)
        if(res[i] == '\0')
            cout << 0;
        else
            cout << res[i];*/

    return res;
}

DWORD getPageSize()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwPageSize;
}