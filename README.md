# The Cow Language

## Still in development...
### Compiles .milk files into AT&T syntax x86-64 Assembly code which can be executed on Linux machines.

# Example: 
```
prime.milk: prints all prime numbers up to n which is given as input.
```
```c
n = input
i = 1

:loop
i = i + 1
if i > n then goto :end
goto :isprime

:isprime
j = 2
:isprimeloop
if j == i then print i
if j == i then goto :loop
remainder = i % j
if remainder == 0 then goto :loop
j = j + 1
goto :isprimeloop

:end
```

```
./leather prime.milk
complies to...
```

```assembly
.section .text
.globl _start
_start:
   mov %rsp, %rbp
   sub $16, %rsp
   sub $24, %rsp
   mov %rbp, %rsi
   addq $16, %rsi
   movq $0, %rax
   movq $0, %rdi
   movq $24, %rdx
   syscall
   mov %rax, %rcx
   subq $1, %rcx
   mov %rsi, %r9
   addq %rcx, %r9
   subq $1, %r9
   movq $1, %rbx
   xor %r8, %r8
   call ascii_to_int
   movq %rax, -8(%rbp)
   movq -8(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -8(%rbp)
   movq $1, %rax
   movq %rax, -16(%rbp)
loop:
   movq -16(%rbp), %rax
   movq $1, %rsi
   sub $24, %rsp
   mov %rbp, %rcx
   addq $39, %rcx
   movq $0x0A, (%rcx)
   dec %rcx
   call int_to_ascii
   movq $1, %rax
   movq $1, %rdi
   syscall
   movq -16(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -16(%rbp)
   movq -16(%rbp), %rax
   mov %rax, %rcx
   movq -8(%rbp), %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF0
   jmp loop
.ENDIF0:
   movq $420, %rax
   movq $1, %rsi
   sub $24, %rsp
   mov %rbp, %rcx
   addq $63, %rcx
   movq $0x0A, (%rcx)
   dec %rcx
   call int_to_ascii
   movq $1, %rax
   movq $1, %rdi
   syscall
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

ascii_to_int:
   movb (%r9), %al
   movzbq %al, %rax
   sub $48, %rax
   mul %rbx
   add %rax, %r8
   mov %rbx, %rax
   mov $10, %rbx
   mul %rbx
   mov %rax, %rbx
   dec %rcx
   dec %r9
   test %rcx, %rcx
   jnz ascii_to_int
   inc %r9
   mov %r8, %rax
   ret
```

```
After linking and excuting, stdout reads:

100
2
3
5
7
11
13
17
19
23
29
31
37
41
43
47
53
59
61
67
71
73
79
83
89
97
```

Special thanks to [Alex](https://github.com/alexjercan) and his [YouTube tutorial](https://youtu.be/HOe2YFnzO2I?si=u_QlAn2AdSL3jNtR) for a lot of guidance on this project. Go check him out :)