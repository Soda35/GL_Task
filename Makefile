# Compiler settings
CXX=g++
CXXFLAGS=-std=c++20 -Wall

# Library settings
LIB_SOURCES=TaskManager.cpp FileSearcher.cpp
LIB_OBJECTS=$(LIB_SOURCES:.cpp=.o)
LIB_NAME=libsearcher.a

# Program settings
PROGRAM_SOURCES=main.cpp
PROGRAM_OBJECTS=$(PROGRAM_SOURCES:.cpp=.o)
PROGRAM_NAME=searcher

.PHONY: all clean

all: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(PROGRAM_OBJECTS) $(LIB_NAME)
        $(CXX) $(CXXFLAGS) $(PROGRAM_OBJECTS) -L. -lsearcher -o $@

$(LIB_NAME): $(LIB_OBJECTS)
        ar rcs $@ $^

%.o: %.cpp
        $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
        rm -f $(LIB_OBJECTS) $(PROGRAM_OBJECTS) $(LIB_NAME) $(PROGRAM_NAME)