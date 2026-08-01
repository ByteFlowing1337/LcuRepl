// Partially copied from https://learn.microsoft.com/en-us/windows/win32/toolhelp/taking-a-snapshot-and-viewing-processes
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include "pid.h"

static void printError(TCHAR const *msg);

DWORD get_client_pid()
{
    HANDLE hProcessSnap;
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;

    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
        return (FALSE);
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        printError(TEXT("Process32First")); // show cause of failure
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return (FALSE);
    }

    // Now walk the snapshot of processes,
    // return the PID of 'LeagueClientUx.exe'
    do
    {
        // Retrieve the priority class.
        dwPriorityClass = 0;
        hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe32.th32ProcessID);
        if (hProcess == NULL)
            continue;
        else
        {
            dwPriorityClass = GetPriorityClass(hProcess);
            if (!dwPriorityClass)
                printError(TEXT("GetPriorityClass"));
            CloseHandle(hProcess);
        }

        if (_stricmp(pe32.szExeFile, "LeagueClientUx.exe") == 0)
        {
            goto success;
        }

    } while (Process32Next(hProcessSnap, &pe32));
    printError(TEXT("Unable to find LeagueClientUx.exe."));
    CloseHandle(hProcessSnap);
    return 0;

success:
    CloseHandle(hProcessSnap);
    return pe32.th32ProcessID;
}

static void printError(TCHAR const *msg)
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR *p;

    eNum = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL, eNum,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  sysMsg, 256, NULL);

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while ((*p > 31) || (*p == 9))
        ++p;
    do
    {
        *p-- = 0;
    } while ((p >= sysMsg) &&
             ((*p == '.') || (*p < 33)));

    // Display the message
    _tprintf(TEXT("\n  WARNING: %s failed with error %ld (%s)"), msg, eNum, sysMsg);
}