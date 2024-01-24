# DirectCallsCaller

This project is created as a completed example for using Direct Syscalls for running the below function:
-	NtOpenProcess
-	NtAllocateVirtualMemory
-	NtWriteVirtualMemory
-	NtCreateThreadEx
-	NtWaitForSingleObject

It will open the process with the provided process ID, then it will allocate a memeory space inside it to be use to write the shellcode using NtWriteVirtualMemory then it will create thread via NtCrerateThreadEx to run the shellcode. The shellcode inside the project will just fire a MessageBox.

# Usage
choose any running process and get the Process ID of it and give it to caller.
```
.\DirectCallsCaller <ProcessID>
```
![image](https://github.com/kmohamed2020/DirectCallsCaller/assets/14153248/e7f2e894-2d75-4d92-bc81-8693c2892f8f)

# Notes
The allocated memory has EXECUTE_READWRITE, i was little bit lazy to make it READWRITE then flip it into EXECUTE_READ, you can do it if you would like.
The project is developed to work on Windows 11 based on the SSNs for the related syscalls.

It's developed for Educational Purposes only.

# References
-  https://www.crow.rip/crows-nest/mal/dev/inject/syscalls/direct-syscalls
-  https://j00ru.vexillium.org/syscalls/nt/64/
