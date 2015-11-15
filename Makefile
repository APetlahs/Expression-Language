CXX = g++
CXXFLAGS = -Wall -MMD
LDFLAGS = -lfl -ly
EXEC = program
OBJECTS = grammar.o lexer.o node.o visitor.o main.o eval_visitor.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} -o ${EXEC} ${OBJECTS}

grammar.o: grammar.y
	bison -d -o grammar.cpp grammar.y
	${CXX} ${CXXFLAGS} -c -o grammar.o grammar.cpp

lexer.o: lexer.l
	flex -o lexer.cpp lexer.l
	${CXX} ${CXXFLAGS} -c -o lexer.o lexer.cpp

-include ${DEPENDS}

.PHONY: clean

clean:
	rm -f ${OBJECTS} ${EXEC} ${DEPENDS} grammar.hpp grammar.cpp lexer.cpp
