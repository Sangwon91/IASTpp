CXX = g++
CXXFLAGS = -O3 -std=c++11 -W -Wall #-Werror
INCPATH = /appl/molsim-Eigen/
SRCS = $(wildcard *.cpp)
OBJS_ALL = $(SRCS:.cpp=.o)
OBJS = $(patsubst arma_solver.%, , $(OBJS_ALL)) # remove arma solver. we don't need it.
TARGET = libiast.a

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rc $(@) $(OBJS)

.cpp.o: $(SRCS)
	$(CXX) $(CXXFLAGS) -c $(<) -I $(INCPATH)

clean:
	$(RM) $(OBJS) $(TARGET)
