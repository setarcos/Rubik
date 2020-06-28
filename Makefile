TARGET=rubic
SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SRC))

all:$(TARGET)

FLAGS=-Wall

%.o : %.cpp
	g++ $(FLAG) -c $< -o $@
$(TARGET) : $(OBJS)
	g++	$(OBJS) -lGL -lGLU -lglfw -lGLEW -o $(TARGET)
clean:
	rm $(TARGET) *.o -f
