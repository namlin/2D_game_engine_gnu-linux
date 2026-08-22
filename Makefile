CXX = g++
CXXFLAGS = -Wall -std=c++17
EXEC = game
SRC = $(wildcard src/*.cc)

# Auto-detect OS from /etc/os-release if DISTRO is not specified:
DISTRO ?= $(shell if grep -qi fedora /etc/os-release 2>/dev/null; then echo fedora; else echo debian; fi)

# Set Lua flags and include paths per distribution:
ifeq ($(DISTRO), fedora)
    LUA_INC = $(shell pkg-config --cflags lua 2>/dev/null || echo "")
    LUA_LIB = $(shell pkg-config --libs lua 2>/dev/null || echo "-llua")
else
    LUA_INC = $(shell pkg-config --cflags lua5.3 2>/dev/null || echo "-I/usr/include/lua5.3")
    LUA_LIB = $(shell pkg-config --libs lua5.3 2>/dev/null || echo "-llua5.3")
endif

# Include paths: local ./libs directory + distro-specific Lua headers.
INC_PATH = -I ./include $(LUA_INC)

# Linker flags:
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer $(LUA_LIB)

.PHONY: all build run clean lint msan asan

all: build

build:
	$(CXX) $(CXXFLAGS) $(INC_PATH) $(SRC) $(LFLAGS) -o $(EXEC)

run:
	./$(EXEC)

clean:
	rm -f $(EXEC)

lint:
	cpplint ./src/*.cc

# Create an executable with Memory Sanitizer (MSan):
msan: $(BIN_DIR)
	clang $(CXX_FLAGS) -fsanitize=memory ./src/*.cc -o $(EXEC)

# Create an executable with Address Sanitizer (ASan):
asan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -fsanitize=address ./src/*.cc -o $(EXEC)
