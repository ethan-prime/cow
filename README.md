# The Cow Language

## Still in development...
### Compiles .milk files into AT&T syntax x86-64 Assembly code which can be executed on Linux machines.

# Examples: 
```
fib.milk: gets the nth fibonacci number from input
```
```c
// get user input
moo "Nth fibonacci number (0-indexed lol):"
int! num = input

// call fib function and MOO result!
moo #fib(num)

define fib: int! n -> int! {
    // base case
    int! ret = 1
    if n > 0 then { 
        // recursive case
        ret = #fib(n - 1) + #fib(n - 1)
    }
    // return result
    -> ret
}
```
```
$ ./leather fib.milk
[leather] successfully compiled:
    fib.milk -> fib.s
```
```
$ gcc -c -g fib.s -o fib.o
$ ld fib.o -o fib
$ ./fib
```
```
20
10946
```

```
prime.milk: retrieves the nth prime number (given by user input)
```
```c
moo "Hello welcome to the prime number generating program!"
moo "Please enter the nth prime number that you want:"

int! n = input

int! cur_prime_n = 0
int! cur_num = 2

// loop until we have the nth prime.
while cur_prime_n < n do {
    while #is_prime == false do { // call is_prime to see if cur_num is prime
        cur_num = cur_num + 1
    }
    cur_prime_n = cur_prime_n + 1
    cur_num = cur_num + 1
}

moo ""
moo "The nth prime number is:"
cur_num = cur_num - 1 // we added an extra 1, we have to subtract
moo cur_num // MOO result!


// FUNCTION DECLARATIONS

// gets ceil(sqrt) of a cur_num
define get_sqrt -> int {
    int! sqrt = 1
    int! squared = 1
    while ! squared > cur_num do {
        sqrt = sqrt + 1
        squared = sqrt ** 2
    }
    -> sqrt
}

// returns whether cur_num is prime or not.
define is_prime -> bool {
    bool! res = true
    for int! i = 2; i < #get_sqrt; i = i + 1 do {
        if i == cur_num then {
            break
        }
        int! remainder = cur_num % i
        if remainder == 0 then {
            res = false
        }
    }
    -> res
}
```
```
$ ./leather prime.milk
[leather] successfully compiled:
    prime.milk -> prime.s
```
```
$ gcc -c -g prime.s -o prime.o
$ ld prime.o -o prime
$ ./prime
```
```
Hello welcome to the prime-number geneting program!
Please enter the nth prime number that you want:
10000

The nth prime number is:
104729
```

```
rps.milk: play rock paper scissors against the computer! (rng)
```
```c
print "Hello. Welcome to Rock, Paper, Scissors!"

int! choice
int! computer_choice

while choice != 4 do {
    print " - 1 for Rock"
    print " - 2 for Paper"
    print " - 3 for Scissors"
    print " - 4 to exit"
    
    choice = input

    if choice == 4 then {
        break
    }

    computer_choice = random % 3
    computer_choice = computer_choice + 1
    
    if computer_choice == 1 then {
        print "The computer chooses Rock"
    }
    if computer_choice == 2 then {
        print "The computer chooses Paper"
    }
    if computer_choice == 3 then {
        print "The computer chooses Scissors"
    }

    if choice == 1 then {
        if computer_choice == 1 then {
            print "You tie."
        }
        if computer_choice == 2 then {
            print "You lose!"
        }
        if computer_choice == 3 then {
            print "You win!"
        }
    }

    if choice == 2 then {
        if computer_choice == 1 then {
            print "You win!"
        }
        if computer_choice == 2 then {
            print "You tie."
        }
        if computer_choice == 3 then {
            print "You lose!"
        }
    }

    if choice == 3 then {
        if computer_choice == 1 then {
            print "You lose!"
        }
        if computer_choice == 2 then {
            print "You win!"
        }
        if computer_choice == 3 then {
            print "You tie."
        }
    }
}
```
```
$ ./leather rps.milk
[leather] successfully compiled:
    rps.milk -> rps.s
```
```
$ gcc -c -g rps.s -o rps.o
$ ld rps.o -o rps
$ ./rps
```
```
Hello. Welcome to Rock, Paper, Scissors!
 - 1 for Rock
 - 2 for Paper
 - 3 for Scissors
 - 4 to exit
1
The computer chooses Paper
You lose!
 - 1 for Rock
 - 2 for Paper
 - 3 for Scissors
 - 4 to exit
1
The computer chooses Scissors
You win!
 - 1 for Rock
 - 2 for Paper
 - 3 for Scissors
 - 4 to exit
2
The computer chooses Scissors
You lose!
 - 1 for Rock
 - 2 for Paper
 - 3 for Scissors
 - 4 to exit
3
The computer chooses Rock
You lose!
 - 1 for Rock
 - 2 for Paper
 - 3 for Scissors
 - 4 to exit
4
```