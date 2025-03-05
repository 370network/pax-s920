CXX = arm-none-linux-gnueabi-g++
CXXFLAGS = -fPIC -shared -nostartfiles -Wall

SRCS = launcher.cpp settings.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = launcher.so

all: $(TARGET) push

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -fPIC -shared -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

push:
	python3 $(PREFIX)/xcb/client.py . push launcher.so /data/app/MAINAPP/lib/libosal.so

.PHONY: all clean
