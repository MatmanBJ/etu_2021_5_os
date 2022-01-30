/*

Win32 API (WinAPI) is a set of functions in the library <windows.h>
API means "Application Programming Interface"

*/

#include <windows.h> // for WinAPI functions
#include <iostream> // just for working
#include <string> // for the "string" type using

using namespace std;

// ---------- MAIN ----------

int main (int argc, char* argv[])
{
	cout << "---------- READER APP ----------\n";

	int i; // loop variable
	int localEndingType = 1; // type choose for output

	// all possible types of types initializing

	LARGE_INTEGER localBytes; // default
	string mappingFileName = "localFileTwo";

	// all possible types of types choosing

	cout << "1 -- char:\t\t" << sizeof(char) << " bytes\n";
	cout << "2 -- short:\t\t" << sizeof(short) << " bytes\n";

	// input and output adress and type choosing

	do
	{
		cout << "Please, choose the output type: ";
		cin >> localEndingType;
	}
	while (localEndingType < 1 || localEndingType > 2);

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

	switch (localEndingType) // starting output type
	{
		case 1:
			localEndingType = sizeof(char);
			break;
		case 2:
			localEndingType = sizeof(short);
			break;
		default:
			localEndingType = sizeof(char);
			break;
	}

	bool localRepeat = true; // repeating input
	SIZE_T localStartingSize = 0; // memory size for output
	SIZE_T localEndingSize = 0; // memory size for output

	HANDLE mappingFileNew = OpenFileMapping(
		FILE_MAP_WRITE, // special desired access flag [FILE_MAP_WRITE is compatiable with PAGE_READWRITE]
		false, // inheritance mechanics -- unuseful here
		mappingFileName.c_str() // unique name of the mapping file
	);

	if (mappingFileNew == NULL)
	{
		cout << "Creating mapping file error. Last error number: " << GetLastError() << "\n";
	}

	// ACCESS GRANTED BY MAPPING FILENAME, SO IT MUST BE THE SAME
	LPVOID mappingNew = MapViewOfFile(
		mappingFileNew, // handle of the mapping file
		FILE_MAP_WRITE, // special desired access flag [FILE_MAP_WRITE is compatiable with PAGE_READWRITE]
		0, // OffsetHigh
		0, // OffsetLow
		0 // bytes to map [0 means all]
	);

	if (mappingNew == NULL)
	{
		cout << "Mapping view of file error. Last error number: " << GetLastError() << "\n";
	}

	cout << "Address: " << mappingNew << "\n";
	//MEMORY_BASIC_INFORMATION localMBI = {0};
	//VirtualQueryEx(GetCurrentProcess(), mappingNew, &localMBI, sizeof(localMBI));
	//out << "TRY IT: " << localMBI.RegionSize;

	// output loop
	// IT COVERS ALL MEMORY CLUSTERS FOR CHAR AND SHORT -- I CHECKED
	for (i = 0; i + 1 < localBytes.QuadPart; i = i + 2)
	{
		if (localEndingType == 2)
		{
			cout << i << " - " << i + 1 << ": ";
			cout << *(short*)(mappingNew + i) << "\n";
		}
		else
		{
			cout << i << " - " << i + 1 << ": ";
			cout << *(char*)(mappingNew + i) << *(char*)(mappingNew + i + 0x1) << "\n";
		}
	}

	system("pause");

	return 0;
}