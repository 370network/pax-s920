CXX = $(HOST)-gcc
CXXFLAGS = -fPIC -shared -nostartfiles -Wall -I$(PREFIX)/include

SRCS = osal_test.c magstripe_reader.c sd_mount.c xui_test.c

OBJS = $(SRCS:.c=.o)
TARGETS = $(SRCS:.c=.so)

all: $(TARGETS)

%.so: %.o
	$(CXX) $(CXXFLAGS) -o $@ $^
	$(HOST)-strip $@

%.o: %.c
	$(CXX) $(CXXFLAGS) -include prolin_printf.h -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGETS)

.PHONY: all clean
