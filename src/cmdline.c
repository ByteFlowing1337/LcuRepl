// https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess
#include <windows.h>
#include <winternl.h>
#include "cmdline.h"
#include "pid.h"

char *get_cmdline()
{
    DWORD pid = get_client_pid();
    if (pid == 0)
        return NULL;

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProcess)
        return NULL;

    ULONG bufLen = 0;

    // First call to get bufLen
    NtQueryInformationProcess(hProcess, ProcessCommandLineInformation, NULL, 0, &bufLen);
    if (bufLen == 0)
    {
        CloseHandle(hProcess);
        return NULL;
    }

    PUNICODE_STRING pUnicodeCmdLine = (PUNICODE_STRING)malloc(bufLen);
    if (!pUnicodeCmdLine)
    {
        CloseHandle(hProcess);
        return NULL;
    }

    // Second call to get cmdline info
    NTSTATUS status = NtQueryInformationProcess(
        hProcess,
        ProcessCommandLineInformation,
        pUnicodeCmdLine,
        bufLen,
        &bufLen);

    CloseHandle(hProcess);

    if (status != 0 || pUnicodeCmdLine->Buffer == NULL)
    {
        free(pUnicodeCmdLine);
        return NULL;
    }

    // First call to query buffer size
    int charCount = WideCharToMultiByte(
        CP_UTF8,
        0,
        pUnicodeCmdLine->Buffer,
        pUnicodeCmdLine->Length / sizeof(WCHAR),
        NULL,
        0,
        NULL,
        NULL);

    char *resultStr = (char *)malloc(charCount + 1);
    if (resultStr)
    {
        // Second call to perform conversion
        WideCharToMultiByte(
            CP_UTF8,
            0,
            pUnicodeCmdLine->Buffer,
            pUnicodeCmdLine->Length / sizeof(WCHAR),
            resultStr,
            charCount,
            NULL,
            NULL);
        resultStr[charCount] = '\0';
    }

    free(pUnicodeCmdLine);
    return resultStr;
}
