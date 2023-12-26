gcc -c main.c -o build/main.o &&
gcc -c scanner.c -o build/scanner.o &&
gcc -c debug.c -o build/debug.o &&
gcc -o main.exe build/main.o build/scanner.o build/debug.o