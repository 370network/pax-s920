CFLAGS = -lfreetype -lpng12 -lxui -lfribidi -lz -lcbinder -lcrypto -lrt -lts -losal
INCLUDES = -I$(PREFIX)/include

SOURCES = main.c
TARGET = suica-reader.o
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) $(LDFLAGS) -o $@
	#following patchelf is a workaround to make our app look
	#specifically for osal on an absolute path to prevent collisions with our root
	patchelf --replace-needed libosal.so /usr/lib/libosal.so $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
