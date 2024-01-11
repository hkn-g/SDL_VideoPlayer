
# Makefile for SDL Video Player Application

# Compiler
CXX := g++

# Application Name
APPNAME := SDL_VideoPlayer

# Source Files
SRC := SDL_VideoPlayer.cpp

# Compiler Flags
CXXFLAGS := $(shell sdl2-config --cflags) $(shell pkg-config --cflags opencv4)
LDFLAGS := $(shell sdl2-config --libs) $(shell pkg-config --libs opencv4)

# Build Target
$(APPNAME): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(LDFLAGS) -o $(APPNAME)

# Clean Target
clean:
	rm -f $(APPNAME)

# Phony Targets
.PHONY: clean

