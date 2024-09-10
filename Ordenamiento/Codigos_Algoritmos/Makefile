# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Executable names
EXEC2 = mergesort
EXEC3 = quicksort
EXEC4 = selectionsort
EXEC5 = funcionsorting

# Source files
SRC2 = mergesort.cpp
SRC3 = quicksort.cpp
SRC4 = selectionsort.cpp
SRC5 = funcionsorting.cpp

# Object files
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)
OBJ5 = $(SRC5:.cpp=.o)

# Default target
all: $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

# Rules for building each executable
$(EXEC2): $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC3): $(OBJ3)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC4): $(OBJ4)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC5): $(OBJ5)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rules for generating object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Run all executables
run: all
	@echo "Running mergesort..."
	./$(EXEC2)
	@echo "Running quicksort..."
	./$(EXEC3)
	@echo "Running selectionsort..."
	./$(EXEC4)
	@echo "Running funcionsorting..."
	./$(EXEC5)

# Clean up generated files
clean:
	rm -f *.o $(EXEC2) $(EXEC3) $(EXEC4) $(EXEC5)

.PHONY: all clean run