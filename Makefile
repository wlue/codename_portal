SHELL := /bin/bash
CXXFLAGS = -g -Wall -MMD -pedantic -O3
EXEC = portal

MACHINE= $(shell uname -s)
ifeq ($(MACHINE), Darwin)
	CXX = clang++
	CXXFLAGS += -std=c++11 -stdlib=libc++
else
	CXX = g++
	CXXFLAGS += -std=c++0x
endif

LIB =

SRC = $(wildcard src/*.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SRC))
DEPENDS = $(OBJECTS:.o=.d)

PKGS = sdl SDL_image SDL_ttf SDL_mixer SDL_gfx
INCLUDE = `pkg-config $(PKGS) --cflags` $(INC)
LIBS = `pkg-config $(PKGS) --libs`

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $+ $(LIBS)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(INCLUDE)

-include $(DEPENDS)

run: ${EXEC}
	./$<

clean:
	rm -f $(DEPENDS) $(OBJECTS) $(EXEC)

