# Compiler (modify if needed)
CXX = g++

# C++ source file extension (modify if needed)
CXX_FLAGS = -Wall -Wextra -w -g  # Enable warnings and debugging symbols
CXX_OBJS = $(wildcard Analisador_Lexico_Regex/*.cpp)  # Source files in Analisador_Lexico_Regex
SRC_OBJS = $(wildcard src/*.cpp)  # Source files in src

# All targets (for clarity)
.PHONY: all clean lexico analyse

all: lexico analyse  # Build both executables

lexico: $(CXX_OBJS)
	$(CXX) $(CXX_FLAGS) -o lexico $^

analyse: $(SRC_OBJS)
	$(CXX) $(CXX_FLAGS) -o analyse $^

clean:
	rm -f *.o lexico analyse  # Remove object files and executables
