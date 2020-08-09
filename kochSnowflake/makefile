CC = g++
SRCS = main.cpp
OBJS = main.o
FLAGS = -std=c++11 -c
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET=kochSnowflake

all: clean $(TARGET)

$(TARGET):
	$(CC) $(FLAGS) -o $(OBJS) $(SRCS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	$(RM) *.o *~ $(TARGET)
