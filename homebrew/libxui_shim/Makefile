CFLAGS = -mcpu=arm1136j-s -shared -Os -fPIC -fvisibility=hidden -fno-unwind-tables -fno-asynchronous-unwind-tables -Wl,-rpath,'/lib' -Wl,-soname,libxui.so -Wl,--no-as-needed -Wl,--build-id=none -Wl,--hash-style=sysv -Wl,-z,noseparate-code -L. -l:libxui_original.so -ldl
INCLUDES += -I$(PREFIX)/include

SOURCES = libxui_shim.c
TARGET = libxui.so
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	cp $(TOOLCHAIN)/arm-unknown-linux-gnueabi/lib/libxui.so libxui_original.so
	patchelf --set-soname /usr/lib/libxui.so libxui_original.so
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@
	$(STRIP) $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
