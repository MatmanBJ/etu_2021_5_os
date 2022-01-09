#include <windows.h>
#include <string>
#include <iostream>
#include <cstdlib>

#include <ctime>
#include <fstream>

#include "./includes/cp.h"
#include "./utils/hash/Imd5.h"
#include "./utils/utils.h"

using namespace std;

void CopyPaste (string path, string target);
DWORD PreparingCopyPaste(string path, string target, unsigned long long bs, unsigned long long thNum);
DWORD LocalCopyPaste (HANDLE in, HANDLE out, unsigned long long fileSize, unsigned long long bs, unsigned long long thNum);

//Certutil -hashfile file

//===============

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

int LocalFileGenerator (string path, size_t bytes)
{
    ofstream fs (path, ios::out | ios::binary | ios::app);
    srand (time(NULL));
    unsigned long bytesNum = bytes;
    char rndByte[1];
    for (unsigned long i = 0; i < bytesNum; ++i)
    {
        rndByte[0] = (unsigned char)(rand() % 256);
        fs.write (rndByte, sizeof(rndByte));
    }
    fs.close();
    return 0;
}

//===============

// ---------- COPY AND PASTE MAIN PROCESS ----------

void CopyPaste (string path, string target)
{
    DWORD time;
    DWORD sectorsPerCluster;
    DWORD localSectorSize = getDriveSectorSize (sectorsPerCluster);
    unsigned long long localBlockSize; // size of the data block I will copy
    unsigned long long localOverlappedIOSize; // number of operations for Overlapped I/O

    cout << "Drive sector size: " << localSectorSize << " bytes. " << endl;
    cout << "Drive cluster size: " << sectorsPerCluster << " sectors (" << localSectorSize*sectorsPerCluster << " bytes). " << endl << endl;

    // THE EXPERIMENT PART I

    //=====localOverlappedIOSize = 1, localBlockSize = x, time = y=====
    const size_t n_bsXY = bs_e - bs_b + 1;
    unsigned bsX[n_bsXY];
    unsigned bsY[n_bsXY];
    size_t i_bsXY = 0;
    i_bsXY++; // useless action for usage, actually
    --i_bsXY; // useless action for usage, actually
    
    localOverlappedIOSize = 1;
    for (unsigned i = bs_b, i_bsXY = 0; i <= bs_e; ++i, ++i_bsXY)
    {
        localBlockSize = localSectorSize*i;
        time = PreparingCopyPaste(path, target, localBlockSize, 1);
        cout << "localBlockSize = " << localBlockSize << " (" << i << "*" << localSectorSize << "), localOverlappedIOSize = " << localOverlappedIOSize << ", time = " << time << ". Hash of result file = \"" << "\". " << endl;
        bsX[i_bsXY] = i;
        bsY[i_bsXY] = time;
    }

    // THE EXPERIMET PART II

    //=====localBlockSize = bs_std, localOverlappedIOSize = x, time = y=====
    const size_t n_thXY = thNum_e - thNum_b + 1;
    unsigned thX[n_bsXY];
    unsigned thY[n_bsXY];
    size_t i_thXY = 0;
    i_thXY++; // useless action for usage, actually
    --i_thXY; // useless action for usage, actually

    localBlockSize = bs_std*localSectorSize; // block size (block size we will copy) = blocks number (how many?) * sector size (on disk)
    for (unsigned i = thNum_b, i_thXY = 0; i <= thNum_e; ++i, ++i_thXY)
    {
        localOverlappedIOSize = i;
        time = PreparingCopyPaste(path, target, localBlockSize, localOverlappedIOSize);
        cout << "localBlockSize = " << localBlockSize << " (" << bs_std << "*" << localSectorSize << "), localOverlappedIOSize = " << localOverlappedIOSize << ", time = " << time << ". Hash of result file = \"" << "\". " << endl;
        thX[i_thXY] = localOverlappedIOSize;
        thY[i_thXY] = time;
    }
}

// ---------- PREPARING FOR COPY AND PASTE ACTIONS ----------

DWORD PreparingCopyPaste(string path, string target, unsigned long long bs, unsigned long long localOverlappedIOSize)
{
    DWORD localActionTime = -1;

    HANDLE src = openSrc(path);
    HANDLE dest = openDest(target);
    WINBOOL closeSuccess;

    if (src == NULL || src == INVALID_HANDLE_VALUE || dest == NULL || dest == INVALID_HANDLE_VALUE)
    {
        cout << "Problem with opening files. " << endl;
        if (GetLastError())
        {
            cout << endl << "\tERROR: " << GetLastError() << endl;
        }
    }
    else
    {
        DWORD hSize;
        DWORD lSize = GetFileSize(src, &hSize);
        unsigned long long fileSize = DWORDS2ULL(lSize, hSize);

        // DEBUG
        ////fileSize = fileSize | ((unsigned long long)hSize << 32);
        //cout << "Size of the file \"" << path << "\" is " << fileSize << ". " << endl;
        //DWORD fi, si;
        //ULL2DWORDS(fileSize, &fi, &si);
        //cout << "Check: " << fi << " == " << lSize << ". " << endl;
        //cout << "Check: " << si << " == " << hSize << ". " << endl;

        localActionTime = LocalCopyPaste(src, dest, fileSize, bs, localOverlappedIOSize);
    }

    if( !(src == NULL || src == INVALID_HANDLE_VALUE) )
    {
        closeSuccess = CloseHandle(src);
        if(closeSuccess)
        {
            // DEBUG
            // cout << endl << "File \"" << src << "\" closed successfully. " << endl;
            closeSuccess = 1;
        }
        else
        {
            cout << "Problem with closing file \"" << src << "\". " << endl;
        }
    }
    if( !(dest == NULL || dest == INVALID_HANDLE_VALUE) )
    {
        closeSuccess = CloseHandle(dest);
        if(closeSuccess)
        {
            // DEBUG
            //cout << endl << "File \"" << dest << "\" closed successfully. " << endl;
            closeSuccess = 1;
        }
        else
        {
            cout << "Problem with closing file \"" << dest << "\". " << endl;
        }
    }
    return localActionTime;
}

unsigned long long callback;
unsigned long long firstAddr;
unsigned long long oneSize;
unsigned long long callLeft;

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

    if (fileSize <= 0)
    {
        return localActionTime;
    }

    over = new OVERLAPPED[localOverlappedIOSize];
    localBuffer = new char*[localOverlappedIOSize];
    for (unsigned long long i = 0; i < localOverlappedIOSize; ++i)
    {
        localBuffer[i] = new char[bs];
    }

    overLeft = (OVERLAPPED*)malloc(sizeof(OVERLAPPED));
    localBufferLeft = (char*)malloc(sizeof(char)*bs);

    firstAddr = (unsigned long long)(&over[0]);
    oneSize = sizeof(OVERLAPPED);
    callLeft = (unsigned long long)overLeft;

    // DEBUG
    //int gi = 0;

    //localActionTime = timeGetTime();
    // ETO NUZHNO ISPRAVIT'
    localActionTime = 0;

    do
    {
        // DEBUG
        //cout << "Iter " << gi++ << ": " << endl;

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
            bsLeft = getDriveSectorSize();
            bsLeft = ( (fileSize%bs)/bsLeft + 1) * bsLeft;
            //bsLeft = fileSize%bs;

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
            ReadFileEx(in, localBuffer[i], bs, &over[i], FileIOCompletionRoutineIN);
        }
        if(leftelse)
        {
            ReadFileEx(in, localBufferLeft, bsLeft, overLeft, FileIOCompletionRoutineIN);
        }
        while (callback < localOverlappedIOSize + leftelse)
        {
            SleepEx(-1, TRUE);
        }

        callback = 0;
        for(unsigned long long i = 0; i < localOverlappedIOSize; ++i)
        {
            WriteFileEx(out, localBuffer[i], bs, &over[i], FileIOCompletionRoutineOUT);
        }
        if(leftelse)
        {
            WriteFileEx(out, localBufferLeft, bsLeft, overLeft, FileIOCompletionRoutineOUT);
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

DWORD getDriveSectorSize()
{
    DWORD localBuffer = -1;
    DWORD bytesPerSector = -1;
    bytesPerSector = getDriveSectorSize(localBuffer);
    return bytesPerSector;
}

// ---------- GET DRIVE SECTOR SIZE OVERLOAD ----------

DWORD getDriveSectorSize (DWORD &sectorsPerCluster)
{
    DWORD totalNumberOfClusters = -1;
    //DWORD sectorsPerCluster = -1;
    DWORD bytesPerSector = -1;
    DWORD numberOfFreeClusters = -1;
    WINBOOL getSpaceSuccess = GetDiskFreeSpaceA(NULL, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);
    if(!getSpaceSuccess)
    {
        cout << "\t!!! An error occurred while getting information about the disk space !!!" << endl;
    }
    return bytesPerSector;
}

unsigned long long DWORDS2ULL(DWORD l, DWORD h)
{
    unsigned long long res;
    res = l;
    res = res | ((unsigned long long)h << 32);
    return res;
}

void ULL2DWORDS(unsigned long long value, DWORD* l, DWORD* h)
{
    *l = value;
    *h = value >> 32;
}

void CALLBACK FileIOCompletionRoutineIN(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    // DEBUG
    //cout << "Read " << getOverlappedNum(lpOverlapped) << " done. " << endl;
    ++callback;
}

void CALLBACK FileIOCompletionRoutineOUT(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    // DEBUG
    //cout << "Write " << getOverlappedNum(lpOverlapped) << " done. " << endl;
    ++callback;
}

unsigned long long getOverlappedNum(LPOVERLAPPED lpOverlapped)
{
    unsigned long long res;
    unsigned long long overAddr = (unsigned long long)lpOverlapped;
    if (overAddr == callLeft)
    {
        res = 0;
    }
    else
    {
        res = (overAddr - firstAddr) / oneSize + 1;
    }
    return res;
}

inline HANDLE openSrc(string path)
{
    HANDLE res = NULL;
    res = CreateFileA(path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    return res;
}

inline HANDLE openDest(string path)
{
    HANDLE res = NULL;
    res = CreateFileA(path.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    return res;
}

// ---------- MAIN ----------

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cout << "Syntax error. " << std::endl;
        return -1;
    }
    else
    {
        const string oldFilePath(argv[1]);
        const string newFilePath(argv[3]);
        const size_t bytes_n = atoi(argv[2]);
        
        std::cout << "Generating file with random bytes (length = " << bytes_n << "): \"" << oldFilePath << "\"... " << std::endl;
        LocalFileGenerator (oldFilePath, bytes_n);
        std::cout << "Generating done. " << std::endl;
        CopyPaste(oldFilePath, newFilePath);

        return 0;
    }
}