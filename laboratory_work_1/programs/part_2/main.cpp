#include <iostream> // for everything
#include <windows.h> // for API
#include <string> // for tring usage
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

int LocalFileGenerator (string localOldFilePath, unsigned long long localBytesRequest);
void CopyPaste (string localOldFilePath, string localNewFilePath);
DWORD PreparingCopyPaste(string localOldFilePath, string localNewFilePath, unsigned long long localBlockSize, unsigned long long localOverlappedIOSize);
DWORD LocalCopyPaste (HANDLE in, HANDLE out, unsigned long long fileSize, unsigned long long bs, unsigned long long localOverlappedIOSize);
DWORD LocalDriveSectorSize ();
DWORD LocalDriveSectorSize (DWORD &localSectorsPerCluster);
unsigned long long DWORDS2ULL(DWORD l, DWORD h);
void ULL2DWORDS(unsigned long long value, DWORD* l, DWORD* h);
void CALLBACK CompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);
//unsigned long long getOverlappedNum(LPOVERLAPPED lpOverlapped);

//Certutil -hashfile file

unsigned long long callback;
unsigned long long globalFirstAdress;
unsigned long long oneSize;
unsigned long long callLeft;
unsigned long long copyTime = 0;

/*
Поменять эти значения на те, которые вам необходимы.

При первом замере (когда кол-во перекрывающихся операций = 1 и меняется):
    bs_b: означает, что минимальный размер копируемого блока при асинхронном копировании будет = bs_b * (размер сектора)
    bs_e: означает, что максимальный размер копируемого блока при асинхронном копировании будет = bs_e * (размер сектора)

При втором замере (размер копируемого блона постоянен и меняется кол-во перекрывающихся операций ввода и вывода):
    bs_std: означает, что при втором замере размер копируемого блока всегда будет = bs_std * (размер сектора)
    thNum_b: означает, что минимальное кол-во перекрывающихся операций ввода и вывода будет = thNum_b
    thNum_e: означает, что максимальное кол-во перекрывающихся операций ввода и вывода будет = thNum_e
*/

const unsigned bs_b = 20;
const unsigned bs_e = 70;

const unsigned bs_std = 16;
const unsigned thNum_b = 1;
const unsigned thNum_e = 15;

// ---------- MAIN ----------

int main(int argc, char **argv)
{
    if (argc == 4)
    {
    	const unsigned long oldFileBytes = atoi(argv[2]);
        const string oldFilePath(argv[1]);
        const string newFilePath(argv[3]);
        LocalFileGenerator (oldFilePath, oldFileBytes);
        cout << "Created file number of bytes: " << oldFileBytes << " Created file path: " << oldFilePath << "\n";
        CopyPaste(oldFilePath, newFilePath);
        cout << "Copied file path: " << oldFilePath << " Pasted file path: " << newFilePath << "\n";

        return 0;
    }
    else
    {
    	cout << "Incorrect start of executable file. Please, check your flags!\n";
        return -1;
    }
}

// ---------- FILE GENERATION FOR COPY TESTS ----------

int LocalFileGenerator (string localOldFilePath, unsigned long long localBytesRequest)
{
	unsigned long long i;
    ofstream localFile (localOldFilePath, ios :: out | ios :: binary | ios :: app);
    srand (time(NULL));
    char localByte[10];
    for (i = 0; i < localBytesRequest; i++)
    {
        localByte[0] = (unsigned char)(rand() % 256);
        localFile.write (localByte, sizeof(localByte));
    }
    localFile.close();
    return 0;
}

// ---------- COPY AND PASTE MAIN PROCESS ----------

void CopyPaste (string localOldFilePath, string localNewFilePath)
{
    //DWORD time;
    DWORD sectorsPerCluster;
    DWORD localSectorSize = LocalDriveSectorSize (sectorsPerCluster);
    unsigned long long localBlockSize; // size of the data block I will copy
    unsigned long long localOverlappedIOSize; // number of operations for Overlapped I/O

    cout << "Drive sector size: " << localSectorSize << " bytes\n";
    cout << "Drive cluster size: " << sectorsPerCluster << " sectors (" << localSectorSize*sectorsPerCluster << " bytes).\n";

    int localBlockSizeIteration = 0; // iterator
    
    localOverlappedIOSize = 1; // overlapped IO number

    localBlockSize = localSectorSize*bs_b;
    copyTime = 0;
    copyTime = PreparingCopyPaste(localOldFilePath, localNewFilePath, localBlockSize, 1);
    cout << localBlockSizeIteration << ".\n"
    << "Current block size to copy: " << localBlockSize << ";\n"
    << "Current overlapped IO number: " << localOverlappedIOSize << ";\n"
    << "Overlapped copy time: " << copyTime << ".\n";

    /*int localOverlappedIOSizeIteration = 0; // iterator

    localBlockSize = bs_std*localSectorSize; // block size (block size we will copy) = blocks number (how many?) * sector size (on disk)

    localOverlappedIOSize = thNum_b;
    copyTime = PreparingCopyPaste(localOldFilePath, localNewFilePath, localBlockSize, localOverlappedIOSize);
    cout << localOverlappedIOSizeIteration << ".\n"
    << "Current block size to copy: " << localBlockSize << ";\n"
    << "Current overlapped IO number: " << localOverlappedIOSize << ";\n"
    << "Overlapped copy time: " << copyTime << "\n";*/
}

// ---------- PREPARING FOR COPY AND PASTE ACTIONS ----------

DWORD PreparingCopyPaste(string localOldFilePath, string localNewFilePath, unsigned long long localBlockSize, unsigned long long localOverlappedIOSize)
{
    DWORD localActionTime = -1;
    
    HANDLE localOldFileHandle = CreateFileA(localOldFilePath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL); // copied file path
    HANDLE localNewFileHandle = CreateFileA(localNewFilePath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL); // new file path
    DWORD lpFileSizeHigh; // lpdword filesize high
    DWORD getFileSize;
    unsigned long long fileSize = DWORDS2ULL(getFileSize, lpFileSizeHigh);

    if (localOldFileHandle == NULL || localOldFileHandle == INVALID_HANDLE_VALUE || localNewFileHandle == NULL || localNewFileHandle == INVALID_HANDLE_VALUE)
    {
        cout << "Problem with opening files!\n";
        cout << "\nError message: " << GetLastError() << "\n";
    }
    else
    {
        getFileSize = GetFileSize(localOldFileHandle, &lpFileSizeHigh);
        fileSize = DWORDS2ULL(getFileSize, lpFileSizeHigh);

        localActionTime = LocalCopyPaste(localOldFileHandle, localNewFileHandle, fileSize, localBlockSize, localOverlappedIOSize);
    }

    // Checking handle and closing the file
    if (localOldFileHandle != NULL && localOldFileHandle != INVALID_HANDLE_VALUE) // old file checking
    {
        if (CloseHandle(localOldFileHandle) == false)
        {
            cout << "EROOR WHILE CLOSING FILE \"" << localOldFileHandle << "\". " << endl;
        }
    }
    if (localNewFileHandle != NULL && localNewFileHandle != INVALID_HANDLE_VALUE) // new file checking
    {
        if (CloseHandle(localNewFileHandle) == false)
        {
            cout << "ERROR WHILE CLOSING FILE \"" << localNewFileHandle << "\". " << endl;
        }
    }

    return localActionTime;
}

// ---------- COPY AND PASTE ACTIONS DIRECTLY ----------

DWORD LocalCopyPaste (HANDLE in, HANDLE out, unsigned long long fileSize, unsigned long long bs, unsigned long long localOverlappedIOSize)
{
    DWORD localActionTime = -1;
    unsigned long long offset_i = 0;
    OVERLAPPED* over = NULL;
    char** localBuffer = NULL;
    OVERLAPPED* overLeft = NULL;
    char* localBufferLeft = NULL;
    unsigned long long bsLeft = 0;
    int leftelse = 0;

    /*if (fileSize <= 0)
    {
        return localActionTime;
    }*/

    over = new OVERLAPPED[localOverlappedIOSize];
    localBuffer = new char*[localOverlappedIOSize];
    for (unsigned long long i = 0; i < localOverlappedIOSize; ++i)
    {
        localBuffer[i] = new char[bs];
    }

    overLeft = (OVERLAPPED*)malloc(sizeof(OVERLAPPED));
    localBufferLeft = (char*)malloc(sizeof(char)*bs);

    globalFirstAdress = (unsigned long long)(&over[0]);
    oneSize = sizeof(OVERLAPPED);
    callLeft = (unsigned long long)overLeft;

    //localActionTime = timeGetTime();
    // ETO NUZHNO ISPRAVIT'
    localActionTime = 0;

    do
    {
        if(bs*localOverlappedIOSize <= fileSize)
        {
            for (unsigned long long i = 0; i < localOverlappedIOSize; ++i)
            {
                ULL2DWORDS(offset_i, &over[i].Offset, &over[i].OffsetHigh);
                offset_i += bs;
            }
        }
        else
        {
            localOverlappedIOSize = fileSize / bs;
            leftelse = (fileSize%bs == 0?0:1);
            bsLeft = LocalDriveSectorSize();
            bsLeft = ( (fileSize%bs)/bsLeft + 1) * bsLeft;

            for (unsigned long long i = 0; i < localOverlappedIOSize; ++i)
            {
                ULL2DWORDS(offset_i, &over[i].Offset, &over[i].OffsetHigh);
                offset_i += bs;
            }
            if(leftelse)
            {
                cout << "Last size is " << bsLeft << ", addr=" << callLeft << ", localOverlappedIOSize = " << localOverlappedIOSize << ". " << endl;
                ULL2DWORDS(offset_i, &(overLeft->Offset), &(overLeft->OffsetHigh));
                offset_i += bsLeft;
            }
        }

        callback = 0;
        for(unsigned long long i = 0; i < localOverlappedIOSize; ++i)
        {
            ReadFileEx(in, localBuffer[i], bs, &over[i], CompletionRoutine);
        }
        if(leftelse)
        {
            ReadFileEx(in, localBufferLeft, bsLeft, overLeft, CompletionRoutine);
        }
        while (callback < localOverlappedIOSize + leftelse)
        {
            SleepEx(-1, TRUE);
        }

        callback = 0;
        for(unsigned long long i = 0; i < localOverlappedIOSize; ++i)
        {
            WriteFileEx(out, localBuffer[i], bs, &over[i], CompletionRoutine);
        }
        if(leftelse)
        {
            WriteFileEx(out, localBufferLeft, bsLeft, overLeft, CompletionRoutine);
        }
        while (callback < localOverlappedIOSize + leftelse)
        {
            SleepEx(-1, TRUE);
        }
    }
    while (offset_i < fileSize - 1);
    //localActionTime = timeGetTime() - localActionTime;
    // ETO NUZHNO IPPRAVIT'
    localActionTime = 0;

    //fix
    LARGE_INTEGER fileSizeStruct;
    GetFileSizeEx(in, &fileSizeStruct);
    SetFilePointerEx(out, fileSizeStruct, NULL, FILE_BEGIN);
    SetEndOfFile(out);

    //CLEAN
    for (unsigned long long i = 0; i < localOverlappedIOSize; ++i)
    {
        delete localBuffer[i];
    }
    delete localBuffer;
    delete over;
    if (localBufferLeft)
    {
        free (localBufferLeft);
    }
    if (overLeft)
    {
        free (overLeft);
    }
    return localActionTime;
}

// ---------- GET DRIVE SECTOR SIZE ----------

DWORD LocalDriveSectorSize()
{
    DWORD localBytes = -1;
    DWORD localValue = -1;
    return localBytes = LocalDriveSectorSize(localValue);
}

// ---------- GET DRIVE SECTOR SIZE (MAIN OVERLOAD FUNCTION) ----------

DWORD LocalDriveSectorSize (DWORD &localSectorsPerCluster)
{
    DWORD localNumberOfClusters = -1;
    DWORD localBytesPerSector = -1;
    DWORD localNumberOfFreeClusters = -1;
    if (GetDiskFreeSpaceA(NULL, &localSectorsPerCluster, &localBytesPerSector, &localNumberOfFreeClusters, &localNumberOfClusters) == false)
    {
        cout << "\nERROR GETTING DRIVE SECTORS\n";
    }
    return localBytesPerSector;
}

// ---------- ??? ----------

unsigned long long DWORDS2ULL(DWORD l, DWORD h)
{
    unsigned long long res;
    res = l;
    res = res | ((unsigned long long)h << 32);
    return res;
}

// ---------- ??? ----------

void ULL2DWORDS(unsigned long long value, DWORD* l, DWORD* h)
{
    *l = value;
    *h = value >> 32;
}

// ---------- AFTER READING FILE NEED TO MAKE THIS FUNCTION ----------

// for read file ex
void CALLBACK CompletionRoutine (DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	callback = callback + 1;
}

// ---------- GET OVERLAPPED NUMBER ----------

/*unsigned long long getOverlappedNum (LPOVERLAPPED localOverlappedPointer)
{
    unsigned long long localNumber;
    unsigned long long localOverlappedAdress = (unsigned long long) localOverlappedPointer;
    if (localOverlappedAdress == callLeft)
    {
        return localNumber = 0;
    }
    else
    {
        return localNumber = (localOverlappedAdress - globalFirstAdress) / oneSize + 1;
    }
}*/