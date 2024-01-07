#include "Parser.h"
#include "Generate.h"
#include <fstream>

int compile_exp(Expression e, std::fstream* p_fs) {

	if (e.inner != nullptr) {
		compile_exp(*e.inner, p_fs);
		generate_unop(e.expr, p_fs);
	}
	else {
		generate_movl(e.expr, p_fs);
	}

	return 1;
}

int compile(Program p) {

	std::fstream fs;
	fs.open("./return2.s", std::fstream::out);

	int program_length = p.glob.size();
	for (int i = 0; i < program_length; i++) {
		Func active_function = p.glob[i];
		std::string id = active_function.identifier;
		generate_function(id, &fs);

		int function_length = active_function.body.size();
		for (int j = 0; j < function_length; j++) {
			Statement active_statement = active_function.body[j];
			if (active_statement.type == active_statement.Return) {
				Expression exp = active_statement.exp;
				compile_exp(exp, &fs);
				std::string return_num = active_statement.exp.expr;
				generate_return(&fs);
			}
		}
	}

	return 0;
}
