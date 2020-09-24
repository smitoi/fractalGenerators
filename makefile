CC = g++
SRCS = complex.cpp
OBJS = complex.o
FLAGS = -std=c++11 -c -pthread -o
LFLAGS = -o $(TARGET) -pthread
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET=complex

all: clean $(TARGET)

$(TARGET):
	$(CC) $(FLAGS) $(OBJS) $(SRCS)
	$(CC) $(LFLAGS) $(OBJS) $(LIBS)

clean:
	$(RM) *.o *~ $(TARGET)
