gcc -g -c -fPIC -Wall shell.c
gcc -g -shared -o shell.so shell.o
