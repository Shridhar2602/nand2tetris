# Define the compiler and compiler flags
CXX = g++
# CXXFLAGS = -Wall

# Define the source files and the target executable
SRCS = jackAnalyzer.cpp jackTokenizer.cpp compilationEngine.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = jackAnalyzer

# Default rule to build the target
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile the .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Phony targets
.PHONY: clean

# Rule to clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)