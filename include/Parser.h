#pragma once
#include<string>
#include<vector>

struct Expression;

//<factor> :: = "(" < exp > ")" | <unary_op> <factor> | <int>
struct Factor {

	enum Types {
		Bracketed,
		Unop,
		Constant
	};

	Types type;
	Factor* inner_factor;
	Expression* bracketed_exp;
	std::string plain_exp;
};

//<term> ::= <factor> { ("*" | "/") <factor> }
struct Term {

	enum Higher_Precedence {
		None,
		Multiplication,
		Division
	};

	//Required
	Factor* factor;

	//Optional
	Term* sub_term;
};

//<exp> ::= <term> { ("+" | "-") <term> }
struct Expression {

	enum Lower_Precedence {
		Addition,
		Negation
	};

	//Required
	Term* term;

	//Optional
	Expression* sub_expr;
};

struct Statement {

	enum Types {
		Declaration,
		Return
	};

	Types type;
	Expression exp;
};

struct Func {

	std::string return_type;
	std::string identifier;
	std::vector<Statement> body;
};

struct Program {

	std::vector<Func> glob;
};

void fail(std::vector<std::string>::iterator* p_it);

Expression parse_expression(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it);
Statement parse_statement(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it);
Func parse_function(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it);
Program parse_program(std::vector<std::string> tokens);



/*
struct Expression {

	enum Types {
		Constant,
		UnOp,
		BinOp
	};

	Expression* inner;
	Types type;
	std::string expr;
};

Expression parse_expression(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it) {

	Expression e;
	std::regex integer_regex("([0-9]+)");

	if (std::regex_match(*(*p_it), integer_regex)) {
		e.type = e.Constant;
		e.expr = *(*p_it);
		e.inner = nullptr;
	}
	else if (is_unop(p_it)) {
		Expression* p_inner = new Expression;
		e.type = e.UnOp;
		e.expr = *(*p_it);
		std::advance(*p_it, 1);
		*p_inner = parse_expression(tokens, p_it);
		e.inner = p_inner;
	}

	return e;
}
*/