.586

.MODEL FLAT, STDCALL
includelib kernel32.lib
includelib libucrt.lib
includelib D:\ExtendedData\Laboratory\ßÏ\17\LP_asm01\Debug\LP_asm01d.lib

SetConsoleTitleA PROTO : DWORD
GetStdHandle PROTO: DWORD
WriteConsoleA PROTO: DWORD, : DWORD, : DWORD, : DWORD, : DWORD
ExitProcess PROTO : DWORD

EXTRN getmin : PROC
EXTRN getmax : PROC

.STACK 8192

.CONST

consoleTitle byte 'LP_asm01', 0
str1 byte 'getmax-getmin=', 0
array dword 12, 51, 6, 89, 4, 7, 13, 10, 78, 123

.DATA

consoleHandle dword 0h
max sdword ?
min sdword ?
result sdword ?
resultString byte 4 dup(0)

.CODE

int_to_char PROC uses eax ebx ecx edi esi, pstr: dword, intfield: sdword
    mov edi, pstr
    mov esi, 0
    mov eax, intfield										
    cdq
    mov ebx, 10
    idiv ebx	
    test eax, 80000000h											
    jz plus														
    neg eax
    neg edx
    mov cl, '-'
    mov[edi], cl
    inc edi														
plus:
    push dx
    inc esi
    cmp eax, 0 ;test eax, eax												
    jz fin
    cdq
    idiv ebx
    jmp plus
fin:
    mov ecx ,esi
write:
    pop bx														
    add bl,'0'
    mov[edi], bl
    inc edi
    loop write
    ret
int_to_char ENDP

main PROC
	    INVOKE SetConsoleTitleA, offset consoleTitle		
	    INVOKE GetStdHandle, -11		       
	mov consoleHandle, eax						        
    push 10
    push offset array
        call getmin
    mov min, eax
    push 10
    push offset array
        call getmax
    mov max, eax
	sub eax, min
	mov result, eax
	    INVOKE WriteConsoleA, consoleHandle, offset str1, sizeof str1, 0, 0
	    INVOKE int_to_char, offset resultString, result
	    INVOKE WriteConsoleA, consoleHandle, offset resultString, sizeof resultString, 0, 0
	    INVOKE ExitProcess,0
main ENDP
end main