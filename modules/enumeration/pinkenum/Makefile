CC		= gcc
CFLAGS		= -g -c -fPIC -Wall
LDFLAGS		= -g -shared
SRCFILES	= pinkenum.c
OBJFILES	= pinkenum.o
TARGET		= pinkenum.so
SONAME		= pinkenum.so

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(SRCFILES)
	$(CC) $(LDFLAGS) -o $(SONAME) $(OBJFILES)
	rm -rf $(OBJFILES)

clean:
	rm -rf $(SONAME)
