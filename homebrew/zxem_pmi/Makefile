CODE_DIRECTORIES = src src/*
INCLUDE_DIRECTORIES = src $(PREFIX)/include/
LIBS = $(PREFIX)/lib/
SOURCE_FILES = $(foreach directory,$(CODE_DIRECTORIES),$(wildcard $(directory)/*.c))
OBJECT_FILES = $(patsubst %.c,%.o,$(SOURCE_FILES))
DEPENDENCY_FILES = $(patsubst %.c,%.d,$(SOURCE_FILES))
ROMS = $(foreach directory,snapshots-roms,$(wildcard $(directory)/*))
OUTPUT_FOLDER = $(PWD)/

CFLAGS = -Wall -fPIC $(foreach DIRECTORY,$(INCLUDE_DIRECTORIES),-I$(DIRECTORY)) -nostartfiles -std=c99
DEPENDENCY_FLAGS = -MMD -MP

print-%:
	@echo "$* = $($*)"

clean:
	rm -f $(OBJECT_FILES) $(DEPENDENCY_FILES)

z80tables.h: maketables.c
	$(CC) -Wall $< -o maketables $(DEPENDENCY_FLAGS)
	./maketables > $@

%.o: %.c
	$(CC) $(CFLAGS) $(DEPENDENCY_FLAGS) -c -o $@ $<

compile:
	$(CC) $(CFLAGS) $(DEPENDENCY_FLAGS) -shared -o $(TARGET) $(SOURCE)
	$(STRIP) $(TARGET)

zxem: src/zxem.o src/z80emu.o src/pax.o src/gui.o
	$(MAKE) compile TARGET=$@ SOURCE="$^"

pmi: src/pmi/PMI.o
	$(MAKE) compile TARGET=$@ SOURCE="$^"
	
.PHONY: all clean
