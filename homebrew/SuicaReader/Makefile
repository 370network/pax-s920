CFLAGS = -include prolin_printf.h -Wl,--unresolved-symbols=ignore-all,--dynamic-linker=/lib/ld-linux.so.3 -lfreetype -lpng12 -lxui -lfribidi -lz -losal -lcbinder -lcrypto -lrt
INCLUDES = -I$(PREFIX)/include 

SOURCES = main.c
TARGET = suica-reader.o
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
