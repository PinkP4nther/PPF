CC		= gcc
CFLAGS		= -Wall -g
LDFLAGS		= -ldl
OBJFILES	= source/ppfCmdParse.o source/ppfModLoad.o source/ppfh.o source/ppf.o
TARGET		= ppf

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -rf $(OBJFILES) $(TARGET)
