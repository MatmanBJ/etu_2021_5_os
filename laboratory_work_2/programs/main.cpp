/*

Saint Petersburg Electrotechnical University "LETI" (ETU "LETI"),
Faculty of Computer Science and Technology "FKTI",
Department of Computer Science and Engineering,
Computer Systems Engineering and Informatics (09.03.01) program.

OS labortory work 1 version 0_8 dated 2021_09_23

This software is under MIT License (X11 License).

Copyight (c) 2021 Sobolev Matvey Sergeevich a.k.a. MatmanBJ

*/

/*

Win32 API (WinAPI) is a set of functions in the library <windows.h>
API means "Application Programming Interface"

*/

#include <windows.h> // for WinAPI functions
#include <bitset> // for binary output
#include <math.h> // for double making
#include <exception> // for exceptions
#include <iostream> // just for working
#include <string> // for the "string" type using
#include <vector> // for the "vector" type using
#include <algorithm> // for the "find" function using

using namespace std;

string currentPath = "c:\\"; // the current working path is disc "c:/" by default!

// ---------- FUNCTION DECLARATION ----------

void LocalGetSystemInfo ();
void LocalGlobalMemoryStatus ();
void LocalGlobalMemoryStatusEx ();
void LocalVirtualQuery ();
void LocalVirtualAlloc ();

string GetDiskName ();
void MainMenu ();
void Info ();
void LocalGetLogicalDrives ();
void LocalGetDriveType ();
void LocalGetVolumeInformation ();
void LocalGetDiskFreeSpaceEx ();
void LocalGetDiskFreeSpace ();
void GetDiskInfo();
bool dirExists(const std::string& dirName_in);
string GetPathKernel (char localFlag);
string GetPathShell (char localFlagOne, char localFlagTwo, string localMessageOne, string localMessageTwo);
void LocalCreateRemoveDirectory (char actionCreateRemove);
void LocalCreateFile();
void LocalCopyMoveFile(char actionCopyMove);
void LocalGetFileAttributes ();
void LocalSetFileAttributes ();
void LocalGetFileInformationByHandle ();
void GetFileTime ();
void SetFileTime ();

// ---------- MAIN ----------

int main (int argc, char* argv[]) // i've finally understood what it means (argc -- number of arguments, argv -- strings of arguments (including -<word> and --<word>))
{
	// "GET CURRENT DIRECTORY", "SET CURRENT DIRECTORY"

	int flag = -1; // "-1" for incorrect input continue the program

	do
	{
		MainMenu();
		cin >> flag;
		cout << "\n";
		switch (flag)
		{
			case 101:
				LocalGetSystemInfo();
				break;
			case 102:
				LocalGlobalMemoryStatus();
				break;
			case 103:
				LocalGlobalMemoryStatusEx();
				break;
			case 104:
				LocalVirtualQuery();
				break;
			case 105:
				LocalVirtualAlloc();
				break;
			case 0:
				cout << "Goodbye!";
				break;
			case 1:
				cout << "Your current working path is: \"" << currentPath << "\" (c:\\ by default).\n";
				break;
			case 2:
				currentPath = GetPathKernel('c');
				break;
			case 3:
				Info();
				break;
			case 11:
				LocalGetLogicalDrives();
				break;
			case 21:
				LocalGetDriveType();
				break;
			case 22:
				LocalGetVolumeInformation();
				break;
			case 23:
				LocalGetDiskFreeSpace();
				break;
			case 24:
				GetDiskInfo();
				break;
			case 231:
				LocalGetDiskFreeSpaceEx();
				break;
			case 31:
				LocalCreateRemoveDirectory ('c');
				break;
			case 32:
				LocalCreateRemoveDirectory ('r');
				break;
			case 41:
				LocalCreateFile();
				break;
			case 51:
				LocalCopyMoveFile('c');
				break;
			case 52:
				LocalCopyMoveFile('m');
				break;
			case 53:
				LocalCopyMoveFile('e');
				break;
			case 61:
				LocalGetFileAttributes();
				break;
			case 62:
				LocalSetFileAttributes();
				break;
			case 63:
				LocalGetFileInformationByHandle();
				break;
			case 64:
				GetFileTime();
				break;
			case 65:
				SetFileTime();
				break;
			default:
				cout << "Incorrect input! Try again.";
				break;
		}
	}
	while (flag != 0);

	return 0;
}

// UTILITIES

string NumberBoolToWordBool (bool localBool, bool localRegister)
{
	string localWordBool;
	if (localBool == true && localRegister == false)
	{
		localWordBool = "true";
	}
	else if (localBool == true && localRegister == true)
	{
		localWordBool = "TRUE";
	}
	else if (localBool == false && localRegister == false)
	{
		localWordBool = "false";
	}
	else if (localBool == false && localRegister == true)
	{
		localWordBool = "FALSE";
	}
	else
	{
		localWordBool = "false";
	}
	return localWordBool;
}

// ---------- 0 -- GET DISK NAME ----------

string GetDiskName ()
{
	string localDisk;

	cout << "Please, input disk name you want:\n";
	cin >> localDisk;
	localDisk = localDisk + ":\\";

	return localDisk;
}

// ---------- 0 -- MAIN MENU ----------

void MainMenu ()
{
	cout << "101 -- F1\n";
	cout << "102 -- F2\n";
	cout << "103 -- F3\n";
	cout << "104 -- F4\n";
	cout << "105 -- F5\n";
	cout << "\n";
	cout << "Please, choose the menu item:\n";
	cout << "0 -- COMMON:\n";
	cout << "0 -- Quit\n";
	cout << "1 -- Output current directory\n";
	cout << "2 -- Change current working directory\n";
	cout << "3 -- Info\n";
	cout << "1 -- DRIVES LIST:\n";
	cout << "11 -- Show all avaliable disk drives\n";
	cout << "2 -- INFORMATION ABOUT DRIVES:\n";
	cout << "21 -- Show the drive type\n";
	cout << "22 -- Show the volume information\n";
	cout << "23 -- Show the disk free space\n";
	cout << "24 -- Show all disk information\n";
	cout << "3 -- DIRECTORIES:\n";
	cout << "31 -- Create new directory\n";
	cout << "32 -- Remove old directory\n";
	cout << "4 -- CREATE/OPEN FILES:\n";
	cout << "41 -- Create new file\n";
	cout << "5 -- COPY/MOVE FILES\n";
	cout << "51 -- Copy file\n";
	cout << "52 -- Move file\n";
	cout << "53 -- Move file (extended properties)\n";
	cout << "6 -- ATTRIBUTES\n";
	cout << "61 -- Get file attributes\n";
	cout << "62 -- Set file attributes\n";
	cout << "63 -- Get file attributes by handle\n";
	cout << "64 -- Get file time\n";
	cout << "65 -- Set file time\n";
	cout << "\n";
}

// ---------- 0 -- INFO ----------

void Info ()
{
	cout << "Saint Petersburg Electrotechnical University \"LETI\" (ETU \"LETI\"),\n"
	<< "Faculty of Computer Science and Technology \"FKTI\",\n"
	<< "Department of Computer Science and Engineering,\n"
	<< "Computer Systems Engineering and Informatics (09.03.01) program.\n\n"
	<< "OS labortory work 1 version 0_8 dated 2021_09_23\n\n"
	<< "This software is under MIT License (X11 License).\n"
	<< "You can see a detailed description in \"LICENSE.md\" file.\n\n"
	<< "Copyight (c) 2021 Sobolev Matvey Sergeevich\n";
}

// ---------- 1 -- LOCAL GET SYSTEM INFO ----------

/*
void GetSystemInfo(
  LPSYSTEM_INFO lpSystemInfo
);

typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

*/

void LocalGetSystemInfo()
{
	SYSTEM_INFO localSystemInfo; // creating the structure
	GetSystemInfo(&localSystemInfo); // sending the pointer and getting the information
	cout << "Hardware information:\n"; // information output

	// DWORD dwOemId output

	cout << "    OEM ID (obsolete member):                                                 " << localSystemInfo.dwOemId << "\n";

	// WORD wProcessorArchitecture output

	if (localSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) // number 9
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "x64 (AMD or Intel)\n";
	}
	else if (localSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM) // number 5
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "ARM\n";
	}
	else if (localSystemInfo.wProcessorArchitecture == 0x000c /*PROCESSOR_ARCHITECTURE_ARM64*/) // number 12; with "PROCESSOR_ARCHITECTURE_ARM64" it doesn't work
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "ARM64\n";
	}
	else if (localSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) // number 6
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "Intel Itanium-based\n";
	}
	else if (localSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) // number 0
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "x86\n";
	}
	else if (localSystemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN) // number 0xffff
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "Unknown architecture.\n";
	}
	else // other number
	{
		cout << "    Processor architecture of the installed OS:                               " << localSystemInfo.wProcessorArchitecture << " -- " << "THIS NUMBER DOESN'T MEAN ANYTHING\n";
	}

	// WORD wReserved output

	cout << "    This member is reserved for future use:                                   " << localSystemInfo.wReserved << "\n";

	// DWORD dwPageSize output

	cout << "    Page size and the granularity of page protection and commitment:          " << localSystemInfo.dwPageSize << "\n";

	// LPVOID lpMinimumApplicationAddress output

	cout << "    Lowest memory address accessible to applications and DLLs:                " << localSystemInfo.lpMinimumApplicationAddress << "\n";

	// LPVOID lpMaximumApplicationAddress output

	cout << "    Highest memory address accessible to applications and DLLs:               " << localSystemInfo.lpMaximumApplicationAddress << "\n";

	// DWORD_PTR dwActiveProcessorMask output

	cout << "    Mask -- set of processors configured into OS (bit 0 = processor 0, etc.): " << bitset<32>(localSystemInfo.dwActiveProcessorMask) << "\n";

	// DWORD dwNumberOfProcessors output

	cout << "    Logical processors in the current group:                                  " << localSystemInfo.dwNumberOfProcessors << "\n";

	// DWORD dwProcessorType output

	if (localSystemInfo.dwProcessorType == PROCESSOR_INTEL_386) // number 386
	{
		cout << "    Processor type (obsolete member):                                         " << localSystemInfo.dwProcessorType << " -- " << "PROCESSOR_INTEL_386\n";
	}
	else if (localSystemInfo.dwProcessorType == PROCESSOR_INTEL_486) // number 486
	{
		cout << "    Processor type (obsolete member):                                         " << localSystemInfo.dwProcessorType << " -- " << "PROCESSOR_INTEL_486\n";
	}
	else if (localSystemInfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM) // number 586; with "PROCESSOR_ARCHITECTURE_ARM64" it doesn't work
	{
		cout << "    Processor type (obsolete member):                                         " << localSystemInfo.dwProcessorType << " -- " << "PROCESSOR_INTEL_PENTIUM\n";
	}
	else if (localSystemInfo.dwProcessorType == PROCESSOR_INTEL_IA64) // number 2200
	{
		cout << "    Processor type (obsolete member):                                         " << localSystemInfo.dwProcessorType << " -- " << "PROCESSOR_INTEL_IA64\n";
	}
	else if (localSystemInfo.dwProcessorType == PROCESSOR_AMD_X8664) // number 8664
	{
		cout << "    Processor type (obsolete member):                                         " << localSystemInfo.dwProcessorType << " -- " << "PROCESSOR_AMD_X8664\n";
	}
	/*else if (localSystemInfo.dwProcessorType == PROCESSOR_ARM) // Reserved
	{
		cout << "    Processor type (obsolete member): " << localSystemInfo.dwProcessorType << " -- " << "PROCESSOR_ARM (Reserved)\n";
	}*/
	else // other number
	{
		cout << "    Processor type (obsolete member):                                         " << localSystemInfo.dwProcessorType << " -- " << "THIS NUMBER DOESN'T MEAN ANYTHING\n";
	}

	// DWORD dwAllocationGranularity output

	cout << "    Granularity of virtual memory aloocation adress:                          0x" << hex << localSystemInfo.dwAllocationGranularity << dec << "\n";

	// WORD wProcessorLevel output

	/*
	If wProcessorArchitecture is PROCESSOR_ARCHITECTURE_INTEL, wProcessorLevel is defined by the CPU vendor.
	If wProcessorArchitecture is PROCESSOR_ARCHITECTURE_IA64, wProcessorLevel is set to 1.
	*/

	cout << "    Architecture-dependent processor level:                                   " << localSystemInfo.wProcessorLevel << "\n";

	// Procaessor features output (it's not a part of the structure)

	cout << "    Processor features presentation:\n";

	cout << "        64-bit load/store atomic instructions are available:                            " << IsProcessorFeaturePresent(PF_ARM_64BIT_LOADSTORE_ATOMIC) << "\n"; // number 25
	cout << "        Divide instructions are available:                                              " << IsProcessorFeaturePresent(PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE) << "\n"; // number 24
	cout << "        External cache is available:                                                    " << IsProcessorFeaturePresent(PF_ARM_EXTERNAL_CACHE_AVAILABLE) << "\n"; // number 26
	cout << "        Floating-point multiply-accumulate instruction is available:                    " << IsProcessorFeaturePresent(PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE) << "\n"; // number 27
	cout << "        VFP/Neon: 32 x 64bit register bank is present:                                  " << IsProcessorFeaturePresent(PF_ARM_VFP_32_REGISTERS_AVAILABLE) << "\n"; // number 18
	//cout << "        VFP/Neon: 32 x 64bit register bank is present (other flag):                     " << IsProcessorFeaturePresent(PF_ARM_VFP_EXTENDED_REGISTERS) << "\n";
	cout << "        3D-Now instruction set is available:                                            " << IsProcessorFeaturePresent(PF_3DNOW_INSTRUCTIONS_AVAILABLE) << "\n"; // number 7

	cout << "        Processor channels are enabled:                                                 " << IsProcessorFeaturePresent(PF_CHANNELS_ENABLED) << "\n"; // number 16
	cout << "        Atomic compare and exchange operation (cmpxchg) is available:                   " << IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE_DOUBLE) << "\n"; // number 2
	cout << "        Atomic compare and exchange 128-bit operation (cmpxchg16b) is available:        " << IsProcessorFeaturePresent(PF_COMPARE_EXCHANGE128) << "\n"; // number 14
	cout << "        Atomic compare 64 and exchange 128-bit operation (cmp8xchg16) is available:     " << IsProcessorFeaturePresent(PF_COMPARE64_EXCHANGE128) << "\n"; // number 15

	cout << "        _fastfail() is available:                                                       " << IsProcessorFeaturePresent(PF_FASTFAIL_AVAILABLE) << "\n"; // number 23
	cout << "        Floating-point operations are emulated using a software emulator:               " << IsProcessorFeaturePresent(PF_FLOATING_POINT_EMULATED) << "\n"; // number 1
	cout << "        On a Pentium, a floating-point precision error can occur in rare circumstances: " << IsProcessorFeaturePresent(PF_FLOATING_POINT_PRECISION_ERRATA) << "\n"; // number 0
	cout << "        MMX instruction set is available:                                               " << IsProcessorFeaturePresent(PF_MMX_INSTRUCTIONS_AVAILABLE) << "\n"; // number 3

	cout << "        Data execution prevention is enabled:                                           " << IsProcessorFeaturePresent(PF_NX_ENABLED) << "\n"; // number 12
	cout << "        Processor is PAE-enabled:                                                       " << IsProcessorFeaturePresent(PF_PAE_ENABLED) << "\n"; // number 9
	cout << "        RDTSC instruction is available:                                                 " << IsProcessorFeaturePresent(PF_RDTSC_INSTRUCTION_AVAILABLE) << "\n"; // number 8
	cout << "        RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE instructions are available:          " << IsProcessorFeaturePresent(PF_RDWRFSGSBASE_AVAILABLE) << "\n"; // number 22

	cout << "        Second Level Address Translation is supported by the hardware:                  " << IsProcessorFeaturePresent(PF_SECOND_LEVEL_ADDRESS_TRANSLATION) << "\n"; // number 20
	cout << "        SSE3 instruction set is available:                                              " << IsProcessorFeaturePresent(PF_SSE3_INSTRUCTIONS_AVAILABLE) << "\n"; // number 13
	cout << "        Virtualization is enabled in the firmware and made available by the OS:         " << IsProcessorFeaturePresent(PF_VIRT_FIRMWARE_ENABLED) << "\n"; // number 21
	cout << "        SSE instruction set is available:                                               " << IsProcessorFeaturePresent(PF_XMMI_INSTRUCTIONS_AVAILABLE) << "\n"; // number 6

	cout << "        SSE2 instruction set is available:                                              " << IsProcessorFeaturePresent(PF_XMMI64_INSTRUCTIONS_AVAILABLE) << "\n"; // number 10
	cout << "        Processor implements the XSAVE and XRSTOR instructions:                         " << IsProcessorFeaturePresent(PF_XSAVE_ENABLED) << "\n"; // number 17
	//cout << "        ARM processor implements the the ARM v8 instructions set: " << IsProcessorFeaturePresent(PF_ARM_V8_INSTRUCTIONS_AVAILABLE) << "\n";
	//cout << "        ARM processor implements the ARM v8 extra cryptographic instructions (i.e. AES, SHA1 and SHA2): " << IsProcessorFeaturePresent(PF_ARM_V8_CRYPTO_INSTRUCTIONS_AVAILABLE) << "\n";
	//cout << "        ARM processor implements the ARM v8 extra CRC32 instructions: " << IsProcessorFeaturePresent(PF_ARM_V8_CRC32_INSTRUCTIONS_AVAILABLE) << "\n";
	//cout << "        ARM processor implements the ARM v8.1 atomic instructions (e.g. CAS, SWP): " << IsProcessorFeaturePresent(PF_ARM_V81_ATOMIC_INSTRUCTIONS_AVAILABLE) << "\n";
	cout << "        ARM processor implements ARM v8 instructions set:                               " << IsProcessorFeaturePresent(29) << "\n"; // [crutch]
	cout << "        ARM processor implements ARM v8 extra crypto instr-s (i.e. AES, SHA1, SHA2):    " << IsProcessorFeaturePresent(30) << "\n"; // [crutch]
	cout << "        ARM processor implements ARM v8 extra CRC32 instructions:                       " << IsProcessorFeaturePresent(31) << "\n"; // [crutch]
	cout << "        ARM processor implements ARM v8.1 atomic instructions (e.g. CAS, SWP):          " << IsProcessorFeaturePresent(34) << "\n"; // [crutch]

	// WORD wProcessorRevision output

	/*
	Intel Pentium, Cyrix, or NextGen 586:
	The high byte is the model and the low byte is the stepping. For example, if the value is xxyy, the model number and stepping can be displayed as follows:
	Model xx, Stepping yy

	Intel 80386 or 80486:
	A value of the form xxyz.
	If xx is equal to 0xFF, y - 0xA is the model number, and z is the stepping identifier.
	If xx is not equal to 0xFF, xx + 'A' is the stepping letter and yz is the minor stepping.

	ARM:
	Reserved.
	*/

	cout << "    Architecture-dependent processor revision:                                0x" << hex << localSystemInfo.wProcessorRevision << dec << "\n";

	/*if (localSystemInfo.dwProcessorType == PROCESSOR_INTEL_386 || localSystemInfo.dwProcessorType == PROCESSOR_INTEL_486)
	{
		if ((localSystemInfo.wProcessorRevision / 256) == 0xff)
		{
			cout << "        Model number: " << (localSystemInfo.wProcessorRevision % 256) - (localSystemInfo.wProcessorRevision % 16) - 0xa << "\n";
			cout << "        Stepping identifier: " << (localSystemInfo.wProcessorRevision % 16) << "\n";
		}
		else
		{
			cout << "        Stepping letter: " << (localSystemInfo.wProcessorRevision / 256) + 'A' << "\n";
			cout << "        Minor stepping: " << (localSystemInfo.wProcessorRevision % 256) + 'A' << "\n";
		}
	}
	else if (localSystemInfo.dwProcessorType == PROCESSOR_INTEL_PENTIUM || localSystemInfo.dwProcessorType == PROCESSOR_INTEL_IA64 || localSystemInfo.dwProcessorType == PROCESSOR_AMD_X8664)
	{
		cout << "        Model: " << (localSystemInfo.wProcessorRevision / 256) << "\n";
		cout << "        Stepping: " << (localSystemInfo.wProcessorRevision % 256) << "\n";
	}
	else
	{}*/
}

// ---------- 2 -- LOCAL GLOBAL MEMORY STATUS ----------

/*
void GlobalMemoryStatus(
  LPMEMORYSTATUS lpBuffer
);

typedef struct _MEMORYSTATUS {
  DWORD  dwLength;
  DWORD  dwMemoryLoad;
  SIZE_T dwTotalPhys;
  SIZE_T dwAvailPhys;
  SIZE_T dwTotalPageFile;
  SIZE_T dwAvailPageFile;
  SIZE_T dwTotalVirtual;
  SIZE_T dwAvailVirtual;
} MEMORYSTATUS, *LPMEMORYSTATUS;
*/

void LocalGlobalMemoryStatus ()
{
	MEMORYSTATUS localMemoryStatus; // creating structure
	GlobalMemoryStatus(&localMemoryStatus); // sending the pointer and getting the information
	cout << "Physical memory (RAM) information:\n"; // information output

	// DWORD dwLength output

	cout << "    MEMORYSTATUS structure size (in bytes): " << localMemoryStatus.dwLength << "\n";

	// DWORD dwMemoryLoad output

	cout << "    Approximate physical memory use (in %): " << localMemoryStatus.dwMemoryLoad << "\n";

	// SIZE_T dwTotalPhys output

	cout << "    Amount of physical memory (in bytes):   " << localMemoryStatus.dwTotalPhys << "\n";

	// SIZE_T dwAvailPhys output

	cout << "    Avaliable physical memory (in bytes):   " << localMemoryStatus.dwAvailPhys << "\n";

	// SIZE_T dwTotalPageFile output

	cout << "    Commited memory limit size, PM + page file - overhead (in bytes): " << localMemoryStatus.dwTotalPageFile << "\n";

	// SIZE_T dwAvailPageFile output

	cout << "    Max memory amount current process can commit (in bytes):          " << localMemoryStatus.dwAvailPageFile << "\n";

	// SIZE_T dwTotalVirtual output

	cout << "    VAS's user-mode portion, who call processes, size (in bytes):     " << localMemoryStatus.dwTotalVirtual << "\n";

	// SIZE_T dwAvailVirtual output

	cout << "    Unreserved & uncommitted VAS's user-mode portion size (in bytes): " << localMemoryStatus.dwAvailVirtual << "\n";
}

// ---------- 2 -- LOCAL GLOBAL MEMORY STATUS EX ----------

/*
BOOL GlobalMemoryStatusEx(
  LPMEMORYSTATUSEX lpBuffer
);

typedef struct _MEMORYSTATUSEX {
  DWORD     dwLength;
  DWORD     dwMemoryLoad;
  DWORDLONG ullTotalPhys;
  DWORDLONG ullAvailPhys;
  DWORDLONG ullTotalPageFile;
  DWORDLONG ullAvailPageFile;
  DWORDLONG ullTotalVirtual;
  DWORDLONG ullAvailVirtual;
  DWORDLONG ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;
*/

void LocalGlobalMemoryStatusEx ()
{
	MEMORYSTATUSEX localMemoryStatusEx; // creating structure
	localMemoryStatusEx.dwLength = sizeof (localMemoryStatusEx); // necessarily, without it it doesn't work!!!
	bool localFlag = GlobalMemoryStatusEx(&localMemoryStatusEx); // sending the pointer and getting the information

	// Physical memory refers to the actual RAM of the system
	if (localFlag == true)
	{
		cout << "Physical memory (RAM) information:\n"; // information output

		// DWORD dwLength output

		cout << "    MEMORYSTATUSEX structure size (in bytes): " << localMemoryStatusEx.dwLength << "\n";

		// DWORD dwMemoryLoad output

		cout << "    Approximate physical memory use (in %): " << localMemoryStatusEx.dwMemoryLoad << "\n";

		// DWORDLONG ullTotalPhys output

		cout << "    Amount of physical memory (in bytes):   " << localMemoryStatusEx.ullTotalPhys << "\n";

		// DWORDLONG ullAvailPhys output

		cout << "    Avaliable physical memory (in bytes):   " << localMemoryStatusEx.ullAvailPhys << "\n";

		// DWORDLONG ullTotalPageFile output

		cout << "    Commited memory limit size, PM + page file - overhead (in bytes): " << localMemoryStatusEx.ullTotalPageFile << "\n";

		// DWORDLONG ullAvailPageFile output

		cout << "    Max memory amount current process can commit (in bytes):          " << localMemoryStatusEx.ullAvailPageFile << "\n";

		// DWORDLONG ullTotalVirtual output

		cout << "    VAS's user-mode portion, who call processes, size (in bytes):     " << localMemoryStatusEx.ullTotalVirtual << "\n";

		// DWORDLONG ullAvailVirtual output

		cout << "    Unreserved & uncommitted VAS's user-mode portion size (in bytes): " << localMemoryStatusEx.ullAvailVirtual << "\n";

		// DWORDLONG ullAvailExtendedVirtual output

		cout << "    Reserved value (equals 0): " << localMemoryStatusEx.ullAvailExtendedVirtual << "\n";
	}
	else
	{
		cout << "Something went wrong! Last error code: " << GetLastError() << "\n";
	}
}

// ---------- 3 -- LOCAL VIRTUAL QUERY ----------

/*
SIZE_T VirtualQuery(
  LPCVOID                   lpAddress,
  PMEMORY_BASIC_INFORMATION lpBuffer,
  SIZE_T                    dwLength
);

typedef struct _MEMORY_BASIC_INFORMATION {
  PVOID  BaseAddress;
  PVOID  AllocationBase;
  DWORD  AllocationProtect;
  WORD   PartitionId;
  SIZE_T RegionSize;
  DWORD  State;
  DWORD  Protect;
  DWORD  Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;
*/

void LocalVirtualQuery ()
{
	DWORD localAdress = 0x11376077;
	//DWORD localAdress = -1; // creating adress variable
	MEMORY_BASIC_INFORMATION localBuffer; // creating buffer for information write
	SIZE_T localLength; // creating size variable (for what?)

	do
	{
		cout << "Please, input virtual adress space (in hex, 0x<hex number>): ";
		cin >> hex >> localAdress >> dec;
	} while (localAdress < 0x00000000 || localAdress > 0xffffffff);

	// The return value is the actual number of bytes returned in the information buffer.
	// If the function fails, the return value is zero. To get extended error information, call GetLastError. Possible error values include ERROR_INVALID_PARAMETER.
	SIZE_T localVirtualQuery = VirtualQuery ((LPVOID)localAdress, &localBuffer, sizeof(localBuffer));
	// LPVOID -- pointer
	// LPCVOID -- pointer to constant

	// Physical memory refers to the actual RAM of the system
	if (localVirtualQuery != 0)
	{
		cout << "Physical memory (RAM) information:\n"; // information output

		// PVOID BaseAddress output

		cout << "    Pointer to the base address of the region of pages:  " << localBuffer.BaseAddress << "\n";

		// PVOID AllocationBase output

		cout << "    Pointer -- // -- allocated by the VirtualAlloc:      " << localBuffer.AllocationBase << "\n";

		// DWORD AllocationProtect output

		cout << "    Memory protection option (for initially allocation): " << localBuffer.AllocationProtect << "\n";

		// WORD PartitionId output

		//cout << "    Partition ID (?): " << localBuffer.PartitionId << "\n"; // compiler can't recognize that

		// SIZE_T RegionSize output

		cout << "    Region's size from base address, pages identical attributes (in bytes): " << localBuffer.RegionSize << "\n";

		// DWORD State output

		if (localBuffer.State == MEM_COMMIT) // number 0x1000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Committed pages for which mem has been allocated\n";
		}
		else if (localBuffer.State == MEM_FREE) // number 0x10000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Free pages not for process, but for allocation\n";
		}
		else if (localBuffer.State == MEM_RESERVE) // number 0x2000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Reserved pages without allocation\n";
		}
		else // another number
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "THIS NUMBER DOESN'T MEAN ANYTHING\n";
		}

		// DWORD Protect output

		cout << "    Access protection of the pages in the region:        " << localBuffer.Protect << "\n";

		// DWORD Type output

		if (localBuffer.Type == MEM_IMAGE) // number 0x1000000
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "Memory pages -> image section\n";
		}
		else if (localBuffer.Type == MEM_MAPPED) // number 0x40000
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "Memory pages -> section\n";
		}
		else if (localBuffer.Type == MEM_PRIVATE) // number 0x20000
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "Memory pages -> private\n";
		}
		else // another number
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "THIS NUMBER DOESN'T MEAN ANYTHING\n";
		}
	}
	else
	{
		cout << "Something went wrong! Last error code: " << GetLastError() << "\n";
	}
}

// ---------- 4 -- LOCAL VIRTUAL ALLOC ----------

/*
LPVOID VirtualAlloc(
  [in, optional] LPVOID lpAddress,
  [in]           SIZE_T dwSize,
  [in]           DWORD  flAllocationType,
  [in]           DWORD  flProtect
);
*/

void LocalVirtualAlloc ()
{
	int localChooseAllocation = 0;
	int localChooseProtect = 0;
	DWORD localAdress = 0x11376077;
	DWORD localflAllocationType = 0;
	DWORD localflProtect = 0;
	//DWORD localAdress = -1; // creating adress variable
	MEMORY_BASIC_INFORMATION localBuffer; // creating buffer for information write
	SIZE_T localLength; // creating size variable (for what?)
	SIZE_T localMemorySize = 4096;

	do
	{
		cout << "Please, input virtual adress space (in hex, 0x<hex number>): ";
		cin >> hex >> localAdress >> dec;
	} while (localAdress < 0x00000000 || localAdress > 0xffffffff);

	// The return value is the actual number of bytes returned in the information buffer.
	// If the function fails, the return value is zero. To get extended error information, call GetLastError. Possible error values include ERROR_INVALID_PARAMETER.
	LPVOID localVirtualAlloc = VirtualAlloc (NULL, localMemorySize, MEM_RESERVE, PAGE_READWRITE);
	// LPVOID -- pointer
	// LPCVOID -- pointer to constant

	while (localChooseAllocation < 1 && localChooseAllocation > 8)
	{
		cout << "Please, input the allocation type constant:\n"
		<< "1 -- MEM_COMMIT (0x00001000)\n"
		<< "2 -- MEM_RESERVE (0x00002000)\n"
		<< "3 -- MEM_RESET (0x00080000)\n"
		<< "4 -- MEM_RESET_UNDO (0x1000000)\n"
		<< "5 -- MEM_LARGE_PAGES (0x20000000)\n"
		<< "6 -- MEM_PHYSICAL (0x00400000)\n"
		<< "7 -- MEM_TOP_DOWN (0x00100000)\n"
		<< "8 -- MEM_WRITE_WATCH (0x00200000)\n"
		<< "9 -- MEM_COMMIT | MEM_RESERVE"

		cin >> localChooseAllocation;
	}

	while (localChooseProtect < 1 && localChooseProtect > 13)
	{
		cout << "Please, input the allocation type constant:\n"
		<< "1 -- PAGE_EXECUTE (0x10)\n"
		<< "2 -- PAGE_EXECUTE_READ (0x20)\n"
		<< "3 -- PAGE_EXECUTE_READWRITE (0x40)\n"
		<< "4 -- PAGE_EXECUTE_WRITECOPY (0x80)\n"
		<< "5 -- PAGE_NOACCESS (0x01)\n"
		<< "6 -- PAGE_READONLY (0x02)\n"
		<< "7 -- PAGE_READWRITE (0x04)\n"
		<< "8 -- PAGE_WRITECOPY (0x08)\n"
		<< "9 -- PAGE_TARGETS_INVALID (0x40000000)\n"
		<< "10 -- PAGE_TARGETS_NO_UPDATE (0x40000000)\n"
		<< "11 -- PAGE_GUARD (0x100)\n"
		<< "12 -- PAGE_NOCACHE (0x200)\n"
		<< "13 -- PAGE_WRITECOMBINE (0x400)\n";

		cin >> localChooseProtect;
	}

	if (localVirtualAlloc != NULL)
	{
		cout << "Allocation was successfull\n" << localVirtualAlloc << "\n";
		if (VirtualFree (localVirtualAlloc, 0, MEM_RELEASE))
		{
			cout << "Free was successfull\n";
		}
		else
		{
			cout << "Free was NOT successfull" << endl;
		}
	}
	else
	{
		cout << "Allocation was NOT successfull\n";
	}

	// Physical memory refers to the actual RAM of the system
	/*if (localVirtualQuery != 0)
	{
		cout << "Physical memory (RAM) information:\n"; // information output

		// PVOID BaseAddress output

		cout << "    Pointer to the base address of the region of pages:  " << localBuffer.BaseAddress << "\n";

		// PVOID AllocationBase output

		cout << "    Pointer -- // -- allocated by the VirtualAlloc:      " << localBuffer.AllocationBase << "\n";

		// DWORD AllocationProtect output

		cout << "    Memory protection option (for initially allocation): " << localBuffer.AllocationProtect << "\n";

		// WORD PartitionId output

		//cout << "    Partition ID (?): " << localBuffer.PartitionId << "\n"; // compiler can't recognize that

		// SIZE_T RegionSize output

		cout << "    Region's size from base address, pages identical attributes (in bytes): " << localBuffer.RegionSize << "\n";

		// DWORD State output

		if (localBuffer.State == MEM_COMMIT) // number 0x00001000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Committed pages for which mem has been allocated\n";
		}
		else if (localBuffer.State == MEM_RESERVE) // number 0x00002000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Free pages not for process, but for allocation\n";
		}
		else if (localBuffer.State == MEM_RESET) // number 0x00080000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Reserved pages without allocation\n";
		}
		else if (localBuffer.State == MEM_RESET_UNDO) // number 0x1000000
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "Reserved pages without allocation\n";
		}
		else // another number
		{
			cout << "    The state of the pages in the region:                0x" << hex << localBuffer.State << dec << " -- " << "THIS NUMBER DOESN'T MEAN ANYTHING\n";
		}

		// DWORD Protect output

		cout << "    Access protection of the pages in the region:        " << localBuffer.Protect << "\n";

		// DWORD Type output

		if (localBuffer.Type == MEM_IMAGE) // number 0x1000000
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "Memory pages -> image section\n";
		}
		else if (localBuffer.Type == MEM_MAPPED) // number 0x40000
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "Memory pages -> section\n";
		}
		else if (localBuffer.Type == MEM_PRIVATE) // number 0x20000
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "Memory pages -> private\n";
		}
		else // another number
		{
			cout << "    The type of pages in the region:                     0x" << hex << localBuffer.Type << dec << " -- " << "THIS NUMBER DOESN'T MEAN ANYTHING\n";
		}
	}
	else
	{
		cout << "Something went wrong! Last error code: " << GetLastError() << "\n";
	}*/
}

// ---------- 1 -- GET LOGICAL DRIVES ----------

void LocalGetLogicalDrives()
{
	int localDriveNumber = 1;
	int localDiskDetection;
	char localDriveLetter[4];
	DWORD dr = GetLogicalDrives();

	for(int i = 0; i < 26; i++)
	{
		localDiskDetection = ((dr >> i) & 0x00000001);
		if (localDiskDetection == 1)
		{
			localDriveLetter[0] = char(65 + i);
			localDriveLetter[1] = ':';
			localDriveLetter[2] = '\\';
			localDriveLetter[3] = 0;
			cout << localDriveNumber << ". Available disk drive: " << localDriveLetter << endl;
			localDriveNumber = localDriveNumber + 1; // next number
		}
	}
}

// ---------- 2 -- GET DRIVE TYPE ----------

void LocalGetDriveType()
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

// ---------- 2 -- GET VOLUME INFORMATION ----------

void LocalGetVolumeInformation ()
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
	else
	{
		cout << " Not Present (GetVolumeInformation)" << endl;
	}
}

// ---------- 2 -- GET DISK FREE SPACE EX ----------

void LocalGetDiskFreeSpaceEx ()
{
	DWORD FreeBytesAvailable;
	DWORD TotalNumberOfBytes;
	DWORD TotalNumberOfFreeBytes;

	BOOL GetDiskFreeSpaceFlag = GetDiskFreeSpaceEx("c:\\", // directory name
	(PULARGE_INTEGER)&FreeBytesAvailable, // bytes available to caller
	(PULARGE_INTEGER)&TotalNumberOfBytes, // bytes on disk
	(PULARGE_INTEGER)&TotalNumberOfFreeBytes  // free bytes on disk
	);
	if(GetDiskFreeSpaceFlag != 0)
	{
		//double d = double(unsigned long(TotalNumberOfFreeBytes))/1024/1024/1024;
		//cout << d;
		cout << " Total Number Of Free Bytes = " << (unsigned long)TotalNumberOfFreeBytes << "( " << (double)(((unsigned long)(TotalNumberOfFreeBytes/1024))/1000) << " Mb )" << endl;
		cout << " Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes << "( " << (double)(((unsigned long)(TotalNumberOfBytes/1024))/1000) << " Mb )" << endl;
		cout << " Total Number Of Bytes = " << (unsigned long)TotalNumberOfBytes << "( " << TotalNumberOfBytes << " Mb )" << endl;
		unsigned long tmp_1 = TotalNumberOfBytes;
		tmp_1 = tmp_1/1024;
		cout << tmp_1;
		double tmp_2 = tmp_1;
		tmp_2 = tmp_2/1000;
		cout << tmp_2;
		cout << TotalNumberOfBytes/1024000;
	}
	else
	{
		cout << " Not Present (GetDiskFreeSpace)" << endl;
	}
}

// ---------- 2 -- GET DISK FREE SPACE ----------

void LocalGetDiskFreeSpace ()
{
	/*long unsigned int * secPerClus;
	long unsigned int * bytePerSec;
	long unsigned int * freeClus;
	long unsigned int * totalClus;*/
	/*LPDWORD secPerClus;
	LPDWORD bytePerSec;
	LPDWORD freeClus;
	LPDWORD totalClus;*/

	string localDiskName = GetDiskName();

	//const char diskNameCC[4] = {'e', ':', '\\'}; // you can do this
	//string diskNameS = "e:\\"; // and you can do this

	DWORD secPerClus;
	DWORD bytePerSec;
	DWORD freeClus;
	DWORD totalClus;
	//int gdfs = GetDiskFreeSpace(diskNameCC, &secPerClus, &bytePerSec, &freeClus, &totalClus); // const char* explicitly
	//int gdfs = GetDiskFreeSpace(diskNameS.c_str(), &secPerClus, &bytePerSec, &freeClus, &totalClus); // const char* from string (with c_str() method)
	int gdfs = GetDiskFreeSpace(localDiskName.c_str(), &secPerClus, &bytePerSec, &freeClus, &totalClus);

	if (gdfs != 0)
	{
		cout << "Disk free space information about disk " << localDiskName << "!\n";
		cout << "---------- NET NUMBERS ----------\n";
		cout << "Total Number Of Sectors per Cluster: " << (unsigned long long)secPerClus << "\nTotal Number Of Bytes per Sector: " << (unsigned long long)bytePerSec << "\nTotal Number Of Free Clusters: " << (unsigned long long)freeClus << "\nTotal Number Of Clusters: " << (unsigned long long)totalClus << "\nReturned value: " << (unsigned long long)gdfs << "\n";
		cout << "---------- CALCULATED NUMBERS ----------\n";
		cout << "Total Number Of Bytes per Cluster: " << (unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\nTotal Number Of Free Sectors: " << (unsigned long long)freeClus*(unsigned long long)secPerClus;
		cout << "\nTotal Number Of Sectors: " << (unsigned long long)totalClus*(unsigned long long)secPerClus << "\nTotal Number Of Free Bytes: " << (unsigned long long)freeClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\nTotal Number of Bytes: " << (unsigned long long)totalClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\n";
		cout << "Total Number Of Used Clusters: " << (unsigned long long)totalClus - (unsigned long long)freeClus;
		cout << "\nTotal Number Of Used Sectors: " << (unsigned long long)totalClus*(unsigned long long)secPerClus - (unsigned long long)freeClus*(unsigned long long)secPerClus << "\nTotal Number Of Used Bytes: " << (unsigned long long)totalClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec - (unsigned long long)freeClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\n";
	}
	else
	{
		cout << "Returned value: " << (unsigned long long)gdfs << "\nThere is no such disk as " << localDiskName << "!\n";
	}
}

// ---------- 2 -- GET DISK FREE SPACE ----------

void GetDiskInfo()
{
	string drive = GetPathShell('f', 's', "Path to the DISK.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");
    DWORD drive_type = GetDriveTypeA(drive.c_str());
    switch (drive_type) {
        case DRIVE_UNKNOWN: cout<< "The drive type cannot be determined.\n";
            break;
        case DRIVE_NO_ROOT_DIR: cout<< "The root path is invalid; for example, there is no volume mounted at the specified path. \n";
            break;
        case DRIVE_REMOVABLE: cout<< "The drive \""<< drive<<"\" has removable media; for example, a floppy drive, thumb drive, or flash card reader. \n";
            break;
        case DRIVE_FIXED: cout<< "The drive \""<< drive<<"\"has fixed media; for example, a hard disk drive or flash drive. \n";
            break;
        case DRIVE_REMOTE: cout<< "The drive \""<< drive<<"\" is a remote (network) drive. \n";
            break;
        case DRIVE_CDROM: cout<< "The drive \""<< drive<<"\" is a CD-ROM drive\n";
            break;
        case DRIVE_RAMDISK: cout<< "The drive \""<< drive<<"\" is a RAM disk.\n";
            break;
        default: cout<< "You shouldn't see this massage. Smth goes wrong";
    }


    char nameBuffer[100];
    char sysNameBuff[100];
    DWORD serialNumber,maxComponentLength,fileSystemFlags;
   if(GetVolumeInformationA(drive.c_str(),nameBuffer,sizeof(nameBuffer),&serialNumber,&maxComponentLength,&fileSystemFlags,sysNameBuff,sizeof(sysNameBuff))){
        cout << "\nDrive name:" << nameBuffer << endl <<
        "Type of File system:" << sysNameBuff << endl <<
        "Serial number:" << serialNumber << endl <<
        "System flags:" << endl;
       string specVol = "The specified volume";
       string specVolSup = specVol + " supports";
       if (fileSystemFlags & FILE_CASE_PRESERVED_NAMES)
           cout << specVol + " preserved case of file names when it places a name on disk.\n";
       if (fileSystemFlags & FILE_CASE_SENSITIVE_SEARCH)
           cout << specVolSup + " case-sensitive file names.\n";
       if (fileSystemFlags & FILE_FILE_COMPRESSION)
           cout << specVolSup + " file-based compression.\n";
       if (fileSystemFlags & FILE_NAMED_STREAMS)
           cout << specVolSup + " named streams.\n";
       if (fileSystemFlags & FILE_PERSISTENT_ACLS)
           cout << specVol + " preserves and enforces access control lists (ACL). For example, the NTFS file system preserves and enforces ACLs, and the FAT file system does not.\n";
       if (fileSystemFlags & FILE_READ_ONLY_VOLUME)
           cout << specVol + " is read-only.\n";
       if (fileSystemFlags & FILE_SEQUENTIAL_WRITE_ONCE)
           cout << specVolSup + " a single sequential write.\n";
       if (fileSystemFlags & FILE_SUPPORTS_ENCRYPTION)
           cout << specVolSup + " the Encrypted File System (EFS).\n";
       if (fileSystemFlags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES)
           cout << specVolSup + " extended attributes.\n";
       if (fileSystemFlags & FILE_SUPPORTS_HARD_LINKS)
           cout << specVolSup + " hard links. \n";
       if (fileSystemFlags & FILE_SUPPORTS_OBJECT_IDS)
           cout << specVolSup + " object identifiers.\n";
       if (fileSystemFlags & FILE_SUPPORTS_OPEN_BY_FILE_ID)
           cout << specVolSup + " open by FileID.\n";
       if (fileSystemFlags & FILE_SUPPORTS_REPARSE_POINTS)
           cout << specVolSup + " reparse points.\n";
       if (fileSystemFlags & FILE_SUPPORTS_SPARSE_FILES)
           cout << specVolSup + " sparse files.\n";
       if (fileSystemFlags & FILE_SUPPORTS_TRANSACTIONS)
           cout << specVolSup + " transactions.\n";
       if (fileSystemFlags & FILE_SUPPORTS_USN_JOURNAL)
           cout << specVolSup + " update sequence number (USN) journals.\n";
       if (fileSystemFlags & FILE_UNICODE_ON_DISK)
           cout << specVolSup + " Unicode in file names as they appear on disk.\n";
       if (fileSystemFlags & FILE_VOLUME_IS_COMPRESSED)
           cout << specVol + " is a compressed volume, for example, a DoubleSpace volume.\n";
       if (fileSystemFlags & FILE_VOLUME_QUOTAS)
           cout << specVolSup + " disk quotas.\n";
   }

    long long FreeBytesAvailableToCaller;
    long long TotalNumberOfBytes;
    GetDiskFreeSpaceExA(drive.c_str(), (PULARGE_INTEGER)&FreeBytesAvailableToCaller, (PULARGE_INTEGER)&TotalNumberOfBytes, nullptr);
    cout << "\nTotal drive space:  " << TotalNumberOfBytes << " bytes" << endl;
    cout << "Available drive space:  " << FreeBytesAvailableToCaller << " bytes" << endl;
}

// ---------- 3 -- CREATE DIRECTORY ----------

bool dirExists(const std::string& dirName_in)
{
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
	{
		return false; // something is wrong with your path!
	}
	else if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true; // this is a directory!
	}
	else
	{
		return false; // this is not a directory!
	}
}

// ---------- 3 -- GET PATH KERNEL ----------

// 'c' -- current (working) directory path
// 'a' -- absolute directory path
// 'r' -- relative directory path
// 'f' -- absolute (full) file path
// 's' -- relative (short) file path
string GetPathKernel (char localFlag)
{
	char localCommit = 'n';
	string localPath;
	string localOldPath = currentPath;
	while (localCommit != 'y')
	{
		if (localFlag == 'a') // 'a' means "absolute path"
		{
			cout << "Please, input absolute directory path:\n";
			cin >> localPath;
		}
		else if (localFlag == 'r') // 'r' means "relative path"
		{
			cout << "Your current path is \"" << currentPath << "\". Please, input relative directory path:\n";
			cin >> localPath;
			localPath = currentPath + localPath;
		}
		else if (localFlag == 'f') // 'f' means "full file path"
		{
			cout << "Please, input absolute file path:\n";
			cin >> localPath;
		}
		else if (localFlag == 's') // 's' means "short file path"
		{
			cout << "Your current path is \"" << currentPath << "\". Please, input relative file path:\n";
			cin >> localPath;
			localPath = currentPath + localPath;
		}
		else if (localFlag == 'c') // 'c' means "current"
		{
			cout << "Please, input current working directory path:\n";
			cin >> localPath;
			if (localPath.length() > 0)
			{
				if (localPath.c_str()[localPath.length() - 1] != '\\')
				{
					localPath = localPath + "\\";
				}
			}
		}
		else // current working path by default
		{
			cout << "Please, input current working directory path:\n";
			cin >> localPath;
		}
		if (localFlag != 'a' && localFlag != 'r' && localFlag != 'f' && localFlag != 's' && dirExists(localPath))
		{
			if (localFlag == 'c') // 'c' means "current"
			{
				cout << "This is your new current working path: " << localPath << "\n";
			}
			else // current working path by default
			{
				cout << "This is your new current working path: " << localPath << "\n";
			}
			cout << "Commit changes? [y/n]\n";
			cin >> localCommit;
		}
		else if (localFlag == 'a' || localFlag == 'r') // for directories
		{
			if (localFlag == 'a') // 'a' means "absolute path"
			{
				cout << "This is your new absolute directory path: " << localPath << "\n";
			}
			else if (localFlag == 'r') // 'r' means "relative path"
			{
				cout << "This is your new relative directory path: " << localPath << "\n";
			}
			cout << "Commit changes? [y/n]\n";
			cin >> localCommit;
		}
		else if (localFlag == 'f' || localFlag == 's') // for files
		{
			if (localFlag == 'f') // 'f' means "full file path"
			{
				cout << "This is your new absolute file path: " << localPath << "\n";
			}
			else if (localFlag == 's') // 's' means "shorth file path"
			{
				cout << "This is your new relative file path: " << localPath << "\n";
			}
			cout << "Commit changes? [y/n]\n";
			cin >> localCommit;
		}
		else
		{
			cout << "Your new path \"" << localPath << "\"isn't valid (exist)! Try again (if no, old path will be returned)? [y/n]: " << "\n";
			cin >> localCommit;
			if (localCommit == 'y')
			{
				localCommit = 'n';
			}
			else if (localCommit == 'n')
			{
				localCommit = 'y';
				localPath = localOldPath;
			}
		}
	}
	if (localFlag != 'a' && localFlag != 'r' && localFlag != 'f' && localFlag != 's')
	{
		currentPath = localPath;
	}
	// because for setting current directory it doesn't matter, but for creating/removing it matters,
	// so to unificate the program, i make local path a current only if it's not for creating or deleting,
	// but i return the local path (for every flags it will be right, only consist different things)
	return localPath;
}

// ---------- 3 -- GET PATH SHELL ----------

string GetPathShell (char localFlagOne, char localFlagTwo, string localMessageOne, string localMessageTwo)
{
	//"Path to the FILE, which ATTRIBUTES you WANT TO GET.\n"
	char localFunctionFlag = '!'; // just random symbol
	string localFunctionPath;
	while (localFunctionFlag != localFlagOne && localFunctionFlag != localFlagTwo)
	{
		cout << localMessageOne << localMessageTwo;
		// checking for the flag
		cin >> localFunctionFlag;
		if (localFunctionFlag != localFlagOne && localFunctionFlag != localFlagTwo)
		{
			cout << "Try again!\n";
		}
	}
	if (localFunctionFlag == localFlagOne) // full file path situation
	{
		localFunctionPath = GetPathKernel(localFunctionFlag); // set new absolute path
	}
	else if (localFunctionFlag == localFlagTwo) // short file path situation
	{
		char localChange = 'n';
		cout << "Do you want to change current working path? [y/n]\n";
		cin >> localChange;
		if (localChange == 'y')
		{
			currentPath = GetPathKernel('c'); // changing current directory
		}
		localFunctionPath = GetPathKernel(localFunctionFlag); // set new relative path
	}
	return localFunctionPath;
}

// ---------- 3 -- LOCAL CREATE DIRECTORY ----------

void LocalCreateRemoveDirectory (char actionCreateRemove) // if 'c' -- creating directory, if 'r' -- removing directory, creating by default
{
	char localPathFlag = 'y'; // just another letter, not 'a' or 'r', so you need input it anyway
	string localDirectory; // directory path you input

	localDirectory = GetPathShell('a', 'r', "", "Do you want to input absolute path of the directory or relative? [a/r]\n");

	if (actionCreateRemove == 'r') // 'r'
	{
		if (RemoveDirectory(localDirectory.c_str()))
		{
			cout << "The directory \"" << localDirectory << "\" has been successfully removed!\n";
		}
		else
		{
			cout << "Something wrong! The directory \"" << localDirectory << "\" hasn't been removed!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
	else // 'c' and default
	{
		if (CreateDirectory(localDirectory.c_str(), NULL))
		{
			cout << "The directory \"" << localDirectory << "\" has been successfully created!\n";
		}
		else
		{
			cout << "Something wrong! The directory \"" << localDirectory << "\" hasn't been created!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
}

// ---------- 4 -- LOCAL CREATE FILE ----------

void LocalCreateFile () // A WISE FACT: THERE IS NO "OPEN FILE" FINCTION, THERE IS "CREATE FILE" FUNCTION WITH SPECIAL FLAG TO OPEN FILE!
{
	int localChoose = 0; // to start a loop
	unsigned long localCreationDisposition; // DWORD = unsigned long, localChoose = number that has been written in specification
	char localPathFlag = 'y'; // just another letter, not 'f' or 's', so you need input it anyway
	string localFilePath; // file path you input

	localFilePath = GetPathShell('f', 's', "Path to the FILE, which you WANT TO CREATE (OPEN).\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");

	while (localChoose < 1 || localChoose > 5)
	{
		// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
		cout << "Please, choose the creation disposition (CREATE_NEW by default):\n" << "1 -- CREATE_NEW\n" << "2 -- CREATE_ALWAYS\n" << "3 -- OPEN_EXISTING\n"
		<< "4 -- OPEN_ALWAYS\n" << "5 -- TRUNCATE_EXISTING\n"; // DWORD = unsigned long, localChoose = number that has been written in specification
		cin >> localChoose;
		if (localChoose < 1 || localChoose > 5)
		{
			cout << "Try again!\n";
		}
	}

	switch(localChoose)
	{
		case 1:
			localCreationDisposition = (unsigned long)localChoose; // DWORD = unsigned long, localChoose = number that has been written in specification
			break;
		case 2:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		case 3:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		case 4:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		case 5:
			localCreationDisposition = (unsigned long)localChoose;
			break;
		default:
			localCreationDisposition = (unsigned long)localChoose;
			break;
	}

	// Open a handle to the file
	HANDLE localFile = CreateFile(
		localFilePath.c_str(), // Filename (<path to the file>)
		GENERIC_WRITE, // Desired access (0/GENERIC_READ/GENERIC_WRITE)
		FILE_SHARE_READ, // Share mode (FILE_SHARE_DELETE/FILE_SHARE_READ/FILE_SHARE_WRITE)
		NULL, // Security attributes (NULL/<structure SECURITY_ATTRIBUTES adress>)
		(DWORD)localCreationDisposition, // Creates a new file, only if it doesn't already exist (CREATE_ALWAYS/CREATE_NEW/OPEN_ALWAYS/OPEN_EXISTING/TRUNCATE_EXISTING)
		FILE_ATTRIBUTE_NORMAL, // Flags and attributes (FILE_ATTRIBUTE_NORMAL/FILE_ATTRIBUTE_ARCHIVE/FILE_ATTRIBUTE_ENCRYPTED/FILE_ATTRIBUTE_SYSTEM/
		// FILE_ATTRIBUTE_HIDDEN/FILE_ATTRIBUTE_NOT_CONTENT_INDEXED/FILE_ATTRIBUTE_OFFLINE/FILE_ATTRIBUTE_READONLY/FILE_ATTRIBUTE_SYSTEM/FILE_ATTRIBUTE_TEMPORARY/
		// FILE_FLAG_BACKUP_SEMANTICS/FILE_FLAG_DELETE_ON_CLOSE/FILE_FLAG_NO_BUFFERING/FILE_FLAG_OPEN_NO_RECALL/FILE_FLAG_OPEN_REPARSE_POINT/FILE_FLAG_OVERLAPPED/
		// FILE_FLAG_POSIX_SEMANTICS/FILE_FLAG_RANDOM_ACCESS/FILE_FLAG_SEQUENTIAL_SCAN/FILE_FLAG_WRITE_THROUGH/SECURITY_ANONYMOUS/SECURITY_CONTEXT_TRACKING/
		// SECURITY_DELEGATION/SECURITY_EFFECTIVE_ONLY/SECURITY_IDENTIFICATION/SECURITY_IMPERSONATION)
		NULL); // Template file handle (NULL/<template file descripter>)
	if (localFile != INVALID_HANDLE_VALUE)
	{
		cout << "The file \"" << localFilePath << "\" has been successfully created (opened, rewrited)!\n";
		CloseHandle(localFile);
	}
	else
	{
		cout << "Something wrong! The file \"" << localFilePath << "\" hasn't been created (opened, rewrited)!\n";
		cout << "Last error code is \"" << GetLastError() << "\"\n";
	}
}

// ---------- 5 -- LOCAL COPY MOVE FILE ----------

void LocalCopyMoveFile (char actionCopyMove) // 'c' for copy, 'm' for moving, 'e' for extended moving (MoveFileEx) ATTENTION: 'c' -- copy -- is default if there is other letter!
{
	// specification of "CopyFile"
	/*BOOL CopyFile(
		LPCTSTR lpExistingFileName, // current file you want to copy
		LPCTSTR lpNewFileName, // new file, where you wnat to copy the old one
		BOOL bFailIfExists // TRUE means STOP IF NEW FILE EXIST, FALSE means OWERWRITE FILE ANYWAY
	);*/

	int localChoose = 0; // to start a loop
	bool localFailIfExists; // for existing file reaction
	char localPathFlag = 'y'; // just another letter, not 'f' or 's', so you need input it anyway
	char localSameNameFlag = 'a'; // random letter
	string localOldFilePath; // old (copied) file path you input
	string localNewFilePath; // new (pasted) file path you input

	// OLD FILE PATH INPUT (INCLUDING SITUATIONS "COPY" AND "MOVE")

	if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
	{
		localOldFilePath = GetPathShell('f', 's', "Path to the FILE, which you WANT TO MOVE (CUT).\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
	}
	else // 'c' is for "copy", "copy" is default
	{
		localOldFilePath = GetPathShell('f', 's', "Path to the FILE, which you WANT TO COPY.\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
	}

	// SAME NAME SITUATION

	while (localSameNameFlag != 'y' && localSameNameFlag != 'n')
	{
		cout << "Do you want the SAME NAME AS COPIED/CUT file (including extension)? [y/n]\n";
		// checking for the flag
		cin >> localSameNameFlag;
		if (localSameNameFlag != 'y' && localSameNameFlag != 'n') // 'n' -- no -- by default
		{
			cout << "Try again!\n";
		}
	}	

	// NEW FILE PATH INPUT

	//localPathFlag = 'y'; // random letter again to prevent ignoring if construction

	if (localSameNameFlag == 'y') // same name of the file -- choosing full directory path instead of the filename
	{
		string localFilename = string(localOldFilePath);
		// Remove directory if present.
		// Do this before extension removal incase directory has a period character.
		const size_t last_slash_idx = localFilename.find_last_of("\\/");
		if (std::string::npos != last_slash_idx)
		{
		    localFilename.erase(0, last_slash_idx + 1);
		}
		// Remove extension if present.
		/*const size_t period_idx = localFilename.rfind('.');
		if (std::string::npos != period_idx)
		{
		    localFilename.erase(period_idx);
		}*/

		if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
		{
			localNewFilePath = GetPathShell('a', 'r', "Path to the NEW DIRECTORY, where you WANT to MOVE (PASTE) the CUT FILE.\n", "Do you want to input absolute path of the directory or relative? [a/r]\n") + localFilename;
		}
		else // 'c' is for "copy", "copy" is DEFAULT
		{
			localNewFilePath = GetPathShell('a', 'r', "Path to the NEW DIRECTORY, where you WANT to PASTE the COPIED FILE.\n", "Do you want to input absolute path of the directory or relative? [a/r]\n") + localFilename;
		}
	}
	else // 'n', different (as user choosed) name of the file -- choosing full file path (not the same name is DEFAULT)
	{
		if (actionCopyMove == 'm' || actionCopyMove == 'e') // 'm' is for "moving", 'e' is for "extended moving"
		{
			localNewFilePath = GetPathShell('f', 's', "Path to the NEW FILE, where you WANT to MOVE (PASTE) the CUT FILE.\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
		}
		else // 'c' is for "copy", "copy" is DEFAULT
		{
			localNewFilePath = GetPathShell('f', 's', "Path to the NEW FILE, where you WANT to PASTE the COPIED FILE.\n", "Do you want to input absolute (full) path of the file or relative (short)? [f/s]\n");
		}
	}

	// COPY and MOVE

	if (actionCopyMove == 'c' || (actionCopyMove != 'm' && actionCopyMove != 'e')) // copy
	{
		// IF NEW FILE EXISTS

		while (localChoose < 1 || localChoose > 2)
		{
			// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
			cout << "Please, choose the possible outcome for you IF NEW FILE EXISTS:\n" << "1 -- DON'T PASTE AND DON'T REWRITE ANY FILE (TRUE)\n"
			<< "2 -- REWRITE THE FILE (FALSE)\n";
			cin >> localChoose;
			if (localChoose < 1 || localChoose > 2)
			{
				cout << "Try again!\n";
			}
		}

		switch(localChoose)
		{
			case 1:
				localFailIfExists = true;
				break;
			case 2:
				localFailIfExists = false;
				break;
			default:
				localFailIfExists = true; // true -- file rewriting protection -- by default
				break;
		}

		// don't forget about c_str()!
		if (CopyFile(localOldFilePath.c_str(), localNewFilePath.c_str(), localFailIfExists)) // copy file and watching the result immediatly
		{
			cout << "The file \"" << localOldFilePath << "\" has been successfully copied to file \"" << localNewFilePath << "\"!\n";
		}
		else
		{
			cout << "Something wrong! The file \"" << localOldFilePath << "\" hasn't been copied to file \"" << localNewFilePath << "\"!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
	else if (actionCopyMove == 'm') // move
	{
		// don't forget about c_str()!
		if (MoveFile(localOldFilePath.c_str(), localNewFilePath.c_str())) // move file and watching the result immediatly
		{
			cout << "The file \"" << localOldFilePath << "\" has been successfully moved to file \"" << localNewFilePath << "\"!\n";
		}
		else
		{
			cout << "Something wrong! The file \"" << localOldFilePath << "\" hasn't been moved to file \"" << localNewFilePath << "\"!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n";
		}
	}
	else if (actionCopyMove == 'e') // extended move
	{
		string localChooseTwo = "2"; // because default
		// next parameters presented in decimal code, but can be translated at once (for example "111111" is all of the flags (but all is uncorrect)!)
		// MOVEFILE_COPY_ALLOWED = 2 (copy, than delete old, normal work), cannot be used with MOVEFILE_DELAY_UNTIL_REBOOT
		// MOVEFILE_CREATE_HARDLINK = 16 ("Reserved for future use", i don't know what does it means)
		// MOVEFILE_DELAY_UNTIL_REBOOT = 4 (waiting util reboot), cannot be used with MOVEFILE_COPY_ALLOWED
		// MOVEFILE_FAIL_IF_NOT_TRACKABLE = 32 ("function fails, if the file is the lik source")
		// MOVEFILE_REPLACE_EXISTING = 1 (replacing file)
		// MOVEFILE_WRITE_THROUGH = 8 (doesn't return anything until it's ACTUALLY MOVE SOMETHING!)
		unsigned long inFunctionNumber = 0;
		while (inFunctionNumber == 0)
		{
			// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
			cout << "Please, choose the possible flags for moving file (you CAN CHOOSE MANY -- JUST SPLIT NUMBERS BY SPACE):\n"
			<< "1 -- MOVEFILE_REPLACE_EXISTING (replacing existing, it it exists)\n"
			<< "2 -- MOVEFILE_COPY_ALLOWED (classic move) -- DEFAULT\n"
			<< "4 -- MOVEFILE_DELAY_UNTIL_REBOOT (moving after rebooting the system)\n"
			<< "8 -- MOVEFILE_WRITE_THROUGH (moving file, than returning value)\n"
			<< "16 -- MOVEFILE_CREATE_HARDLINK\n"
			<< "32 -- MOVEFILE_FAIL_IF_NOT_TRACKABLE\n";
			fflush(stdin);
			std::getline(std::cin, localChooseTwo);

			// spit the string
			std::string s = string(localChooseTwo);
			std::string delimiter = " ";

			int i = 0;
			size_t pos = 0;
			std::string token;
			std::vector<string> v;
			std::vector<int> vect{1, 2, 4, 8, 16, 32};
			while ((pos = s.find(delimiter)) != std::string::npos)
			{
				int tmpNumber = 0;
			    token = s.substr(0, pos);
			    v.push_back(token);
			    tmpNumber = std::stoi(token);
			    if (std::find(vect.begin(), vect.end(), tmpNumber) != vect.end())
			    {
			    	inFunctionNumber = inFunctionNumber + tmpNumber;
			    	vect.erase(std::remove(vect.begin(), vect.end(), tmpNumber), vect.end());
			    }
			    //std::cout << token << std::endl;
			    s.erase(0, pos + delimiter.length());
			}

			int newTMPNumber = std::stoi(s);
			if (std::find(vect.begin(), vect.end(), newTMPNumber) != vect.end())
			{
			    inFunctionNumber = inFunctionNumber + newTMPNumber;
			    vect.erase(std::remove(vect.begin(), vect.end(), newTMPNumber), vect.end());
			}

			//cout << inFunctionNumber;

			//std::cout << s << std::endl;
			// end split of the string

			if (inFunctionNumber == 0)
			{
				cout << "Try again!\n";
			}
		}
		// don't forget about c_str()!
		if (MoveFileEx(localOldFilePath.c_str(), localNewFilePath.c_str(), (DWORD)inFunctionNumber)) // extended move file and watching the result immediatly
		{
			cout << "The file \"" << localOldFilePath << "\" has been successfully moved to file \"" << localNewFilePath << "\"!\n";
		}
		else
		{
			cout << "Something wrong! The file \"" << localOldFilePath << "\" hasn't been moved to file \"" << localNewFilePath << "\"!\n";
			cout << "Last error code is \"" << GetLastError() << "\"\n"; // here i need to insert last error text string
		}
	}
}

// ---------- 6 -- LOCAL GET FILE ATTRIBUTES ----------

void LocalGetFileAttributes ()
{
	// specification of "GetFileAttributesA"
	/*DWORD GetFileAttributesA(
		LPCSTR lpFileName // file name, which i want to get the file attributes
	);*/

	DWORD localFileAttributes = 0;
	string localFilePath; // file path you input

	// FILE PATH INPUT

	localFilePath = GetPathShell('f', 's', "Path to the FILE or DIRECTORY, which ATTRIBUTES you WANT TO GET.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	localFileAttributes = GetFileAttributes(localFilePath.c_str());

	cout << "File \"" << localFilePath << "\" attributes:\n";
	if (localFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
	{
		cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
	{
		cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_DEVICE)
	{
		cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
	{
		cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_HIDDEN)
	{
		cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
	{
		cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_NORMAL)
	{
		cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
	{
		cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
	{
		cout << "Data stream not to be read by the data integrity scanner (FILE_ATTRIBUTE_NO_SCRUB_DATA)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_OFFLINE)
	{
		cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_READONLY)
    {
		cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
	}
	/*if (localFileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
	{
		cout << "Data is not fully presented locally (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
	{
		cout << "Data hasn't physical representation on system (FILE_ATTRIBUTE_RECALL_ON_OPEN)\n";
	}*/
	if (localFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
	{
		cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_SYSTEM)
	{
		cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
	{
		cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
	}
	if (localFileAttributes & FILE_ATTRIBUTE_VIRTUAL)
	{
		cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
	}
}

// ---------- 6 -- LOCAL SET FILE ATTRIBUTES ----------

void LocalSetFileAttributes ()
{
	// specification of "SetFileAttributesA"
	/*BOOL SetFileAttributesA(
		LPCSTR lpFileName, // filename
		DWORD  dwFileAttributes //attributes
	);*/

	DWORD localFileAttributes = 0;
	string localFilePath; // file path you input

	// FILE PATH INPUT

	localFilePath = GetPathShell('f', 's', "Path to the FILE or DIRECTORY, which ATTRIBUTES you WANT TO SET.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	localFileAttributes = GetFileAttributes(localFilePath.c_str());

	string localChooseTwo = "128";
	unsigned long inFunctionNumber = 0;
	while (inFunctionNumber == 0)
	{
		// because "CREATE_NEW" by default (1 is number for "CREATE_NEW")
		cout << "Please, choose the possible attributes for the file/directory (you CAN CHOOSE MANY -- JUST SPLIT NUMBERS BY SPACE):\n"
		<< "1 -- FILE_ATTRIBUTE_READONLY (read-only)\n"
		<< "2 -- FILE_ATTRIBUTE_HIDDEN (hidden)\n"
		<< "4 -- FILE_ATTRIBUTE_SYSTEM (system used)\n"
		<< "32 -- FILE_ATTRIBUTE_ARCHIVE (archive)\n"
		<< "128 -- FILE_ATTRIBUTE_NORMAL -- DEFAULT\n"
		<< "256 -- FILE_ATTRIBUTE_TEMPORARY (temporary storage)\n"
		<< "4096 -- FILE_ATTRIBUTE_OFFLINE (don't avaliable immediatly)\n"
		<< "8192 -- FILE_ATTRIBUTE_NOT_CONTENT_INDEXED (not indexed)\n";
		fflush(stdin);
		std::getline(std::cin, localChooseTwo);

		// spit the string
		std::string s = string(localChooseTwo);
		std::string delimiter = " ";

		int i = 0;
		size_t pos = 0;
		std::string token;
		std::vector<string> v;
		std::vector<int> vect{1, 2, 4, 32, 128, 256, 4096, 8192};
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			int tmpNumber = 0;
		    token = s.substr(0, pos);
		    v.push_back(token);
		    tmpNumber = std::stoi(token);
		    if (std::find(vect.begin(), vect.end(), tmpNumber) != vect.end())
		    {
		    	inFunctionNumber = inFunctionNumber + tmpNumber;
		    	vect.erase(std::remove(vect.begin(), vect.end(), tmpNumber), vect.end());
		    }
		    //std::cout << token << std::endl;
		    s.erase(0, pos + delimiter.length());
		}

		int newTMPNumber = std::stoi(s);
		if (std::find(vect.begin(), vect.end(), newTMPNumber) != vect.end())
		{
		    inFunctionNumber = inFunctionNumber + newTMPNumber;
		    vect.erase(std::remove(vect.begin(), vect.end(), newTMPNumber), vect.end());
		}

		//cout << inFunctionNumber;

		//std::cout << s << std::endl;
		// end split of the string

		if (inFunctionNumber == 0)
		{
			cout << "Try again!\n";
		}
	}

	if (SetFileAttributes(localFilePath.c_str(), (DWORD) inFunctionNumber))
	{
		cout << "The file's (directory's) \"" << localFilePath << "\" attributes has been successfully changed to:\n";
		cout << "File \"" << localFilePath << "\" attributes:\n";
		if (inFunctionNumber & FILE_ATTRIBUTE_ARCHIVE)
		{
			cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_COMPRESSED)
		{
			cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_DEVICE)
		{
			cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_DIRECTORY)
		{
			cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_ENCRYPTED)
		{
			cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_HIDDEN)
		{
			cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
		}
		/*if (inFunctionNumber & FILE_ATTRIBUTE_INTEGRITY_STREAM)
		{
			cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
		}*/
		if (inFunctionNumber & FILE_ATTRIBUTE_NORMAL)
		{
			cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
		{
			cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
		}
		/*if (inFunctionNumber & FILE_ATTRIBUTE_NO_SCRUB_DATA)
		{
			cout << "Data stream not to be read by the data integrity scanner (FILE_ATTRIBUTE_NO_SCRUB_DATA)\n";
		}*/
		if (inFunctionNumber & FILE_ATTRIBUTE_OFFLINE)
		{
			cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_READONLY)
	    {
			cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
		}
		/*if (inFunctionNumber & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
		{
			cout << "Data is not fully presented locally (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_RECALL_ON_OPEN)
		{
			cout << "Data hasn't physical representation on system (FILE_ATTRIBUTE_RECALL_ON_OPEN)\n";
		}*/
		if (inFunctionNumber & FILE_ATTRIBUTE_REPARSE_POINT)
		{
			cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_SPARSE_FILE)
		{
			cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_SYSTEM)
		{
			cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_TEMPORARY)
		{
			cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
		}
		if (inFunctionNumber & FILE_ATTRIBUTE_VIRTUAL)
		{
			cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
		}
	}
	else
	{
		cout << "Something wrong! The file's (directory's) \"" << localFilePath << "\" attributes hasn't been changed\n";
		cout << "Last error code is \"" << GetLastError() << "\"\n"; // here i need to insert last error text string
	}
}

// ---------- 6 -- LOCAL GET FILE INFORMATION BY HANDLE ----------

void LocalGetFileInformationByHandle ()
{
	// specification of "GetFileInformationByHandle"
	/*BOOL GetFileInformationByHandle(
		HANDLE                       hFile, // path to the handle
		LPBY_HANDLE_FILE_INFORMATION lpFileInformation // file information
	);*/
	string localFilePath = GetPathShell('f', 's', "Path to the FILE or DIRECTORY, which ATTRIBUTES you WANT TO GET.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	HANDLE hFile = CreateFile(localFilePath.c_str(), // file name
        GENERIC_READ,          // open for reading
        0,                     // do not share
        NULL,                  // default security
        OPEN_EXISTING,         // existing file only
        FILE_ATTRIBUTE_NORMAL, // normal file
        NULL);
    int size=0;
    //PBY_HANDLE_FILE_INFORMATION lpFileInformation = new _BY_HANDLE_FILE_INFORMATION();
    BY_HANDLE_FILE_INFORMATION* lpFileInformation = new BY_HANDLE_FILE_INFORMATION();
    int resalt = GetFileInformationByHandle(hFile,lpFileInformation);
    size = lpFileInformation->nFileSizeLow;

    DWORD localAttributes = lpFileInformation->dwFileAttributes;

	if (localAttributes & FILE_ATTRIBUTE_ARCHIVE)
	{
		cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_COMPRESSED)
	{
		cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_DEVICE)
	{
		cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_ENCRYPTED)
	{
		cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_HIDDEN)
	{
		cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
	}
	/*if (localAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
	{
		cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
	}*/
	if (localAttributes & FILE_ATTRIBUTE_NORMAL)
	{
		cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
	{
		cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
	}
	/*if (localAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
	{
		cout << "Data stream not to be read by the data integrity scanner (FILE_ATTRIBUTE_NO_SCRUB_DATA)\n";
	}*/
	if (localAttributes & FILE_ATTRIBUTE_OFFLINE)
	{
		cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_READONLY)
    {
		cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
	}
	/*if (localAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
	{
		cout << "Data is not fully presented locally (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
	{
		cout << "Data hasn't physical representation on system (FILE_ATTRIBUTE_RECALL_ON_OPEN)\n";
	}*/
	if (localAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
	{
		cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
	{
		cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_SYSTEM)
	{
		cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_TEMPORARY)
	{
		cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
	}
	if (localAttributes & FILE_ATTRIBUTE_VIRTUAL)
	{
		cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
	}

	char buffer[256];

    SYSTEMTIME st;
    FILETIME ft;
    string strMessage;

    // first

    ft.dwLowDateTime = (lpFileInformation->ftCreationTime).dwLowDateTime;
    ft.dwHighDateTime = (lpFileInformation->ftCreationTime).dwHighDateTime;

    FileTimeToSystemTime(&ft, &st);

    sprintf( buffer,
             "%d-%02d-%02d %02d:%02d:%02d.%03d", 
             st.wYear,
             st.wMonth, 
             st.wDay,                      
             st.wHour, 
             st.wMinute, 
             st.wSecond,
             st.wMilliseconds );
	strMessage = buffer;

	std::cout << "CREATION TIME = " << strMessage << endl;

	// second

    ft.dwLowDateTime = (lpFileInformation->ftLastWriteTime).dwLowDateTime;
    ft.dwHighDateTime = (lpFileInformation->ftLastWriteTime).dwHighDateTime;

    FileTimeToSystemTime(&ft, &st);

    sprintf( buffer,
             "%d-%02d-%02d %02d:%02d:%02d.%03d", 
             st.wYear,
             st.wMonth, 
             st.wDay,                      
             st.wHour, 
             st.wMinute, 
             st.wSecond,
             st.wMilliseconds );
	strMessage = buffer;

	std::cout << "LAST WRITE TIME = " << strMessage << endl;

	// third

    ft.dwLowDateTime = (lpFileInformation->ftLastAccessTime).dwLowDateTime;
    ft.dwHighDateTime = (lpFileInformation->ftLastAccessTime).dwHighDateTime;

    FileTimeToSystemTime(&ft, &st);

    sprintf( buffer,
             "%d-%02d-%02d %02d:%02d:%02d.%03d", 
             st.wYear,
             st.wMonth, 
             st.wDay,                      
             st.wHour, 
             st.wMinute, 
             st.wSecond,
             st.wMilliseconds );
	strMessage = buffer;

	std::cout << "LAST ACCESS TIME = " << strMessage << endl;

    cout << "Volume serial number: " << lpFileInformation->dwVolumeSerialNumber << "\n";
    cout << "Local size high/low: " << lpFileInformation->nFileSizeHigh << " " << lpFileInformation->nFileSizeLow << "\n";
    cout << "Number Of Links: " << lpFileInformation->nNumberOfLinks << "\n";
    cout << "Index High/low: " << lpFileInformation->nFileIndexHigh << " " << lpFileInformation->nFileIndexLow << "\n";

    /*DWORD localAttributes = lpFileInformation->dwFileAttributes;
    //localCreationTime;
    DWORD localAccessTime = (lpFileInformation->ftLastAccessTime).dwLowDateTime;
    //localChangeTime;
    DWORD localVolumeSerialNumber = lpFileInformation->dwVolumeSerialNumber;
    DWORD localSizeHigh = lpFileInformation->nFileSizeHigh;
    DWORD localSizeLow = lpFileInformation->nFileSizeLow;
    DWORD localNumberOfLinks = lpFileInformation->nNumberOfLinks;
    DWORD localIndexHigh = lpFileInformation->nFileIndexHigh;
    DWORD localIndexLow = lpFileInformation->nFileIndexLow;*/

    /*cout << localAttributes;
    cout << localAccessTime;
    cout << localVolumeSerialNumber;
    cout << localSizeHigh;
    cout << localSizeLow;
    cout << localNumberOfLinks;
    cout << localIndexHigh;
    cout << localIndexLow;*/

    CloseHandle(hFile);
}

// ---------- 6 -- LOCAL GET FILE INFORMATION BY HANDLE ----------

void GetFileTime ()
{
	string localFilePath = GetPathShell('f', 's', "Path to the FILE get time.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	HANDLE hFile = CreateFile(localFilePath.c_str(), // file name
	    GENERIC_READ,          // open for reading
	    0,                     // do not share
	    NULL,                  // default security
	    OPEN_EXISTING,         // existing file only
	    0, // normal file
	    NULL);

    FILETIME creationTime;
    FILETIME lastWriteTime;
    FILETIME lastAccessTime;

    if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
    {
    	//FILETIME ft;

	    //ft.dwHighDateTime = creationTime.dwHighDateTime;
	    //ft.dwLowDateTime = creationTime.dwLowDateTime;
	    char buffer[256];

	    SYSTEMTIME st;
	    FileTimeToSystemTime(&creationTime, &st);

	    sprintf( buffer,
	             "%d-%02d-%02d %02d:%02d:%02d.%03d", 
	             st.wYear,
	             st.wMonth, 
	             st.wDay,                      
	             st.wHour, 
	             st.wMinute, 
	             st.wSecond,
	             st.wMilliseconds );
    	string strMessage = buffer;

    	std::cout << "System time = " << strMessage << std::endl;

    	FileTimeToSystemTime(&lastAccessTime, &st);

	    sprintf( buffer,
	             "%d-%02d-%02d %02d:%02d:%02d.%03d", 
	             st.wYear,
	             st.wMonth, 
	             st.wDay,                      
	             st.wHour, 
	             st.wMinute, 
	             st.wSecond,
	             st.wMilliseconds );
    	strMessage = buffer;

    	std::cout << "Access time = " << strMessage << std::endl;

    	FileTimeToSystemTime(&lastWriteTime, &st);

	    sprintf( buffer,
	             "%d-%02d-%02d %02d:%02d:%02d.%03d", 
	             st.wYear,
	             st.wMonth, 
	             st.wDay,                      
	             st.wHour, 
	             st.wMinute, 
	             st.wSecond,
	             st.wMilliseconds );
    	strMessage = buffer;

    	std::cout << "Change time = " << strMessage << std::endl;
    }
    else
    {
        cout << "Something wrong!" << "\n";
    }
    CloseHandle(hFile);
}

// ---------- 6 -- LOCAL GET FILE INFORMATION BY HANDLE ----------

void SetFileTime ()
{
	string localFilePath = GetPathShell('f', 's', "Path to the FILE set time.\n", "Do you want to input absolute (full) path of the file (directory) or relative (short)? [f/s]\n");

	HANDLE hFile = CreateFile(localFilePath.c_str(), // file name
	    GENERIC_WRITE,          // open for reading
	    0,                     // do not share
	    NULL,                  // default security
	    OPEN_EXISTING,         // existing file only
	    0, // normal file
	    NULL);

	string one;
	string two;
	string three;

	cout << "Time format: 2021-10-03 18:29:40.152\n";
	cout << "Creation time:\n";
	fflush(stdin);
	getline(cin, one);

	cout << "Last write time:\n";
	fflush(stdin);
	getline(cin, two);


	cout << "Last access time:\n";
	fflush(stdin);
	getline(cin, three);

	FILETIME creationTime;//= buffTime.creationTime;
    FILETIME lastWriteTime;// = buffTime.lastWriteTime;
    FILETIME lastAccessTime;// = buffTime.lastAccessTime;

    SYSTEMTIME systime_1;
    SYSTEMTIME systime_2;
    SYSTEMTIME systime_3;

    memset(&systime_1,0,sizeof(systime_1));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(one.c_str(), "%d-%d-%d%d:%d:%d:%d.%d", 
                            &systime_1.wYear, 
                            &systime_1.wMonth,
                            &systime_1.wDay,
                            &systime_1.wHour,
                            &systime_1.wMinute,
                            &systime_1.wSecond,
                            &systime_1.wMilliseconds);

    SystemTimeToFileTime(&systime_1, &creationTime);

    memset(&systime_2,0,sizeof(systime_2));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(two.c_str(), "%d-%d-%d%d:%d:%d:%d.%d", 
                            &systime_2.wYear, 
                            &systime_2.wMonth,
                            &systime_2.wDay,
                            &systime_2.wHour,
                            &systime_2.wMinute,
                            &systime_2.wSecond,
                            &systime_2.wMilliseconds);

    SystemTimeToFileTime(&systime_2, &lastWriteTime);

    memset(&systime_3,0,sizeof(systime_3));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(three.c_str(), "%d-%d-%d%d:%d:%d:%d.%d", 
                            &systime_3.wYear, 
                            &systime_3.wMonth,
                            &systime_3.wDay,
                            &systime_3.wHour,
                            &systime_3.wMinute,
                            &systime_3.wSecond,
                            &systime_3.wMilliseconds);

    SystemTimeToFileTime(&systime_3, &lastAccessTime);

    if(SetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
    {
        cout << "Time changed!" << endl;
    }
    else
    {
        cout << "Time hasn't been changed!" << endl;
    }

    CloseHandle(hFile);
}
