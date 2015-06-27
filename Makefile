# This makefile is for OSX
# You need to install `glfw3` and `glew`. I install them by `Homebrew` with the following command:
# brew tap homebrew/versions
# brew install glfw3 glew

ProjName = Carving

# OBJS specifies which files to compile as part of the project
OBJS = $(wildcard $(ProjName)/*.cpp)
# OBJS = $(ProjName)/$(ProjName).cpp $(ProjName)/world.cpp $(ProjName)/chunk.cpp $(ProjName)/spot.cpp $(ProjName)/surface.cpp $(ProjName)/triangle.cpp $(ProjName)/helper.cpp $(ProjName)/looper.cpp $(ProjName)/hand.cpp $(ProjName)/border.cpp $(ProjName)/wall.cpp $(ProjName)/character.cpp $(ProjName)/hurtable.cpp $(ProjName)/base.cpp

# CC specifies which compiler we're using
CC = g++

# INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -I/usr/local/include -I/opt/X11/include

# LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -L/usr/local/lib -I/opt/X11/lib

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -std=c++0x

# LINKER_FLAGS specifies the libraries we're linking against
# Cocoa, IOKit, and CoreVideo are needed for static GLFW3.
LINKER_FLAGS = -framework OpenGL -framework GLUT -lglfw3 -framework ApplicationServices

OBJ_FOLDER = OSX_build

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = $(OBJ_FOLDER)/$(ProjName)

# This is the target that compiles our executable
all : $(OBJS)
	mkdir -p $(OBJ_FOLDER)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
