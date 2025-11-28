#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
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

    vector<TokenStruct> tokens;
    string line, word, text;
    int lineNumber = 1;
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
                    text += word;
                    tokens.push_back({STRING_LITERAL, text.substr(1, text.size() - 2), lineNumber});
                    text = "";
                }
            }

            if (isspace(c) && !inString) {
                if (word == "PROGRAMA" || word == "FINPROG" || word == "MIENTRAS" || word == "ENTONCES" || word == "SI"
                    || word == "SINO" || word == "LEE" || word == "FINSI" || word == "IMPRIME" || word == "FINM"
                    || word == "HACER") {
                    tokens.push_back({KEYWORDS, word, lineNumber});
                }else if (isNumeric(word)) {
                    tokens.push_back({CONSTANTS, word, lineNumber});
                }else if (isalpha(word[0])){
                    tokens.push_back({IDENTIFIER, word, lineNumber});
                }
                word = "";
            }
        }
        if (inString) {
            cerr << "missing closing quote" << endl;
            return EXIT_FAILURE;
        }
        word = "";
        lineNumber++;
    }
    for (const TokenStruct token: tokens) {
        cout << token.type << ' ' << left << setw(30) << token.value << "line: " << right << setw(3) << token.line <<
                endl;
    }
    return (EXIT_SUCCESS);
}