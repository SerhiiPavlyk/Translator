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
	i	dd	00h
	x	dd	00h
	j	dd	00h

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
	push a
	pop i
forStart1:
	push b
	pop buf
	push i
	push buf
	call Less_
	pop buf
	mov eax,dword ptr buf
	cmp eax,0
	jz forFinish1
	push i
	push i
	pop edx
	pop eax
	imul edx
	push eax
	pop buf
	call Output
	mov eax, dword ptr i
	inc eax
	mov dword ptr i, eax
	jmp forStart1
forFinish1:
	mov dword ptr buf, 0
	push buf
	pop x
	mov dword ptr buf, 1
	push buf
	pop i
forStart2:
	push a
	pop buf
	push i
	push buf
	call Less_
	pop buf
	mov eax,dword ptr buf
	cmp eax,0
	jz forFinish2
	mov dword ptr buf, 1
	push buf
	pop j
forStart3:
	push b
	pop buf
	push j
	push buf
	call Less_
	pop buf
	mov eax,dword ptr buf
	cmp eax,0
	jz forFinish3
	push x
	mov dword ptr buf, 1
	push buf
	pop edx
	pop eax
	add eax,edx
	push eax
	pop x
	mov eax, dword ptr j
	inc eax
	mov dword ptr j, eax
	jmp forStart3
forFinish3:
	mov eax, dword ptr i
	inc eax
	mov dword ptr i, eax
	jmp forStart2
forFinish2:
	push x
	pop buf
	call Output
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
