#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
using namespace std;

enum TokenType {
    KEYWORDS,
    IDENTIFIER,
    CONSTANTS,
    STRING_LITERAL,
    AR_OP,
    REL_OP,
    ASIGNMENT,
};

typedef struct{
    TokenType type;
    string value;
    int line;

}TokenStruct;
int hexToInt(const string& hex) {
    const string hex_str = hex.substr(2);
    return stoi(hex_str, nullptr, 16);
}



bool isNumeric(const std::string& str) {
    //hexadecimal numbers
    if (str.size() > 2 && str[0] == '0' && str[1] == 'x') {
        for (int i = 2; i < str.size(); i++) {
            if (!std::isxdigit(str[i])) {
                return false;
            }
        }

        return true;
    }
    return false;
}

string to_string(const TokenStruct& type) {
    switch (type.type) {
        case KEYWORDS: return "KEYWORDS";
        case IDENTIFIER: return "IDENTIFIER";
        case CONSTANTS: return "CONSTANTS";
        case STRING_LITERAL: return "STRING_LITERAL";
        case AR_OP: return "AR_OP";
        case REL_OP: return "REL_OP";
        case ASIGNMENT: return "ASIGNMENT";
    }
    return "UNKNOWN";
    /*COMENTARIO DE PRUEBA*/
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "missing arguments" << endl;
        return EXIT_FAILURE;
    }
    fstream inputFile(argv[1], ios::in);
    if (!inputFile) {
        cerr << "couldn't open file" << argv[1] << endl;
        return EXIT_FAILURE;
    }
    map<string, int> identifiers;
    map<string, int> const_values;
    map<string, int> string_literals;
    vector<string> identifiers_list, const_values_list, string_literals_list;
    vector<TokenStruct> tokens;
    string line, word;
    int lineNumber = 1, id = 0, txt = 0;
    bool inString = false;
    while (getline(inputFile, line)) {
        line += " ";
        for (char c : line) {
            if (c == '#') {
                break;
            }
            if (!isspace(c) || inString) {
                word += c;
            }

            if (c == '"') {
                inString = !inString;
                if (!inString) {
                    string_literals_list.push_back(word);
                    string_literals.insert({word, ++txt});
                    tokens.push_back({STRING_LITERAL, word, lineNumber});
                    word = "";
                    break;
                }
            }

            if (isspace(c) && !inString) {
                if (word == "PROGRAMA" || word == "FINPROG" || word == "MIENTRAS" || word == "ENTONCES" || word == "SI"
                    || word == "SINO" || word == "LEE" || word == "FINSI" || word == "IMPRIME" || word == "FINM"
                    || word == "HACER") {
                    tokens.push_back({KEYWORDS, word, lineNumber});
                }else if (isNumeric(word)) {
                    if (const_values.find(word) == const_values.end()) {
                        const_values_list.push_back(word);
                    }
                    const_values.insert({word, hexToInt(word)});
                    tokens.push_back({CONSTANTS, word, lineNumber});
                }else if(word == "+" || word == "-" || word == "*" || word == "/") {
                    tokens.push_back({AR_OP, word, lineNumber});
                }else if(word == ">" || word == "<" || word == "<>" || word == ">=" || word == "<=" || word == "==" || word == "!="){
                    tokens.push_back({REL_OP, word, lineNumber});
                }else if(word == "=") {
                    tokens.push_back({ASIGNMENT, word, lineNumber});
                }else if (isalpha(word[0])){
                    bool isIdentifier = true;
                    for (const char idC : word) {
                        isIdentifier = isIdentifier && isalnum(idC);
                        if (!isIdentifier) {
                            cerr << "invalid token: \"" << word << "\" in line: "<< lineNumber <<endl;
                            return EXIT_FAILURE;
                        }
                    }
                    if (identifiers.find(word) == identifiers.end()) {
                        identifiers_list.push_back(word);
                    }
                    identifiers.insert({word, ++id});
                    tokens.push_back({IDENTIFIER, word, lineNumber});
                }else {
                    cerr << "invalid token: \"" << word <<"\" in line: "<< lineNumber << endl;
                    return EXIT_FAILURE;
                }
                word = "";
            }
        }
        if (inString) {
            cerr << "missing closing quote" << " in line :" << lineNumber << endl;
            return EXIT_FAILURE;
        }
        word = "";
        lineNumber++;
    }
    for (const TokenStruct& token: tokens) {
        cout << to_string(token) << ": " << left << setw(30) << token.value << "line: " << right << setw(3) << token.line <<
                endl;
    }
    cout << endl <<"IDS" <<endl;
    for (const string& k: identifiers_list) {
        cout << k << ",ID" << setw(2) << setfill('0') << identifiers.at(k)<< endl;
    }
    cout << endl <<"TXT" <<endl;
    for (const string& k: string_literals_list) {
        cout << k << ",TXT" << setw(2)<< setfill('0')<< string_literals.at(k)<< endl;
    }
    cout << endl <<"VAL" <<endl;
    for (const string& k: const_values_list) {
        cout << k << ": " << const_values.at(k) << endl;
    }
    return (EXIT_SUCCESS);
}