.686
.model flat, stdcall
option casemap :none
include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\masm32.inc
include \masm32\include\user32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\masm32.lib
includelib \masm32\lib\user32.lib
includelib \masm32\lib\msvcrt.lib

.DATA
;-------------------------------------------------------------------------
;------User's data--------------------------------------------------------
;-------------------------------------------------------------------------
	a	dd	00h
	b	dd	00h
	c1	dd	00h

;-------------------------------------------------------------------------
;------Addition Data------------------------------------------------------
;-------------------------------------------------------------------------
	hConsoleInput	dd	?
	hConsoleOutput	dd	?
	ErrorMessage	db	"Error: division by zero", 0
	InputBuf	db	15 dup (?)
	InMessage	db	"GET: ", 0
	OutMessage	db	"PUT: "," %d",0
	ResMessage	db	20 dup (?)
	NumberOfCharsRead	dd	?
	NumberOfCharsWrite	dd	?
	msg1310	dw	13,10
	buf	dd	0
	lb1	dd	?
	lb2	dd	?

.CODE
start:
invoke AllocConsole
invoke GetStdHandle, STD_INPUT_HANDLE
mov hConsoleInput, EAX
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hConsoleOutput, EAX
	call Input
	push buf
	pop a
	call Input
	push buf
	pop b
	call Input
	push buf
	pop c1
	push a
	push b
	call Eq_
	push b
	push c1
	call Eq_
	call And_
	push a
	push c1
	call Eq_
	call And_
	pop buf
	cmp dword ptr buf, 0
	je ifLabel1
	mov dword ptr buf, 1
	push buf
	pop buf
	call Output
	jmp endIf1
ifLabel1:
	mov dword ptr buf, 0
	push buf
	pop buf
	call Output
endIf1:
	push a
	mov dword ptr buf, 0
	push buf
	call Less_
	push b
	mov dword ptr buf, 0
	push buf
	call Less_
	call Or_
	push c1
	mov dword ptr buf, 0
	push buf
	call Less_
	call Or_
	pop buf
	cmp dword ptr buf, 0
	je ifLabel2
	mov dword ptr buf, 1
	push buf
	pop buf
	neg buf
	push buf
	pop buf
	call Output
	jmp endIf2
ifLabel2:
	mov dword ptr buf, 0
	push buf
	pop buf
	call Output
endIf2:
	push a
	push b
	push c1
	pop edx
	pop eax
	add eax,edx
	push eax
	call Less_
	call Not_
	pop buf
	cmp dword ptr buf, 0
	je ifLabel3
	mov dword ptr buf, 10
	push buf
	pop buf
	call Output
	jmp endIf3
ifLabel3:
	mov dword ptr buf, 0
	push buf
	pop buf
	call Output
endIf3:
exit_label:
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,1,ADDR NumberOfCharsRead,0
invoke ExitProcess, 0

;-------------------------------------------------------------------------
;------Procedure "Input"------------------------------------------------
;-------------------------------------------------------------------------
Input PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke WriteConsoleA,hConsoleOutput,ADDR InMessage,SIZEOF InMessage,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,11,ADDR NumberOfCharsRead,0
invoke  crt_atoi, addr InputBuf
mov dword ptr buf, eax
ret
Input ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------Procedure "Output"-----------------------------------------------
;-------------------------------------------------------------------------
Output PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke wsprintf, addr ResMessage, addr OutMessage, dword ptr buf
invoke WriteConsoleA,hConsoleOutput,ADDR ResMessage,eax,ADDR NumberOfCharsWrite,0
ret
Output ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------Procedure "And"--------------------------------------------------
;-------------------------------------------------------------------------
And_ PROC
	pop ebx
	pop lb1
	pop lb2
	mov eax,lb1
	cmp eax,0
	jnz and_t1
	jz and_false
and_t1:
	mov eax,lb2
	cmp eax,0
	jnz and_true
and_false:
	push 0
	jmp and_fin
and_true:
	push 1
and_fin:

	push ebx
	ret
And_ ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------Procedure "Or"---------------------------------------------------
;-------------------------------------------------------------------------
Or_ PROC
	pop ebx
	pop lb1
	pop lb2
	mov eax,lb1
	cmp eax,0
	jnz or_true
	mov eax,lb2
	cmp eax,0
	jnz or_true
	push 0
	jmp or_fin
or_true:
	push 1
or_fin:

	push ebx
	ret
Or_ ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------Procedure "Not"--------------------------------------------------
;-------------------------------------------------------------------------
Not_ PROC
	pop ebx
	pop lb1
	mov eax,lb1
	cmp eax,0
	jne not_false
	push 1
	jmp not_fin
not_false:
	push 0
not_fin:

	push ebx
	ret
Not_ ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------Procedure "Equal"------------------------------------------------
;-------------------------------------------------------------------------
Eq_ PROC
	pop ebx
	pop lb2
	pop lb1
	mov eax,lb1
	mov edx,lb2
	cmp eax,edx
	jne not_eq
	push 1
	jmp eq_fin
not_eq:
	push 0
eq_fin:
	push ebx
	ret
Eq_ ENDP

;-------------------------------------------------------------------------


;-------------------------------------------------------------------------
;------Procedure "Less"-------------------------------------------------
;-------------------------------------------------------------------------
Less_ PROC
	pop ebx
	pop lb2
	pop lb1
	mov eax, lb1
	mov edx, lb2
	cmp eax,edx
	jge gr
	push 1
	jmp less_fin
gr:
	push 0
less_fin:
	push ebx
	ret
Less_ ENDP

;-------------------------------------------------------------------------

end start
