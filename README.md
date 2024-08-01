# The Cow Language

## Still in development...
### Compiles .milk files into AT&T syntax x86-64 Assembly code which can be executed on Linux machines.

```
example.milk:
```
```
a = 1
:loop
print a
a = a + 1
if a < 101 then
goto :loop
```

```
-> example.s
```

```assembly
.section .text
.globl _start
_start:
   mov %rsp, %rbp
   sub $8, %rsp
   movq $1, %rax
   movq %rax, -8(%rbp)
loop:
   movq -8(%rbp), %rax
   movq $1, %rsi
   sub $24, %rsp
   mov %rbp, %rcx
   addq $31, %rcx
   movq $0x0A, (%rcx)
   dec %rcx
   call int_to_ascii
   movq $1, %rax
   movq $1, %rdi
   syscall
   movq -8(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -8(%rbp)
   movq -8(%rbp), %rax
   mov %rax, %rcx
   movq $101, %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF0
   jmp loop
.ENDIF0:
   movq $60, %rax
   xor %rdi, %rdi
   syscall

int_to_ascii:
   movq $10, %rbx
   xor %rdx, %rdx
   div %rbx
   add $48, %dl
   movb %dl, (%rcx)
   inc %rsi
   dec %rcx
   test %rax, %rax
   jnz int_to_ascii
   inc %rcx
   mov %rsi, %rdx
   mov %rcx, %rsi
   ret

```

Special thanks to [Alex](https://github.com/alexjercan) and his [YouTube tutorial](https://youtu.be/HOe2YFnzO2I?si=u_QlAn2AdSL3jNtR) for a lot of guidance on this project. Go check him out :)