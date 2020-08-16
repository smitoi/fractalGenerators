CC = g++
SRCS = mandelbrot.cpp
OBJS = mandelbrot.o
FLAGS = -std=c++11 -c
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET=mandelbrot

all: clean $(TARGET)

$(TARGET):
	$(CC) $(FLAGS) -o $(OBJS) $(SRCS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	$(RM) *.o *~ $(TARGET)
