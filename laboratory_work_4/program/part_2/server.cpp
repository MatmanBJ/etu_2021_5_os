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

int main()
{
    int menuChoose = 1; // choosed value
    HANDLE hPipe = INVALID_HANDLE_VALUE; // default value
    HANDLE hEvent = NULL; // default value
    cout << "---------- SERVER APP ----------\n";

    while (menuChoose != 0)
    {
        menuChoose = -1;
        while (menuChoose < 0 || menuChoose > 7)
        {
            cout << "\n";
            cout << "0 -- Exit\n";
            cout << "1 -- Create the named pipe (only one)\n";
            cout << "2 -- Wait for client connect to pipe\n";
            cout << "3 -- Disconnect client from pipe\n";
            cout << "4 -- Close the named pipe (only one)\n";
            cout << "5 -- Write to pipe\n";
            cout << "6 -- Create the event\n";
            cout << "Please, input item you choosed: ";
            cin >> menuChoose;
            if (menuChoose < 0 || menuChoose > 7)
            {
                cout << "Something wrong! Please, try again!\n";
            }
            else
            {
                cout << "\n";
            }
        }
        switch (menuChoose)
        {
            case 1: // Create the named pipe (only one)
                if (hPipe == INVALID_HANDLE_VALUE) // error check
                {
                    char localPipeName[maxBufferSize];
                    cout << "Please, write pipe name (128 max, begin w/ \\\\.\\pipe\\): ";
                    cin >> localPipeName;
                    hPipe = CreateNamedPipe(localPipeName, PIPE_ACCESS_OUTBOUND, PIPE_TYPE_BYTE | PIPE_WAIT,
                        PIPE_UNLIMITED_INSTANCES, maxBufferSize, 0UL, 0UL, NULL);
                    if (hPipe != INVALID_HANDLE_VALUE)
                    {
                        cout << "Pipe is created!\n";
                    }
                    else
                    {
                        cout << "Pipe is not created! Last error code: " << GetLastError() << ".\n";
                    }
                }
                else
                {
                    cout << "Pipe is created already!\n";
                }
                break;
            case 2: // Wait for client connect to pipe
                if (hPipe == INVALID_HANDLE_VALUE)
                {
                    cout << "No pipe.\n";
                }
                else if (ConnectNamedPipe(hPipe, NULL))
                {
                    cout << "Connected!\n";
                }
                else
                {
                    cout << "No connection! Last error code is " << GetLastError() << "\n";
                }
                break;
            case 3: // Disconnect client from pipe
                if (hPipe == INVALID_HANDLE_VALUE)
                {
                    cout << "No pipe.\n";
                }
                else if (DisconnectNamedPipe(hPipe))
                {
                    cout << "Disconnected!\n";
                }
                else
                {
                    cout << "No connection! Last error code is " << GetLastError() << "\n";
                }
                break;
            case 4: // Close the named pipe (only one)
                if (hPipe != INVALID_HANDLE_VALUE)
                {
                    DisconnectNamedPipe(hPipe);
                    CloseHandle(hPipe);
                    hPipe = INVALID_HANDLE_VALUE;
                    cout << "Pipe closed!\n";
                }
                break;
            case 5: // Write to pipe
                if (hPipe == INVALID_HANDLE_VALUE)
                {
                    cout << "No pipe.\n";
                }
                else if (hEvent == NULL)
                {
                    cout << "No event.\n";
                }
                else
                {
                    bool localSuccess = false; // start -- false
                    OVERLAPPED overlapped; // creating overlapped structure
                    overlapped.Offset = 0UL; // offset
                    overlapped.OffsetHigh = 0UL; // offset high
                    overlapped.hEvent = hEvent; // event set
                    string localMessage;
                    cout << "Please, input message (" << maxBufferSize << " max):\n";
                    fflush(stdin);
                    cin >> localMessage;
                    localSuccess = WriteFile(hPipe, localMessage.c_str(), maxBufferSize, NULL, &overlapped);
                    WaitForSingleObject(hEvent, INFINITE);

                    if(localSuccess == true)
                    {
                        cout << "Message in pipe!\n";
                    }
                    else
                    {
                        cout << "Message is not in pipe! Last error code: " << GetLastError() << ".\n";
                    }
                }
                break;
            case 6: // Create the event
                if (hEvent == NULL)
                {
                    hEvent = CreateEvent(NULL, false, false, NULL);
                    if(hEvent != NULL)
                    {
                        cout << "Event created!\n";
                    }
                    else
                    {
                        cout << "Event isn't created! Last error code is " << GetLastError() << "\n";
                    }
                }
                else
                {
                    cout << "Pipe is already created.\n";
                }
                break;
        }
    }

    // Cleaning and freeing

    if (hPipe != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hPipe);
    }
    if (hEvent != NULL)
    {
        CloseHandle(hEvent);
    }

    // Return

    return 0;
}