CROSS_COMPILE := arm-none-linux-gnueabi-
CC := ${CROSS_COMPILE}gcc
STRIP := ${CROSS_COMPILE}strip
INCLUDES = -I$(PREFIX)/include
CFLAGS = -fPIC -march=armv6 -std=gnu99 -shared -nostartfiles -include prolin_printf.h

SOURCES = main.c app.c applist.c ui.c apploader.c
HEADERS = app.h applist.h ui.h apploader.h
TARGET = Launcher.so
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LDFLAGS) -o $@
	$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
