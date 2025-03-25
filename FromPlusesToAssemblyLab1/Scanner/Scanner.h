#ifndef FROMPLUSESTOASSEMBLYLAB1_SCANNER_H
#define FROMPLUSESTOASSEMBLYLAB1_SCANNER_H

/// <summary>
/// [EN] Libraries that we need to use
/// [RU] Библиотеки, необходимые для использования
/// </summary>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <limits>
#include <sstream>
#include <iostream>
#include <regex>
#include <sstream>  // Для stringstream
#include "./../Constants.h"
#include "../SymbolTableV2_0.h"
#include "../ConstantTableV2_0/ConstantTableV2_0.h"
#include "../VariableTableV2_0/VariableTableV2_0.h"
#include "../OutputModuleV2_0/OutputModuleV2_0.h"

using namespace std;

struct Declaration {
    string type;
    string name;
    string value;
    string templateType;
};

struct StateType {
    int PERM_ERROR = -1;
    int LEXIC_ERROR = -2;
    int TYPE_ERROR = -3;
    int START = 1;
    int IN_KEY = 10;
    int IN_SEP = 20;
    int IN_IDENT = 30;
    int IN_NUM = 40;
};

class Scanner {
private:
    ConstantTableV2_0& keywords;
    ConstantTableV2_0& separators;
    VariableTableV2_0& identifiers;
    VariableTableV2_0& constants;
    ofstream tokenFile;
    ofstream errorFile;

public:
    Scanner(ConstantTableV2_0& kw,
            ConstantTableV2_0& sep,
            VariableTableV2_0& id,
            VariableTableV2_0& con);
    ~Scanner();

    // Анализирует токен и возвращает его тип, индекс в таблице и тип таблицы
    Token analyzeToken(const string& token,
                       ofstream& errorLogger);

    void processCode(const string& code,
                     const string& errorFile);

    vector<string> splitBySpace(const string str);

    void saveToken(const Token& t);

    static void printToken(const Token& t, const string& value);

    void printTableContents(ofstream& logger);

    vector<Declaration> recognizeType(const string& line,
                                             ofstream& errorLogger,
                                             const string& token);

    static vector<string> splitParameters(const string& paramList);

    static void parseParameter(const string& param,
                               string& paramType,
                               string& paramName,
                               string& paramTemplateType);

    static LexemeType stringToLexemeType(const string& typeStr);

    static void errorOutput(const string& message,
                            ofstream& errorLogger,
                            basic_string<char> token);

    int levenshteinDistance(const string& s1, const string& s2);
//
//    // Вспомогательные методы
//    bool isLetter(char c);
//    bool isDigit(char c);
//    bool isOperator(char c);
//    std::string getTokenType(const string& token);
//    void processToken(const string& token,
//                      size_t& pos,
//                      const string& code);
};



#endif //FROMPLUSESTOASSEMBLYLAB1_SCANNER_H
