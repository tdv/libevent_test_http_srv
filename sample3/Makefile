PROJECT = sample3

CPP = $(wildcard *.cpp)

LIBS = pthread event

CXX = g++

FLAGS = -Wall -std=c++11  -O3 -g0

OBJ = $(CPP:.cpp=.o)

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^ -o $@

all: $(OBJ)
	$(CXX) $(FLAGS) $(OBJ) -o $(PROJECT) $(addprefix -l, $(LIBS))
