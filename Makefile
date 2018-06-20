# source C++ files for the project:
SRC = main.cpp common/GLShader.cpp common/generate_MVP.cpp

# object files to be compiled together:
OBJS = $(SRC:.cpp = .o)

# compiler:
CC = g++

# include paths:
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include

# library paths:
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib

# compiler flags (first line is for debugging): include -03 for optomization
# COMPILER_FLAGS = -Wall -Wextra -v
COMPILER_FLAGS = -Wall -Wextra
# COMPILER_FLAGS = -w -O3

# libraries linked against:
LINKER_FLAGS = -framework OpenGL -lglfw -lglew

# name of executable:
OBJ_NAME = orbit

# target to compile executable:
all: $(OBJS)
	$(CC) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) $(OBJS)

clean:
	rm -f core *.o
