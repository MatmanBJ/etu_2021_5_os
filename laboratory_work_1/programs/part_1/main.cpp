/*

Saint Petersburg Electrotechnical University "LETI" (ETU "LETI"),
Faculty of Computer Science and Technology "FKTI",
Department of Computer Science and Engineering,
Computer Systems Engineering and Informatics (09.03.01) program.

OS labortory work 1

Copyight (c) 2021 Sobolev Matvey Sergeevich

This software is under MIT License (X11 License).
You can see a detailed description in "LICENSE.md" file.

*/

/*

Win32 API (WinAPI) is a set of functions in the library <windows.h>
API means "Application Programming Interface"

*/

#include <windows.h> // for WinAPI functions
#include <iostream> // just for working
#include <string> // for "string" type using

using namespace std;

/*void DiskInfoMenu ()
{
	cout << "Please, choose the disk info menu item:" << "\n" << "1 -- Show all avaliable disk drives" << "\n" << "2 -- Show the drive type" << "\n";
}*/

string GetDiskName();
void MainMenu ();
void MyGetLogicalDrives ();
void MyGetDriveType ();
void MyGetVolumeInformation ();

int main ()
{
	// "GET CURRENT DIRECTORY", "SET CURRENT DIRECTORY"

	int flag = 0;

	do
	{
		MainMenu();
		cin >> flag;
		cout << "\n";
		switch (flag)
		{
			case 0:
				cout << "Goodbye!";
				break;
			case 1:
				MyGetLogicalDrives();
				break;
			case 2:
				MyGetDriveType();
				break;
			case 3:
				MyGetVolumeInformation();
				break;
			default:
				cout << "Incorrect input! Try again.";
				break;
		}
	}
	while (flag != 0);

	return 0;
}

string GetDiskName ()
{
	string localDisk;

	cout << "Please, input disk name you want:\n";
	cin >> localDisk;
	localDisk = localDisk + ":\\";

	return localDisk;
}

void MainMenu ()
{
	cout << "\n";
	cout << "Please, choose the menu item:\n";
	cout << "0 -- Quit\n";
	cout << "1 -- Show all avaliable disk drives\n";
	cout << "2 -- Show the drive type\n";
	cout << "3 -- Show the volume information\n";
	cout << "\n";
}

void MyGetLogicalDrives()
{
	int n;
	char dd[4];
	DWORD dr = GetLogicalDrives();

	for(int i = 0; i < 26; i++)
	{
		n = ((dr>>i) & 0x00000001);
		if(n == 1)
		{
			dd[0] =  char(65+i);
			dd[1] = ':';
			dd[2] = '\\';
			dd[3] = 0;
			cout << "Available disk drives : " << dd << endl;
		}
	}
}

void MyGetDriveType()
{
	// i need to check, if i use the "uint" namespace

	int d;
	string n;

	n = GetDiskName();

	// <string variable>.c_str() means that you convert to <const char * type>, because "" isn't <const char *> type

	d = GetDriveType(n.c_str()); // i just want to know what's going one if i choose d or f!
	cout << n << " is";
	//cout << n + " is";

	if (d == DRIVE_UNKNOWN)
	{
		cout << " UNKNOWN" << endl;
	}
	if (d == DRIVE_NO_ROOT_DIR)
	{
		cout << " DRIVE NO ROOT DIR" << endl;
	}
	if (d == DRIVE_REMOVABLE)
	{
		cout << " REMOVABLE" << endl;
	}
	if (d == DRIVE_FIXED)
	{
		cout << " FIXED" << endl;
	}
	if (d == DRIVE_REMOTE)
	{
		cout << " REMOTE" << endl;
	}
	if (d == DRIVE_CDROM)
	{
		cout << " CDROM" << endl;
	}
	if (d == DRIVE_RAMDISK)
	{
		cout << " RAMDISK" << endl;
	}
}

void MyGetVolumeInformation ()
{
	char VolumeNameBuffer[100]; 
	char FileSystemNameBuffer[100];
	string n;
	unsigned long VolumeSerialNumber;

	n = GetDiskName();

	BOOL GetVolumeInformationFlag = GetVolumeInformationA(
		n.c_str(),
		VolumeNameBuffer,
		100,
		&VolumeSerialNumber,
		NULL, //&MaximumComponentLength,
		NULL, //&FileSystemFlags,
		FileSystemNameBuffer,
		100
	);

	/*BOOL GetVolumeInformationFlag = GetVolumeInformationA(
		"d:\\",
		VolumeNameBuffer,
		100,
		&VolumeSerialNumber,
		NULL, //&MaximumComponentLength,
		NULL, //&FileSystemFlags,
		FileSystemNameBuffer,
		100
	);*/

	if (GetVolumeInformationFlag != 0) 
	{
		cout << " Volume Name is " << VolumeNameBuffer << endl;
		cout << " Volume Serial Number is " << VolumeSerialNumber << endl;
		cout << " File System is " << FileSystemNameBuffer << endl;
	}
	else cout << " Not Present (GetVolumeInformation)" << endl;
}