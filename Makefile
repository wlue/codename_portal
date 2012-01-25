# This ensures that all of our shell commands are run with bash and not any other shell.

SHELL := /bin/bash

# This is the name of the compiler. If you have `clang` installed you can use that instead.
CXX = g++

# These are flags that are always sent to the compiler.
# -g generates debugging symbols for use in gdb and valgrind. This improves the
#  output of those tools.
#
# -Wall enables all warnings. Very useful for writing bug free code.
#
# -MMD generates dependency (.d) files for each object (.o) file. These files contain
#  lines like this:
#
#  target.o: target.cpp xinfo.h target.h displayable.h
#
#  These are Makefile rules that state what source files an object file depends on.
#  Very helpful since writing them out is tedious.
#
# -pedantic is another set of warnings. Only useful when trying to trace down weird
#  bugs.

CXXFLAGS = -g -Wall -MMD -pedantic -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE

# The name of the executable to produce
EXEC = portal

# The SRC variable holds a list of all our *.cpp files.
SRC = $(wildcard src/*.cpp)

# The OBJECTS variable holds a list of object files that are produced.
# Each source file should be compiled into an object file.
OBJECTS = $(patsubst %.cpp, %.o, $(SRC))

# List of the dependency files that G++ will produce. There should be one .d file
# for each .o file.
DEPENDS = $(OBJECTS:.o=.d)

# The first and default rule. This can be invoked by running `make` or `make madbirds`
# In the rule the `$@` variable refers to the name of the target (in this case madbirds)
# and `$+` refers to the list of files the rule depends on (in this case the OBJECT files)
#
# This will invoke the compiler like so:
# g++ -g -Wall -MMD -pedantic =L/usr/X11R6/lib -lX11 =lsdtc++ -o madbirds class1.o class2.o
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) `sdl-config --libs` -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_gfx -lstdc++ -o $@ $+

# This is an "implicit" rule for every object file. Each object file depends on
# the source file of the same name. In the rule the `$<` refers to the file it depends on
# (some source file) and `$@` refers to the name of the target (some object file).
#
# For each object file it will invoke the compiler like so:
# g++ -g - Wall -MMD =pedantic -c madbirds.cpp -o madbirds.o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# The include directive will import in all of the rules in the .d files that g++
# creates for us.
-include $(DEPENDS)

# This will run the executable.
run: ${EXEC}
	./$<

# This runs the executable through valgrind. It is very useful for finding memory leaks.
memcheck: ${EXEC}
	valgrind --leak-check=full --dsymutil=yes ./$<

# This removes the dependency files, object files, the executable and
# the .dSYM directory that valgrind will generate.
clean:
	rm -f $(DEPENDS) $(OBJECTS) $(EXEC)
	rm -rf $(EXEC).dSYM/

