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
sqrt = 2

:loop
i = i + 1
if i > n then goto :end
goto :getsqrt

:isprime
j = 2
:isprimeloop
if j == sqrt then print i
if j == sqrt then goto :loop
remainder = i % j
if remainder == 0 then goto :loop
j = j + 1
goto :isprimeloop

:getsqrt
k = 1
:getsqrtloop
square = k ** 2
if square > i then sqrt = k
if square > i then goto :isprime
k = k + 1
goto :getsqrtloop

:end
```

```
./leather prime.milk
gcc -c prime.s -o prime.o
ld prime.o -o prime
complies to...
```

```assembly
.section .text
.globl _start
_start:
   mov %rsp, %rbp
   sub $56, %rsp
   sub $24, %rsp
   mov %rbp, %rsi
   addq $56, %rsi
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
   movq $1, %rax
   movq %rax, -16(%rbp)
   movq $2, %rax
   movq %rax, -24(%rbp)
loop:
   movq -16(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -16(%rbp)
   movq -16(%rbp), %rax
   mov %rax, %rcx
   movq -8(%rbp), %rax
   cmpq %rax, %rcx
   setg %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF0
   jmp end
.ENDIF0:
   jmp getsqrt
isprime:
   movq $2, %rax
   movq %rax, -32(%rbp)
isprimeloop:
   movq -32(%rbp), %rax
   mov %rax, %rcx
   movq -24(%rbp), %rax
   cmpq %rcx, %rax
   sete %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF1
   movq -16(%rbp), %rax
   movq $1, %rsi
   sub $24, %rsp
   mov %rbp, %rcx
   addq $79, %rcx
   movq $0x0A, (%rcx)
   dec %rcx
   call int_to_ascii
   movq $1, %rax
   movq $1, %rdi
   syscall
.ENDIF1:
   movq -32(%rbp), %rax
   mov %rax, %rcx
   movq -24(%rbp), %rax
   cmpq %rcx, %rax
   sete %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF2
   jmp loop
.ENDIF2:
   movq -32(%rbp), %rax
   mov %rax, %rcx
   movq -16(%rbp), %rax
   xor %rdx, %rdx
   div %rcx
   mov %rdx, %rax
   movq %rax, -40(%rbp)
   movq -40(%rbp), %rax
   mov %rax, %rcx
   movq $0, %rax
   cmpq %rcx, %rax
   sete %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF3
   jmp loop
.ENDIF3:
   movq -32(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -32(%rbp)
   jmp isprimeloop
getsqrt:
   movq $1, %rax
   movq %rax, -48(%rbp)
getsqrtloop:
   movq -48(%rbp), %rax
   mov %rax, %rcx
   movq $2, %rax
   mov %rax, %rbx
   movq $1, %rax
   call exp
   movq %rax, -56(%rbp)
   movq -56(%rbp), %rax
   mov %rax, %rcx
   movq -16(%rbp), %rax
   cmpq %rax, %rcx
   setg %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF4
   movq -48(%rbp), %rax
   movq %rax, -24(%rbp)
.ENDIF4:
   movq -56(%rbp), %rax
   mov %rax, %rcx
   movq -16(%rbp), %rax
   cmpq %rax, %rcx
   setg %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF5
   jmp isprime
.ENDIF5:
   movq -48(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -48(%rbp)
   jmp getsqrtloop
end:
.exit:
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

exp:
   test %rbx, %rbx
   jz .endexp
   mul %rcx
   dec %rbx
   jmp exp
.endexp:
   ret

```

```
After assembling, linking, and executing, stdout reads:

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