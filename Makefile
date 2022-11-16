# Gzip tar file
# All source files .c and .h
# program MUST be named "primesMT"


# Linking the part 2 program
# rebuilds primesMT.o if necessary
primesMT: primesMT.o
	gcc -Wall -Wshadow -Wunreachable-code -Wredundant-decls -Wmissing-declarations -Wold-style-definition -Wmissing-prototypes -Wdeclaration-after-statement -Wno-return-local-addr -Wuninitialized -Werror -Wunsafe-loop-optimizations sieve.c -o primesMT -lm -pthread


# Should build all out-of-date programs and pre-reqs
all: primesMT.o primesMT


# builds primesMT.o if necessary
primesMT.o: sieve.c
	gcc -c sieve.c


# Clean up compiled files and editor chaff
clean:
	rm -f *.o primesMT

# Does what clean does AND removes all the .gz and logs from the bash-script
super:
	rm -f *.o TestPrimes* coreDump.log primesMT

