CC = gcc
XX = g++
CFLAGS = -Wall -O -g
TARGET = helloworld
SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SOURCES)))

%.o:%.cpp
	$(XX) $(CFLAGS) -c $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):$(OBJS)
	$(XX) $(OBJS) -o $(TARGET)

clean:
	rm -rf *.o $(TARGET)