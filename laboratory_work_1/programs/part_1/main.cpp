#include <windows.h> // i just needed this thing to read "GetDriveType" function
//#include "fileapi.h" // first of all i thought, that i need this, but i don't need that header file

/*

Win32 API (WinAPI) is a set of functions in the library <windows.h>
API means "Application Programming Interface"

*/

#include <iostream>

using namespace std;

void MainMenu ()
{
	cout << "Please, choose the menu item:" << "\n" << "1 -- Show all avaliable disk drives" << "\n" << "2 -- Show the drive type" << "\n";
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
	//using namespace uint;

	int d;
	
	//d = GetDriveType("c:\\"); // i just want to know what's going one if i choose d or f!
	//cout << "c:\\ is";

	d = GetDriveType("d:\\"); // i just want to know what's going one if i choose d or f!
	cout << "d:\\ is";

	//d = GetDriveType("f:\\"); // i just want to know what's going one if i choose d or f!
	//cout << "f:\\ is";

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
	/*if (d == LMEM_FIXED)
	{
		cout << " FIXED" << endl;
	}
	if (d == FILE_CREATE)
	{
		cout << " REMOTE" << endl;
	}*/
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

int main()
{
	//cout << "Test!";

	// "GET CURRENT DIRECTORY", "SET CURRENT DIRECTORY"

	int flag = 0;

	do
	{
		MainMenu();
		cin >> flag;
		switch (flag)
		{
			case 1:
				MyGetLogicalDrives();
				break;
			case 2:
				MyGetDriveType();
				break;
			default:
				break;
		}
	}
	while (flag != 0);

	/*uint :: int d;
	
	uint :: d = GetDriveType("c:\\");
	if (uint :: d == uint :: DRIVE_UNKNOWN)
	{
		cout << " UNKNOWN" << endl;
	}
	if (uint :: d == uint :: DRIVE_NO_ROOT_DIR)
	{
		cout << " DRIVE NO ROOT DIR" << endl;
	}
	if (uint :: d == uint :: DRIVE_REMOVABLE)
	{
		cout << " REMOVABLE" << endl;
	}
	if (uint :: d == uint :: DRIVE_FIXED)
	{
		cout << " FIXED" << endl;
	}
	if (uint :: d == uint :: DRIVE_REMOTE)
	{
		cout << " REMOTE" << endl;
	}
	if (uint :: d == uint :: DRIVE_CDROM)
	{
		cout << " CDROM" << endl;
	}
	if (uint :: d == uint :: DRIVE_RAMDISK)
	{
		cout << " RAMDISK" << endl;
	}*/

	return 0;
}