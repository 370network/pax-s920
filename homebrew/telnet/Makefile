CC = arm-none-linux-gnueabi-gcc
CFLAGS = -fPIC -std=gnu99 -shared -nostartfiles -include prolin_printf.h
INCLUDES = -I$(PREFIX)/include 

SOURCES = main.c ui.c  
HEADERS = ui.h font.h
TARGET = telnet.so
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
