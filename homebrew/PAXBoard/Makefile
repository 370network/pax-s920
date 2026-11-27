INCLUDES = -I$(PREFIX)/include
CFLAGS = -shared -nostartfiles

SOURCES = main.c app.c applist.c ui.c apploader.c
HEADERS = app.h applist.h ui.h apploader.h
TARGET = Launcher.so
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
