/*

Win32 API (WinAPI) is a set of functions in the library <windows.h>
API means "Application Programming Interface"

*/

#include <windows.h> // for WinAPI functions
#include <iostream> // just for working
#include <ctime> // for randomization
#include <string> // for the "string" type using

using namespace std;

// ---------- MAIN ----------

int main (int argc, char* argv[])
{
	cout << "---------- WRITER APP ----------\n";
	
	int i; // loop variable
	int localEndingType = 1; // type choose for output

	// all possible types of types choosing

	cout << "1 -- char:\t\t" << sizeof(char) << " bytes\n";
	cout << "2 -- short:\t\t" << sizeof(short) << " bytes\n";

	int localStartingType = 1; // type choose for input
	LARGE_INTEGER localBytes; // default
	string createFileName = "localFileOne";
	string mappingFileName = "localFileTwo";

	// input and output adress and type choosing

	do
	{
		cout << "Please, choose the output type: ";
		cin >> localStartingType;
	}
	while (localStartingType < 1 || localStartingType > 2);

	do
	{
		cout << "Please, enter the original file name (255 characters max): ";
		cin >> createFileName;
	}
	while (createFileName.length() > 255);

	do
	{
		cout << "Please, enter the mapping file name (255 characters max): ";
		cin >> mappingFileName;
	}
	while (mappingFileName.length() > 255);

	cout << "Please, choose the number of bytes of mapping object (low part): ";
	cin >> localBytes.LowPart;
	cout << "Please, choose the number of bytes of mapping object (high part, maybe 0): ";
	cin >> localBytes.HighPart;

	switch (localStartingType) // starting output type
	{
		case 1:
			localStartingType = sizeof(char);
			break;
		case 2:
			localStartingType = sizeof(short);
			break;
		default:
			localStartingType = sizeof(char);
			break;
	}

	srand((unsigned)time(0));

	HANDLE createdFile = CreateFile(
		createFileName.c_str(), // filename
		GENERIC_READ | GENERIC_WRITE, // desired access [all usage]
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, // share mode [all usage]
		NULL, // security attributes [NULL has been chosen because of current unusability of the other ones]
		CREATE_NEW, // creating/open files [CREATE_NEW is safety for other files]
		FILE_ATTRIBUTE_NORMAL, // flags and attributes [normal mode has been chosen because of current unusabilty of the other ones]
		NULL // templete file handle [NULL has been chosen because of current unusability of the other ones]
	);

	if (createdFile == INVALID_HANDLE_VALUE)
	{
		cout << "Creating file error. Last error number: " << GetLastError() << "\n";
	}

	HANDLE mappingFileOld = CreateFileMapping(
		createdFile, // handle of the created file [choosed the new one]
		NULL, // mapping attributes [NULL has been chosen because of current unusability of the other ones]
		PAGE_READWRITE, // page protection type [PAGE_READWRITE has been chosen because it' most convinient]
		localBytes.HighPart, // high order DWORD (second part)
		localBytes.LowPart, // low order DWORD (first part) [0 if the low-part means mapping file size = created file size]
		mappingFileName.c_str() // mapping filename [filename]
	);

	if (mappingFileOld == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		cout << "Creating mapping file error. Last error number: " << GetLastError() << "\n";
	}

	LPVOID mappingOld = MapViewOfFile(
		mappingFileOld, // handle of the mapping file
		FILE_MAP_WRITE, // special desired access flag [FILE_MAP_WRITE is compatiable with PAGE_READWRITE]
		0, // OffsetHigh
		0, // OffsetLow
		0 // bytes to map [0 means all]
	);

	if (mappingOld == NULL)
	{
		cout << "Mapping view of file error. Last error number: " << GetLastError() << "\n";
	}

	cout << "Address: " << mappingOld << "\n";

	// randomization and output loop
	for (i = 0; i + 1 < localBytes.QuadPart; i = i + 2)
	{
		*(short*)(mappingOld + i) = (rand() % 6)*2 + (rand() % 855);
		if (localStartingType == 2)
		{
			cout << i << " - " << i + 1 << ": ";
			cout << *(short*)(mappingOld + i) << "\n";
		}
		else
		{
			cout << i << " - " << i + 1 << ": ";
			cout << *(char*)(mappingOld + i) << *(char*)(mappingOld + i + 0x1) << "\n";
		}
	}

	UnmapViewOfFile(mappingOld); // unmapping current file
	CloseHandle(createdFile); // closing handle of original file, NOT THE MAPPING FILE, DON'T DO THAT
	//CloseHandle(mappingFileOld);

	system("pause");

	return 0;
}