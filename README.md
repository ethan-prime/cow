# The Cow Language

## Still in development...
### Compiles .milk files into AT&T syntax x86-64 Assembly code which can be executed on Linux machines.

# Example: 
```
fib.milk: computes the first n fibonacci numbers
```

```c
int! n = input

int... fib[n]

fib[0] = 1
fib[1] = 1

for int! i = 2; i < n; i = i + 1 do {
    fib[i] = fib[i - 1] + fib[i - 2]
}

for int! j = 0; j < n; j = j + 1 do {
    moo fib[j]
}
```

```
$ ./leather sqrt_newtons_method.milk
[leather] successfully compiled:
    sqrt_newtons_method.milk -> sqrt_newtons_method.s
```

```assembly
.section .text
.globl _start
_start:
   mov %rsp, %rbp
   sub $48, %rsp
   sub $128, %rsp
   mov %rbp, %rsi
   addq $48, %rsi
   movq $0, %rax
   movq $0, %rdi
   movq $64, %rdx
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
   movq -8(%rbp), %rdi
   xor %r12, %r12
   lea (%r12, %rdi, 8), %rdi
   call heapalloc
   movq %rax, -16(%rbp)
   movq $1, %rax
   movq -16(%rbp), %rdx
   movq $0, %rbx
   movq %rax, (%rdx, %rbx, 8)
   movq $1, %rax
   movq -16(%rbp), %rdx
   movq $1, %rbx
   movq %rax, (%rdx, %rbx, 8)
   movq $2, %rax
   movq %rax, -24(%rbp)
.STARTLOOP0:
   movq -24(%rbp), %rax
   mov %rax, %rcx
   movq -8(%rbp), %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDLOOP0
   movq $1, %rax
   mov %rax, %rcx
   movq -24(%rbp), %rax
   subq %rcx, %rax
   movq %rax, -32(%rbp)
   movq $2, %rax
   mov %rax, %rcx
   movq -24(%rbp), %rax
   subq %rcx, %rax
   movq %rax, -40(%rbp)
   movq -16(%rbp), %rdx
   movq -32(%rbp), %rbx
   movq (%rdx, %rbx, 8), %rax
   mov %rax, %rcx
   movq -16(%rbp), %rdx
   movq -40(%rbp), %rbx
   movq (%rdx, %rbx, 8), %rax
   addq %rcx, %rax
   movq -16(%rbp), %rdx
   movq -24(%rbp), %rbx
   movq %rax, (%rdx, %rbx, 8)
   movq -24(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -24(%rbp)
   jmp .STARTLOOP0
.ENDLOOP0:
   movq $0, %rax
   movq %rax, -48(%rbp)
.STARTLOOP1:
   movq -48(%rbp), %rax
   mov %rax, %rcx
   movq -8(%rbp), %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDLOOP1
   movq -16(%rbp), %rdx
   movq -48(%rbp), %rbx
   movq (%rdx, %rbx, 8), %rax
   movq $1, %rsi
   mov %rbp, %rcx
   addq $111, %rcx
   movq $0x0A, (%rcx)
   dec %rcx
   call int_to_ascii
   movq $1, %rax
   movq $1, %rdi
   syscall
   movq -48(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -48(%rbp)
   jmp .STARTLOOP1
.ENDLOOP1:
.exit:
   movq $60, %rax
   xor %rdi, %rdi
   syscall

int_to_ascii:
   xor %r10, %r10
   test %rax, %rax
   jns .convert_positive
   movq $1, %r10
   neg %rax
.convert_positive:
   movq $10, %rbx
   xor %rdx, %rdx
   div %rbx
   add $48, %dl
   movb %dl, (%rcx)
   inc %rsi
   dec %rcx
   test %rax, %rax
   jnz .convert_positive
   test %r10, %r10
   jz .end_itoa
   movb $'-', (%rcx)
   dec %rcx
   inc %rsi
.end_itoa:
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

double_to_ascii:
   xor %r12, %r12
   cvttsd2si %xmm0, %r9
   test %r9, %r9
   jns .convert_positive_dta
   movq $1, %r12
   movabs $0x8000000000000000, %rax
   movq %rax, %xmm2
   xorpd %xmm2, %xmm0
   cvttsd2si %xmm0, %r9
.convert_positive_dta:
   movq $10000000000, %rax
   cvtsi2sd %rax, %xmm1
   mulsd %xmm1, %xmm0
   cvttsd2si %xmm0, %rax
   movq $10000000000, %r8
   xor %rdx, %rdx
   div %r8
   mov %rdx, %rax
.double_to_ascii_loop:
   mov %rax, %rbx
   test %rax, %rax
   jz .double_to_ascii_loop_end
   xor %rdx, %rdx
   movq $10, %r8
   div %r8
   test %rdx, %rdx
   jz .double_to_ascii_loop
.double_to_ascii_loop_end:
   mov %rbx, %rax
   call int_to_ascii
   mov %rsi, %rcx
   mov %rdx, %rsi
   dec %rcx
   movb $46, (%rcx)
   inc %rsi
   dec %rcx
   mov %r9, %rax
   call int_to_ascii
   test %r12, %r12
   jz .end_dtoa
   dec %rsi
   movb $'-', (%rsi)
   inc %rdx
.end_dtoa:
   ret

heapalloc:
   mov %rdi, %rsi
   movq $0, %rdi
   movq $0x3, %rdx
   movq $0x22, %r10
   movq $-1, %r8
   xor %r9, %r9
   movq $9, %rax
   syscall
   ret

```

```
$ gcc -c -g fib.s -o fib.o
$ ld fib.o -o fib
$ ./fib
```

```
stdout:
20
1
1
2
3
5
8
13
21
34
55
89
144
233
377
610
987
1597
2584
4181
6765
```