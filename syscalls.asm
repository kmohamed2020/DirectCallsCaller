.code
	NtOpenProcess proc
		mov r10, rcx
		mov eax, 26h
		syscall
		ret
	NtOpenProcess endp

	NtAllocateVirtualMemory proc
		mov r10, rcx
		mov eax, 18h
		syscall
		ret
	NtAllocateVirtualMemory endp

	NtWriteVirtualMemory proc
		mov r10, rcx
		mov eax, 3Ah
		syscall
		ret
	NtWriteVirtualMemory endp

	NtCreateThreadEx PROC
			mov r10, rcx
			mov eax, 0C5h
			syscall
			ret
	NtCreateThreadEx ENDP

	NtWaitForSingleObject PROC
			mov r10, rcx
			mov eax, 4h
			syscall
			ret
	NtWaitForSingleObject ENDP

	NtClose PROC
			mov r10, rcx
			mov eax, 0Fh
			syscall
			ret
	NtClose ENDP
end