#include "Parser.h"
#include <vector>
#include <string>

/*
Currently not working for:
	No end brace
	New lines
*/

//Need to sort out adding files from this cpp file.
std::vector<std::string> lex();
//int compile(Program p);

int main() {

	std::vector<std::string> lexed = lex();
	Program p = parse_program(lexed);
	//compile(p);

	return 0;
}