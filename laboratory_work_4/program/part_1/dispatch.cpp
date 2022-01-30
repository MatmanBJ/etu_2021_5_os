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
#include <malloc.h>

using namespace std;

// ---------- CONSTANTS DECLARATION ----------

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

// ---------- FUNCTION PROTOTYPES ----------

char* parse4Env(string s_id, string s_matshift);

// ---------- MAIN ----------

int main()
{
    size_t li = 0;
    size_t MATSHIfT = 0;
    char* envs[N_R + N_W];
	string buffS;
    HANDLE prs_rw[N_R + N_W];
    HANDLE ths_rw[N_R + N_W];

	// Getting page size

	SYSTEM_INFO temporarySystemInfo; // temporary item
    GetSystemInfo(&temporarySystemInfo); // getting system info
    const DWORD PAGE_SIZE = temporarySystemInfo.dwPageSize; // page size getting from system info

    // Creating base file, which will be mapped

    HANDLE hBasicFile = CreateFileA(FILE_NAME.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (hBasicFile == INVALID_HANDLE_VALUE) // check, if it's wrong value
    {
        cout << "Basic file hasn't been created (something wrong). Last error code: " << GetLastError() << ".\n";
        return GetLastError();
    }
    else // successful creation
    {
    	cout << "Basic file has been created successfully!\n";
    }

    SetFilePointer(hBasicFile, PAGE_SIZE * PAGE_NUM, 0, FILE_BEGIN);
    SetEndOfFile(hBasicFile);

    // Making mapped file from basic file

    HANDLE hMappedFile = CreateFileMappingA(hBasicFile, NULL, PAGE_READWRITE, 0, 0, MAP_NAME.c_str());
    if (hMappedFile == NULL) // check, if it's wrong value
    {
        cout << "Hasn't been maked mapped file from basic file (something wrong). Last error code: " << GetLastError() << ".\n";
        CloseHandle(hBasicFile); // closing successfully created file handle (see previous if/else)
        return GetLastError();
    }
    else if (GetLastError() == ERROR_ALREADY_EXISTS) // check, if it's "existing" value
    {
        cout << "Hasn't been maked mapped file from basic file (already exists). Last error code: " << GetLastError() << ".\n";
        CloseHandle(hBasicFile); // closing successfully created file handle (see previous if/else)
        return GetLastError();
    }
    else // successful making
    {
    	cout << "Has been maked mapped file from basic file successfully!\n";
    }

    // Creating mutex for logging

    HANDLE logMutex = CreateMutexA(NULL, FALSE, LOG_MUTEX_NAME.c_str());
    if (logMutex == NULL) // check, if it's wrong value
    {
        cout << "Hasn't been created mutex for logging file (something wrong). Last error code: " << GetLastError() << ".\n";
        return GetLastError();
    }
    else if (GetLastError() == ERROR_ALREADY_EXISTS) // check, if it's "existing" value
    {
    	cout << "Hasn't been created mutex for logging file (already exists). Last error code: " << GetLastError() << ".\n";
    	return GetLastError();
    }
    else
    {
    	cout << "Has been created mutex for logging file successfullt!\n";
    }

    // Creating mutexes for all of the pages

    // !!!!!
    // NEED TO REWRITE CONSTANT AND ALL THE NAMES

    HANDLE io_mutexs[PAGE_NUM];
    for (size_t i = 0; i < PAGE_NUM; i++)
    {
        buffS = IO_MUTEX_NAME + std::to_string(i);
        io_mutexs[i] = CreateMutexA(NULL, FALSE, buffS.c_str());
        if (io_mutexs[i] == NULL)
        {
            cout << "Hasn't been created mutex for input/output (something wrong). Last error code: " << GetLastError() << ".\n";
            return GetLastError();
        }
        else if (GetLastError() == ERROR_ALREADY_EXISTS) // check, if it's "existing" value
        {
        	cout << "Hasn't been created mutex for input/output (already exists). Last error code: " << GetLastError() << ".\n";
        	return GetLastError();
        }
    }

    // Starting writer processes

    for (size_t i = 0; i < N_R; i++, li++, MATSHIfT+=(MAT_FILE_SIZE_BLOCK/sizeof(size_t)))
    {
        STARTUPINFOA si;
        SecureZeroMemory(&si, sizeof(STARTUPINFOA)); // "ZeroMemory" macro is unsecured!!!
        si.cb = sizeof(STARTUPINFOA);
        PROCESS_INFORMATION pi;

        string s_id = "PR_ID=" + std::to_string(i);
        string smatshift = "matshift=" + std::to_string(MATSHIfT);
        envs[li] = parse4Env(s_id, smatshift);

        WINBOOL resCreate = CreateProcessA("writer.exe", NULL, NULL, NULL, FALSE, 0, envs[li], NULL, &si, &pi);
        if (resCreate == true)
        {
            cout << "writer " << i << " started. " << endl;
        }
        else
        {
            cout << "Problem with creating writer " << i << " process (error " << GetLastError() << "). " << endl;
            return GetLastError();
        }

        prs_rw[li] = pi.hProcess;
        ths_rw[li] = pi.hThread;
    }

    // Starting reader processes

    for (size_t i = 0; i < N_W; i++, li++, MATSHIfT+=(MAT_FILE_SIZE_BLOCK/sizeof(size_t)))
    {
        STARTUPINFOA si;
        SecureZeroMemory(&si, sizeof (STARTUPINFOA)); // "ZeroMemory" macro is unsecured!!!
        si.cb = sizeof (STARTUPINFOA);
        PROCESS_INFORMATION pi;

        string s_id = "PR_ID=" + std::to_string(i);
        string smatshift = "matshift=" + std::to_string(MATSHIfT);
        envs[li] = parse4Env(s_id, smatshift);

        WINBOOL resCreate = CreateProcessA("reader.exe", NULL, NULL, NULL, FALSE, 0, envs[li], NULL, &si, &pi);
        if (resCreate)
        {
            cout << "reader " << i << " started. " << endl;
        }
        else
        {
            cout << "Problem with creating reader " << i << " process (error " << GetLastError() << "). " << endl;
            return GetLastError();
        }

        prs_rw[li] = pi.hProcess;
        ths_rw[li] = pi.hThread;
    }

    // Waitig for all process finish

    WaitForMultipleObjects(N_R + N_W, prs_rw, TRUE, INFINITE);

    cout << "All process finished.\n";

    // Cleaning and freeing

    for(size_t i = 0; i < N_R + N_W; i++)
    {
        CloseHandle(prs_rw[i]);
        CloseHandle(ths_rw[i]);
    }
    for(size_t i = 0; i < PAGE_NUM; i++)
    {
        CloseHandle(io_mutexs[i]);
    }
    CloseHandle(logMutex);
    CloseHandle(hMappedFile);
    CloseHandle(hBasicFile);

    // End

    cout << "End.\n";

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

    size_t n = s_id.length() + 1 + s_matshift.length() + 1 + real_npath + 1 + 1;
    char *res = (char*)malloc(n*sizeof(char));

    size_t i, j;
    for(i = 0, j = 0; j < s_id.length(); j++, i++)
    {
        res[i] = s_id[j];
    }
    res[i++] = '\0';

    for(j = 0; j < s_matshift.length(); j++, i++)
    {
        res[i] = s_matshift[j];
    }
    res[i++] = '\0';

    res[i++] = 'P';
    res[i++] = 'a';
    res[i++] = 't';
    res[i++] = 'h';
    res[i++] = '=';
    for(j = 0; j < real_npath; j++, i++)
    {
        res[i] = path[j];
    }
    res[i++] = '\0';

    res[i++] = '\0';

    return res;
}