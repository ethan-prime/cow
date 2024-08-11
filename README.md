# The Cow Language

## Still in development...
### Compiles .milk files into AT&T syntax x86-64 Assembly code which can be executed on Linux machines.

# Example: 
```
prime.milk: prints (moos) all prime numbers up to n which is given as input.
```
```c
n = input
i = 0

while i < n do {
    sqrt = 2
    squared = sqrt ** 2
    while squared < i do {
        sqrt = sqrt + 1
        squared = sqrt ** 2
    }
    factor = 2
    prime = true
    while factor < sqrt do {
        remainder = i % factor
        if remainder == 0 then {
            prime = false
            break
        }
        factor = factor + 1
    }
    if prime == true then {
        moo i
    }
    i = i + 1
}
```

```
sqrt.milk: continuously finds the square root of a given input until the user inputs 0.
```
```c
n = input
while n != 0 do {
    l = 0
    r = n

    while ! l > r do {
        mid = l + r
        mid = mid / 2
        squared = mid ** 2
        if squared == n then {
            break
        }
        if squared > n then {
            r = mid - 1
        }
        if squared < n then {
            l = mid + 1
        }
    }

    if squared > n then {
        mid = mid - 1
    }

    moo mid
    n = input
}
```

```
./leather prime.milk

[leather] successfully compiled:
    prime.milk -> prime.s
```

```assembly
.section .text
.globl _start
_start:
   mov %rsp, %rbp
   sub $56, %rsp
   sub $64, %rsp
   mov %rbp, %rsi
   addq $56, %rsi
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
   movq $0, %rax
   movq %rax, -16(%rbp)
.STARTWHILE0:
   movq -16(%rbp), %rax
   mov %rax, %rcx
   movq -8(%rbp), %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDWHILE0
   movq $2, %rax
   movq %rax, -24(%rbp)
   movq -24(%rbp), %rax
   mov %rax, %rcx
   movq $2, %rax
   mov %rax, %rbx
   movq $1, %rax
   call exp
   movq %rax, -32(%rbp)
.STARTWHILE1:
   movq -32(%rbp), %rax
   mov %rax, %rcx
   movq -16(%rbp), %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDWHILE1
   movq -24(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -24(%rbp)
   movq -24(%rbp), %rax
   mov %rax, %rcx
   movq $2, %rax
   mov %rax, %rbx
   movq $1, %rax
   call exp
   movq %rax, -32(%rbp)
   jmp .STARTWHILE1
.ENDWHILE1:
   movq $2, %rax
   movq %rax, -40(%rbp)
   movq $1, %rax
   movq %rax, -48(%rbp)
.STARTWHILE2:
   movq -40(%rbp), %rax
   mov %rax, %rcx
   movq -24(%rbp), %rax
   cmpq %rax, %rcx
   setl %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDWHILE2
   movq -40(%rbp), %rax
   mov %rax, %rcx
   movq -16(%rbp), %rax
   xor %rdx, %rdx
   div %rcx
   mov %rdx, %rax
   movq %rax, -56(%rbp)
   movq -56(%rbp), %rax
   mov %rax, %rcx
   movq $0, %rax
   cmpq %rcx, %rax
   sete %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF0
   movq $0, %rax
   movq %rax, -48(%rbp)
   jmp .ENDWHILE2
.ENDIF0:
   movq -40(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -40(%rbp)
   jmp .STARTWHILE2
.ENDWHILE2:
   movq -48(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   cmpq %rcx, %rax
   sete %al
   movzbq %al, %rax
   test %rax, %rax
   jz .ENDIF1
   movq -16(%rbp), %rax
   movq $1, %rsi
   mov %rbp, %rcx
   addq $119, %rcx
   movq $0x0A, (%rcx)
   dec %rcx
   call int_to_ascii
   movq $1, %rax
   movq $1, %rdi
   syscall
.ENDIF1:
   movq -16(%rbp), %rax
   mov %rax, %rcx
   movq $1, %rax
   addq %rcx, %rax
   movq %rax, -16(%rbp)
   jmp .STARTWHILE0
.ENDWHILE0:
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
gcc -c prime.s -o prime.o
ld prime.o -o prime
./prime
```

```
100 # input
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