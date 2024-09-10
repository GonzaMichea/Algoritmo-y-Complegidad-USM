# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g

# Executable names
EXEC2 = IterativoTradicional
EXEC3 = strassen
EXEC4 = traspuesta

# Source files
SRC2 = IterativoTradicional.cpp
SRC3 = strassen.cpp
SRC4 = traspuesta.cpp

# Object files
OBJ2 = $(SRC2:.cpp=.o)
OBJ3 = $(SRC3:.cpp=.o)
OBJ4 = $(SRC4:.cpp=.o)

# Default target
all: $(EXEC2) $(EXEC3) $(EXEC4)

# Rules for building each executable
$(EXEC2): $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC3): $(OBJ3)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(EXEC4): $(OBJ4)
	$(CXX) $(CXXFLAGS) -o $@ $^


# Rules for generating object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Run all executables
run: all
	@echo "Running IterativoTradicional..."
	./$(EXEC2)
	@echo "Running strassen..."
	./$(EXEC3)
	@echo "Running traspuesta..."
	./$(EXEC4)

# Clean up generated files
clean:
	rm -f *.o $(EXEC2) $(EXEC3) $(EXEC4) 

.PHONY: all clean run