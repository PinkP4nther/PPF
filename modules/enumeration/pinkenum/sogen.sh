gcc -g -c -fPIC -Wall pinkenum.c
gcc -g -shared -o pinkenum.so pinkenum.o
rm pinkenum.o
