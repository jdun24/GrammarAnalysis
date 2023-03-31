# Specify the compiler to use
CXX = g++

# Specify the flags to use
CXXFLAGS = -std=c++11 -Wall -Wextra

# Specify the source files
SRCS = lexer.cc inputbuf.cc project2.cc

# Specify the name of the executable
EXEC = exe

# Specify the command to build the executable
$(EXEC): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(EXEC)

# Specify the command to clean the project
clean:
	rm -f $(EXEC)
	
clena:
	rm -f $(EXEC)
	
run:
	clear
	./$(EXEC) 5 < test.txt


# Specify the default command
.DEFAULT_GOAL := $(EXEC)