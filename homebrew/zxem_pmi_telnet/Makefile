CODE_DIRECTORIES = src src/*
INCLUDE_DIRECTORIES = src $(PREFIX)/include/
LIBS = $(PREFIX)/lib/
SOURCE_FILES = $(foreach directory,$(CODE_DIRECTORIES),$(wildcard $(directory)/*.c))
OBJECT_FILES = $(patsubst %.c,%.o,$(SOURCE_FILES))
DEPENDENCY_FILES = $(patsubst %.c,%.d,$(SOURCE_FILES))
ROMS = $(foreach directory,snapshots-roms,$(wildcard $(directory)/*))
OUTPUT_FOLDER = $(PWD)/

CC = $(PREFIX)/bin/$(HOST)-gcc
CFLAGS = -Wall -fPIC $(foreach DIRECTORY,$(INCLUDE_DIRECTORIES),-I$(DIRECTORY)) -nostartfiles -std=c99
DEPENDENCY_FLAGS = -MMD -MP
UPLOAD_COMMON_PATH = /data/app/MAINAPP

PUSH_COMMAND = paxpush

.PHONY: all clean
.SECONDEXPANSION:

rebuild: clean all

print-%:
	@echo "$* = $($*)"

clean:
	rm -f $(OBJECT_FILES) $(DEPENDENCY_FILES)

z80tables.h: maketables.c
	$(CC) -Wall $< -o maketables $(DEPENDENCY_FLAGS)
	./maketables > $@

%.o: %.c
	$(CC) $(CFLAGS) $(DEPENDENCY_FLAGS) -c -o $@ $<

generic-upload:
	$(PUSH_COMMAND) $(TARGET) $(UPLOAD_COMMON_PATH)/$(TARGET)

compile:
	$(CC) $(CFLAGS) $(DEPENDENCY_FLAGS) -shared -o $(OUTPUT_FOLDER)/$(TARGET) $(SOURCE)
	$(HOST)-strip $(OUTPUT_FOLDER)/$(TARGET)

zxem: src/zxem.o src/z80emu.o src/pax.o src/gui.o
	$(MAKE) compile TARGET=$@ SOURCE="$^"

pmi: src/pmi/*.o
	$(MAKE) compile TARGET=$@ SOURCE="$^"

telnet: src/telnet/telnet.o
	$(MAKE) compile TARGET=$@ SOURCE="$^"

upload-roms:
	for rom in $(ROMS); do $(MAKE) generic-upload TARGET=$$rom; done

# obsolete, left here for compatibility reasons
push: zxem
	$(MAKE) upload-roms
