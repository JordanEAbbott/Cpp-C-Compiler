#pragma once
#include<string>
#include<fstream>

//Must be better string manip techniques

void generate_function(std::string s, std::fstream* fs) {

	std::string output = "\t.globl _";
	output += s;
	output += "\n_";
	output += s;
	output += ":\n";
	*fs << output;

	return;
}

void generate_unop(std::string s, std::fstream* fs) {

	std::string output;
	if (s == "LogicalNegation") {
		output = "\tcmpl\t$0, %eax\n\tmovl\t$0, %eax\n\tsete %al\n";
	}
	else if (s == "Negation") {
		output = "\tneg\t%eax\n";
	}
	else {
		output = "\tnot\t%eax\n";
	}
	*fs << output;

	return;
}

void generate_movl(std::string s, std::fstream* fs) {

	std::string output = "\tmovl\t$";
	output += s;
	output += ", %eax\n";
	*fs << output;

	return;
}

void generate_return(std::fstream* fs) {

	std::string output = "\tret";
	*fs << output;
	return;
}