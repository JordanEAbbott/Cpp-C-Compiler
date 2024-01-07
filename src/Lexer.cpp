#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

struct Match {

    std::vector<std::string> keywords{ "int", "return" };

    enum Punctuation {
        LogicalNegation = 33,
        OpenBracket = 40,
        CloseBracket = 41,
        Multiplication = 42,
        Addition = 43,
        Negation = 45,
        Division = 47,
        Semicolon = 59,
        OpenBrace = 123,
        CloseBrace = 125,
        Complement = 126
    };
};

bool match_regex(std::string text, std::regex regex) {

    return std::regex_match(text, regex);
}

bool match_keywords(std::string text, Match m) {

    for (int i = 0; i < m.keywords.size(); i++) {
        if (text == m.keywords[i]) {
            return true;
        }
    }

    return false;
}

int match_symbols(char text, const char symbols[]) {

    for (int i = 0; i < strlen(symbols); i++) {
        if (text == symbols[i]) {
            return symbols[i];
        }
    }
    return 0;
}

std::vector<std::string> lex()
{
    Match match;
    std::string my_file;
    std::string line;
    std::vector<std::string> lexed;
    std::regex integer_regex("([0-9]+)");
    std::regex identifier_regex("([a-zA-Z]\\w*)");
    std::fstream fs;

    fs.open("./return2.c", std::fstream::in);
    if (!fs) {
        std::cout << "No file found";
    }
    else {
        while (std::getline(fs, line)) {
            my_file.append(line);
        }
    }
    fs.close();

    const char symbols[] = "(){};!~-*/+";
    for (int i = 0; i < my_file.length(); i++) {
        if (match_symbols(my_file[i], symbols)) {
            my_file.insert(i, " ");
            i = i + 2;
            my_file.insert(i, " ");
        }
    }

    std::istringstream iss(my_file);
    std::vector<std::string> tokens{ std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{} };
    for (int i = 0; i < tokens.size(); i++) {
        if (match_keywords(tokens[i], match)) {
            lexed.push_back(tokens[i].append("Keyword"));
        } //Need to put in special chars
        else if (match_regex(tokens[i], identifier_regex)) {
            lexed.push_back(tokens[i]);
        }
        else if (match_regex(tokens[i], integer_regex)) {
            lexed.push_back(tokens[i]);
        }
        else {
            std::string lexed_symbol;
            int symbol = match_symbols(tokens[i][0], symbols);
            if (symbol == match.LogicalNegation) {
                lexed_symbol = "LogicalNegation";
            }
            else if (symbol == match.OpenBracket) {
                lexed_symbol = "OpenBracket";
            }
            else if (symbol == match.CloseBracket) {
                lexed_symbol = "CloseBracket";
            }
            else if (symbol == match.Multiplication) {
                lexed_symbol = "Multiplication";
            }
            else if (symbol == match.Addition) {
                lexed_symbol = "Addition";
            }
            else if (symbol == match.Negation) {
                lexed_symbol = "Negation";
            }
            else if (symbol == match.Division) {
                lexed_symbol = "Division";
            }
            else if (symbol == match.Semicolon) {
                lexed_symbol = "Semicolon";
            }
            else if (symbol == match.OpenBrace) {
                lexed_symbol = "OpenBrace";
            }
            else if (symbol == match.CloseBrace) {
                lexed_symbol = "CloseBrace";
            }
            else if (symbol == match.LogicalNegation) {
                lexed_symbol = "LogicalNegation";
            }
            else if (symbol == match.Complement) {
                lexed_symbol = "Complement";
            }
            lexed.push_back(lexed_symbol);
        }
    }

    for (int i = 0; i < lexed.size(); i++) {
        std::cout << lexed[i] << std::endl;
    }

    std::cout << my_file << std::endl;

    return lexed;
}