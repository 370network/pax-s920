CFLAGS += -mtune=arm1136j-s -ftree-vectorize -funroll-loops -fomit-frame-pointer -ffast-math -s -fno-strict-aliasing -O3 -g0 -fPIC -shared -nostartfiles -Wall -I$(PREFIX)/include

SRCS = src/arcade.c src/pacbox.c src/hardware.c src/z80.c src/wsg3.c src/pacbox_lcd.c

OBJS = $(SRCS:.c=.o)

TARGET = paxman/paxman.so

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	$(STRIP) $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -include prolin_printf.h -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
