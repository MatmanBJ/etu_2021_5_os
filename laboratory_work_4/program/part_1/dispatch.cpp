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

const size_t PAGE_NUMBER = 9 + 3 + 0 + 8 + 2 + 4 - 9;
const size_t READER_NUMBER = 9;
const size_t WRITER_NUMBER = 9;
const string FILE_NAME("basicfile");
const string MAP_NAME("mappingfile");
const string LOG_MUTEX_NAME("logmutexfile");
const string IO_MUTEX_NAME("iomutexfile");

// ---------- MAIN ----------

int main()
{
    size_t localCounter = 0;
	string localBuffer;
    HANDLE hProcesses[READER_NUMBER + WRITER_NUMBER];
    HANDLE hThreads[READER_NUMBER + WRITER_NUMBER];

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

    SetFilePointer(hBasicFile, PAGE_SIZE * PAGE_NUMBER, 0, FILE_BEGIN);
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

    HANDLE mLogFile = CreateMutexA(NULL, FALSE, LOG_MUTEX_NAME.c_str());
    if (mLogFile == NULL) // check, if it's wrong value
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

    HANDLE mInputOutput[PAGE_NUMBER];
    for (int i = 0; i < PAGE_NUMBER; i++)
    {
        localBuffer = IO_MUTEX_NAME + std::to_string(i);
        mInputOutput[i] = CreateMutexA(NULL, FALSE, localBuffer.c_str());
        if (mInputOutput[i] == NULL)
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

    for (int i = 0; i < READER_NUMBER; i++, localCounter++)
    {
        STARTUPINFOA localDestionation;
        SecureZeroMemory(&localDestionation, sizeof(STARTUPINFOA)); // "ZeroMemory" macro is unsecured!!!
        localDestionation.cb = sizeof(STARTUPINFOA);
        PROCESS_INFORMATION localInformation;

        WINBOOL localResult = CreateProcessA("writer.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &localDestionation, &localInformation);
        if (localResult == true)
        {
            cout << "WRITER " << i << " has been started.\n";
        }
        else
        {
            cout << "Problem with creating writer number " << i << " process. Last error number: " << GetLastError() << ".\n";
            return GetLastError();
        }

        hProcesses[localCounter] = localInformation.hProcess;
        hThreads[localCounter] = localInformation.hThread;
    }

    // Starting reader processes

    for (int i = 0; i < WRITER_NUMBER; i++, localCounter++)
    {
        STARTUPINFOA localDestionation;
        SecureZeroMemory(&localDestionation, sizeof(STARTUPINFOA)); // "ZeroMemory" macro is unsecured!!!
        localDestionation.cb = sizeof(STARTUPINFOA);
        PROCESS_INFORMATION localInformation;

        WINBOOL localResult = CreateProcessA("reader.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &localDestionation, &localInformation);
        if (localResult == true)
        {
            cout << "READER " << i << " has been started.\n";
        }
        else
        {
            cout << "Problem with creating reader number " << i << " process. Last error number: " << GetLastError() << ".\n";
            return GetLastError();
        }

        hProcesses[localCounter] = localInformation.hProcess;
        hThreads[localCounter] = localInformation.hThread;
    }

    // Waitig for all process finish

    WaitForMultipleObjects(READER_NUMBER + WRITER_NUMBER, hProcesses, TRUE, INFINITE);

    cout << "All process finished.\n";

    // Cleaning and freeing

    for (int i = 0; i < READER_NUMBER + WRITER_NUMBER; i++)
    {
        CloseHandle(hProcesses[i]);
        CloseHandle(hThreads[i]);
    }
    for (int i = 0; i < PAGE_NUMBER; i++)
    {
        CloseHandle(mInputOutput[i]);
    }
    CloseHandle(mLogFile);
    CloseHandle(hMappedFile);
    CloseHandle(hBasicFile);

    // End

    cout << "End.\n";

    return 0;
}