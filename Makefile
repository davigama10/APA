# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17

# Target executable name
TARGET = projetoAPA

# Source files
SOURCES = main.cpp

# Object files (generated from source files)
OBJECTS = $(SOURCES:.cpp=.o)

# Default rule for make
all: $(TARGET)

# Rule to link the object files to produce the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

# Rule to compile source files to produce object files
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for cleaning up (removing) generated files
clean:
	rm -f $(OBJECTS) $(TARGET)
