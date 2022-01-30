/*

Program:

Saint-Petersburg ETU OS laboratory work 4 part 2

Author:

Matvey Sobolev, 2021

Compiler:

g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0
Copyright (C) 2018 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

*/

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

const size_t maxBufferSize = 256;
char localMessage[maxBufferSize];

void endRoutine (DWORD dwE, DWORD dwBT, LPOVERLAPPED lpO);

int main()
{
    int menuChoose = 1;
    HANDLE hPipe = INVALID_HANDLE_VALUE;
    cout << "---------- CLIENT APP ----------\n";

    while (menuChoose != 0)
    {
        menuChoose = -1;
        while (menuChoose < 0 || menuChoose > 2)
        {
            cout << "\n";
            cout << "0 -- Exit\n";
            cout << "1 -- Connect to a named pipe\n";
            cout << "2 -- Read from the pipe\n";
            cout << "Please, input the choosed item: ";
            cin >> menuChoose;
            if (menuChoose < 0 || menuChoose > 2)
            {
                cout << "Please, try again!\n";
            }
            else
            {
                cout << "\n";
            }
        }
        char localPipeName[maxBufferSize];
        //char localMessage[maxBufferSize];
        switch(menuChoose)
        {
            case 1:
                if(hPipe != INVALID_HANDLE_VALUE)
                {
                    CloseHandle(hPipe);
                }
                cout << "Please, write the name of the pipe (" << maxBufferSize << " max,begin w/ \\\\.\\pipe\\): ";
                cin >> localPipeName;
                hPipe = CreateFile(localPipeName, GENERIC_READ, 0UL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY | FILE_FLAG_OVERLAPPED, NULL);
                if(hPipe != INVALID_HANDLE_VALUE)
                {
                    cout << "Connected!\n";
                }
                else
                {
                    cout << "Not connected! Error code is " << GetLastError() << "\n";
                }
                break;
            case 2:
                OVERLAPPED overlapped;
                overlapped.Offset = 0UL;
                overlapped.OffsetHigh = 0UL;
				cout << "Waiting for message ... [if no from server, the program will wait]\n";
				ReadFileEx(hPipe, localMessage, sizeof(localMessage), &overlapped, &endRoutine); // handle / place for message / size of the message / end function
                SleepEx(INFINITE, true);
                break;
        }
    }

    // Cleaning and freeing
    
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hPipe);
    }
    return 0;
}

void endRoutine (DWORD dwE, DWORD dwBT, LPOVERLAPPED lpO)
{
    cout << localMessage << "\n";
	cout << "Message received!\n";
}