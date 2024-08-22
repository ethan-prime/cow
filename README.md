# The Cow Language

## Still in development...
### Compiles .milk files into AT&T syntax x86-64 Assembly code which can be executed on Linux machines.

# Examples: 
```
fib.milk: gets the nth fibonacci number from input
```
```c
moo "nth fibonacci number:"
// get user input
int! num = input

// call fib function and MOO result!
moo #fib(num)

define fib: int! n -> int! {
    if n == 0 then {
        -> 1
    }
    if n == 1 then {
        -> 1
    }
    -> #fib(n - 1) + #fib(n - 2)
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
nth fibonacci number (0-indexed lol):
20
10946
```

```
prime.milk: retrieves the nth prime number (given by user input)
```
```c
moo "Enter the nth prime number you want:"
int! n = input // get user input

int! count = 0
int! i = 2

// loop until we get the nth prime
while count < n do {
    while #is_prime(i) == false do {
        i = i + 1
    }
    i = i + 1
    count = count + 1
} 

i = i - 1
moo "The nth prime is:"
moo i // we found the nth prime!


// returns ceil(sqrt) + 1 of a number for the prime checking function.
define sqrt: int! num -> int! {
    int! sqrt = 1
    int! squared = 1
    while squared < num do {
        sqrt = sqrt + 1
        squared = sqrt ** 2
    }
    -> sqrt + 1
}

// returns true if a number is prime, false otherwise.
define is_prime: int! num -> bool! {
    if num == 2 then { -> true } // 2 can give us headaches bc sqrt(2) -> 2.
    for int! i = 2; i < #sqrt(num); i = i + 1 do {
        int! remainder = num % i
        if remainder == 0 then {
            -> false
        }
    }
    -> true
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