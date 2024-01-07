vpath %.h = include
vpath %.cpp = src

CXXFLAGS = -fPIC -Wall -Wextra -O0 -Iinclude
simple_compile = g++ -c $(CXXFLAGS) $< -o $@

sources = CCompiler.cpp Compile.cpp Lexer.cpp Parser.cpp
objects = $(sources:.cpp=.o)

jcc : $(objects)
	g++ -o jcc $(objects)

CCompiler.o : CCompiler.cpp Parser.h
	$(simple_compile)

Compile.o : Compile.cpp Parser.h Generate.h
	$(simple_compile)

Lexer.o : Lexer.cpp
	$(simple_compile)

Parser.o : Parser.cpp Parser.h
	$(simple_compile)

clean : 
	rm jcc $(objects)