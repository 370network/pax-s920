CFLAGS = -fPIC -shared -nostartfiles -Wall -I$(PREFIX)/include -ldl

SRCS = osal_test.c xui_test.c

OBJS = $(SRCS:.c=.o)
TARGETS = $(SRCS:.c=.so)

all: $(TARGETS)

%.so: %.o
	$(CC) $(CFLAGS) -o $@ $^
	$(STRIP) $@

%.o: %.c
	$(CC) $(CFLAGS) -include prolin_printf.h -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGETS)

.PHONY: all clean
