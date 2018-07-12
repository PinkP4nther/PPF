CC		= gcc
CFLAGS		= -g -c -fPIC -Wall
LDFLAGS		= -g -shared
SRCFILES	= shell.c
OBJFILES	= shell.o
TARGET		= shell.so
SONAME		= shell.so

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(SRCFILES)
	$(CC) $(LDFLAGS) -o $(SONAME) $(OBJFILES)
	rm -rf $(OBJFILES)

clean:
	rm -rf $(SONAME)
