//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information. 
//

/*=============================================================
**
** Source: ReadProcessMemory_neg.c
**
** Purpose: Negative test the ReadProcessMemory API.
**          Call ReadProcessMemory to read unreadabel memory area
**
**
**============================================================*/
#include <palsuite.h>

#define REGIONSIZE 1024

int __cdecl main(int argc, char *argv[])
{
    int err;
    BOOL bResult;
    HANDLE ProcessHandle;
    DWORD ProcessID;
    LPVOID lpProcessAddress = NULL;
    char ProcessBuffer[REGIONSIZE];
    ULONG_PTR size = 0;


    /*Initialize the PAL environment*/
    err = PAL_Initialize(argc, argv);
    if(0 != err)
    {
        return FAIL;
    }
    
    /*retrive the current process ID*/    
    ProcessID = GetCurrentProcessId();

    /*retrive the current process handle*/
    ProcessHandle = OpenProcess(
                PROCESS_ALL_ACCESS,
                FALSE,          /*not inherited*/
                ProcessID);
    
    if(NULL == ProcessHandle)
    {
        Fail("\nFailed to call OpenProcess API to retrive "
                "current process handle error code=%u\n",
                GetLastError());
    }


    
    /*allocate the virtual memory*/
    lpProcessAddress = VirtualAlloc(
            NULL,            /*system determine where to allocate the region*/
            REGIONSIZE,      /*specify the size*/
            MEM_RESERVE,     /*allocation type*/
            PAGE_READONLY);  /*access protection*/

    if(NULL == lpProcessAddress)
    {
        Fail("\nFailed to call VirtualAlloc API to allocate "
                "virtual memory, error code=%u\n", GetLastError());
    }

    /*zero the memory*/
    memset(ProcessBuffer, 0, REGIONSIZE);
    /*try to retrieve the unreadable memory area*/
    bResult = ReadProcessMemory(
            ProcessHandle,         /*current process handle*/
            lpProcessAddress,      /*base of memory area*/
            (LPVOID)ProcessBuffer,
            REGIONSIZE,            /*buffer length in bytes*/
            &size);


    /*check the return value*/
    if(0 != bResult)
    {
        Trace("\nFailed to call ReadProcessMemory API for a negative test, "
                "Try to read an unreadable memory area will cause fail "
                "but it successes\n");

        err = CloseHandle(ProcessHandle);
        if(0 == err)
        {
            Trace("\nFailed to call CloseHandle API, error code=%u\n",
                GetLastError());
        }

        /*decommit the specified region*/
        err = VirtualFree(lpProcessAddress, REGIONSIZE, MEM_DECOMMIT);
        if(0 == err)
        {
            Trace("\nFailed to call VirtualFree API, error code=%u\n",
                GetLastError());
        }
        Fail("");
    }

    err = CloseHandle(ProcessHandle);
    if(0 == err)
    {
        Trace("\nFailed to call CloseHandle API, error code = %u\n",
                GetLastError());

        err = VirtualFree(lpProcessAddress, REGIONSIZE, MEM_DECOMMIT);
        if(0 == err)
        {
            Trace("\nFailed to call VirtualFree API, error code=%u\n",
                    GetLastError());
        }

        Fail("");
    }
    /*decommit the specified region*/
    err = VirtualFree(lpProcessAddress, REGIONSIZE, MEM_DECOMMIT);
    if(0 == err)
    {
        Fail("\nFailed to call VirtualFree API, error code=%u\n",
                GetLastError());
    }

    PAL_Terminate();
    return PASS;
}
