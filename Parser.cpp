#include "Parser.h"
#include<iostream>
#include<vector>
#include<string>
#include<regex>

/*
Clarify declaration rules: Statement MUST be of form "return" <int> ";" | "int" <id> "=" <int> ";" etc.
*/

void fail(std::vector<std::string>::iterator* p_it) {
	std::cout << "Error in formatting of file. Failed at " << *(*p_it) << std::endl;
	exit(0);
}

int is_unop(std::vector<std::string>::iterator* p_it) {

	std::vector<std::string> unops = { "LogicalNegation", "Negation", "Complement" };
	int res = 0;
	for (int i = 0; i < unops.size(); i++) {
		if (*(*p_it) == unops[i]) {
			res = 1;
			break;
		}
	}

	return res;
}

std::string sort_negation(std::vector<std::string>::iterator* p_it) {

	std::regex integer_regex("([0-9]+)");
	std::advance(*p_it, -1);
	if (std::regex_match(*(*p_it), integer_regex)) {
		std::advance(*p_it, 1);
		return "Subtraction";
	}

	std::advance(*p_it, 1);
	return "Negation";
}

Factor parse_factor(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it) {

	Factor f;
	std::regex integer_regex("([0-9]+)");

	std::string current = *(*p_it);
	if (current == "OpenBracket") {
		std::advance(*p_it, 1);
		Expression e = parse_expression(tokens, p_it);
		if (*(*p_it) != "CloseBracket") {
			fail(p_it);
		}
		f.bracketed_exp = &e;
		f.type = f.Bracketed;
		return f;
	}
	else if (is_unop(p_it)) {
		Factor inner_f = parse_factor(tokens, p_it);
		f.inner_factor = &inner_f;
		f.type = f.Unop;
		return f;
	}
	else if (std::regex_match(*(*p_it), integer_regex)) {
		f.plain_exp = *(*p_it);
		f.type = f.Constant;
		return f;
	}
	else {
		fail(p_it);
		return f;
	}
}

Term parse_term(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it) {

	Term t;
	Term* p_term = &t;

	Factor f = parse_factor(tokens, p_it); //Important to ensure p_it is not advanced in this function.
	std::advance(*p_it, 1);
	while (*(*p_it) == "Multiplication" || *(*p_it) == "Division") {
		Term* sub_t = new Term;
		std::advance(*p_it, 1);
		Factor sub_f = parse_factor(tokens, p_it);
		sub_t->factor = &sub_f;
		p_term->sub_term = sub_t;
		p_term = sub_t;
		delete sub_t;
	}

	t.factor = &f;
	return t;
}

Expression parse_expression(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it) {

	Expression e;
	Expression* p_exp = &e;

	Term t = parse_term(tokens, p_it);
	std::string next = *(*p_it);
	if (next == "Negation") {
		next = sort_negation(p_it);
	}
	while (next == "Addition" || next == "Subtraction") {
		Expression* sub_e = new Expression;
		std::advance(*p_it, 1);
		Term sub_term = parse_term(tokens, p_it);
		sub_e->term = &sub_term;
		p_exp->sub_expr = sub_e;
		p_exp = sub_e;
		delete sub_e;

		next = *(*p_it);
		if (next == "Negation") {
			next = sort_negation(p_it);
		}
	}

	e.term = &t;
	return e;
}

Statement parse_statement(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it) {

	if (*(*p_it) != "returnKeyword") {
		fail(p_it);
		return {};
	}
	std::advance(*p_it, 1);

	Expression exp = parse_expression(tokens, p_it);
	Statement statement;
	statement.type = statement.Return;
	statement.exp = exp;
	std::advance(*p_it, 1);

	if (*(*p_it) != "Semicolon") {
		fail(p_it);
		return {};
	}
	std::advance(*p_it, 1);

	return statement;
}

Func parse_function(std::vector<std::string> tokens, std::vector<std::string>::iterator* p_it) {

	Func f;
	std::regex identifier_regex("([a-zA-Z]\\w*)");
	std::vector<Statement> statements;

	if (*(*p_it) != "intKeyword") {
		fail(p_it);
		return {};
	}
	f.return_type = *(*p_it);
	std::advance(*p_it, 1);

	if (!(std::regex_match(*(*p_it), identifier_regex))) {
		fail(p_it);
		return {};
	}
	f.identifier = *(*p_it);
	std::advance(*p_it, 1);

	if (*(*p_it) != "OpenBracket") {
		fail(p_it);
		return {};
	}
	std::advance(*p_it, 1);

	if (*(*p_it) != "CloseBracket") {
		fail(p_it);
		return {};
	}
	std::advance(*p_it, 1);

	if (*(*p_it) != "OpenBrace") {
		fail(p_it);
		return {};
	}
	std::advance(*p_it, 1);

	while (*(*p_it) != "CloseBrace") {
		//Fix this - no close brace results in out of vector range error
		Statement st = parse_statement(tokens, p_it);
		statements.push_back(st);
	}
	std::advance(*p_it, 1);
	f.body = statements;

	return f;
}

Program parse_program(std::vector<std::string> tokens) {

	std::vector<Func> functions;
	std::vector<std::string>::iterator it = tokens.begin();
	std::vector<std::string>::iterator debug_it = tokens.end();
	std::vector<std::string>::iterator* p_it = &it;
	while (*p_it != tokens.end()) {
		Func f = parse_function(tokens, p_it);
		functions.push_back(f);
	}

	Program prog;
	prog.glob = functions;

	return prog;
}