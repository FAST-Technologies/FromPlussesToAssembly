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
#include "./../Constants.h"
#include "../SymbolTableV2_0.h"
#include "../ConstantTableV2_0/ConstantTableV2_0.h"
#include "../VariableTableV2_0/VariableTableV2_0.h"
#include "../OutputModuleV2_0/OutputModuleV2_0.h"

class Scanner {
private:
    ConstantTableV2_0& keywords;
    ConstantTableV2_0& separators;
    VariableTableV2_0& identifiers;
    VariableTableV2_0& constants;
    std::ofstream tokenFile;

    // Вспомогательные методы
    bool isLetter(char c);
    bool isDigit(char c);
    bool isOperator(char c);
    std::string getTokenType(const std::string& token);
    void processToken(const std::string& token, size_t& pos, const std::string& code);

public:
    Scanner(ConstantTableV2_0& kw, ConstantTableV2_0& sep,
            VariableTableV2_0& id, VariableTableV2_0& con);
    ~Scanner();
    void processCode(const std::string& code);
    void printTableContents(ofstream& logger);
};
//class Scanner {
//private:
//    ConstantTableV2_0 keywords;
//    ConstantTableV2_0 delimiters;
//    VariableTableV2_0 identifiers;
//    VariableTableV2_0 constants;
//    ofstream tokenFile;
//public:
//    Scanner() :
//            keywords(GlobalPaths::KeyWordsTable, (ofstream &) GlobalPaths::KeyWordsLogFile),
//            delimiters(GlobalPaths::SeparatorsTable, (ofstream &) GlobalPaths::SeparatorsLogFile),
//            identifiers(20),
//            constants(20),
//            tokenFile(GlobalPaths::TokenFile) {
//        if (!tokenFile.is_open()) {
//            cerr << "Error: Could not open tokens.txt" << endl;
//        }
//    }
//
//    ~Scanner() {
//        if (tokenFile.is_open()) {
//            tokenFile.close();
//        }
//    }
//
//    Token analyzeToken(const string& token) {
//        int keywordIndex = keywords.getIndex(token);
//        if (keywordIndex != -1) {
//            saveToken({ KeyWord, keywordIndex, -1 });
//            return { KeyWord, keywordIndex, -1 };
//        }
//        int delimiterIndex = delimiters.getIndex(token);
//        if (delimiterIndex != -1) {
//            saveToken({ Separator, delimiterIndex, -1 });
//            return { Separator, delimiterIndex, -1 };
//        }
//        if (isdigit(token[0])) {
//            int value = stoi(token);
//            constants.addLexeme(token[0], 40);
//            auto pos = constants.getPosition(Constant(value));
//            saveToken({ Constant, pos.first, pos.second });
//            return { Constant, pos.first, pos.second };
//        }
//        identifiers.add(Identifier(token));
//        auto pos = identifiers.getPosition(Identifier(token));
//        saveToken({ Identifier, pos.first, pos.second });
//        return { Identifier, pos.first, pos.second };
//    }
//
//    void processCode(const string& code) {
//        string token;
//        bool inFunction = false;
//        vector<string> params;
//
//        for (char ch : code) {
//            if (isalnum(ch) || ch == '_') {
//                token += ch;
//            }
//            else {
//                if (!token.empty()) {
//                    Token t = analyzeToken(token);
//                    printToken(t, token);
//                    if (t.type == KeyWord && token == "int" && !inFunction) {
//                        inFunction = true;
//                    }
//                    else if (inFunction && t.type == Identifier) {
//                        if (ch == '(') {
//                            identifiers.setAttribute(t.tableIndex, "function", params);
//                            params.clear();
//                        }
//                        else if (ch != ')') {
//                            params.push_back(token);
//                        }
//                    }
//                    token.clear();
//                }
//                if (!isspace(ch)) {
//                    string delim(1, ch);
//                    Token t = analyzeToken(delim);
//                    printToken(t, delim);
//                    if (delim == ")") inFunction = false;
//                }
//            }
//        }
//        if (!token.empty()) {
//            Token t = analyzeToken(token);
//            printToken(t, token);
//        }
//    }
//
//    void saveToken(const Token& t) {
//        if (tokenFile.is_open()) {
//            tokenFile << t.type << " " << t.tableIndex << " " << t.hashIndex << endl;
//        }
//    }
//
//    void printToken(const Token& t, const string& value) {
//        cout << "Token: " << value << " Type: " << t.type;
//        if (t.tableIndex >= 0) {
//            cout << " Table Index: " << t.tableIndex;
//        }
//        if (t.hashIndex >= 0) {
//            cout << " Hash Index: " << t.hashIndex;
//        }
//        cout << endl;
//    }
//
//    void printTableContents() {
//        cout << "Keywords:" << endl;
//        vector<ConstantTableEntry> KeyWordsTable = keywords.getTable();
//        int key_index = 0;
//        for (const auto& entry : KeyWordsTable) {
//            key_index += 1;
//            outputConstantTableEntry(key_index, entry, (ofstream &) GlobalPaths::KeyWordsTable);
//        }
//
//        cout << "\nDelimiters:" << endl;
//        vector<ConstantTableEntry> SeparatorsTable = delimiters.getTable();
//        int key_index1 = 0;
//        for (const auto& entry : SeparatorsTable) {
//            key_index1 += 1;
//            outputConstantTableEntry(key_index1, entry, (ofstream &) GlobalPaths::SeparatorsTable);
//        }
//
//        cout << "\nIdentifiers:" << endl;
//        for (int i = 0; i < identifiers.size(); i++) {
//            Identifier id = identifiers.get(i);
//            cout << id.name << " (" << id.type;
//            if (!id.params.empty()) {
//                cout << ", params: ";
//                for (const auto& p : id.params) cout << p << " ";
//            }
//            cout << ") ";
//        }
//        cout << "\nConstants:" << endl;
//        for (int i = 0; i < constants.size(); i++) {
//            Constant c = constants.get(i);
//            cout << c.value << " (" << c.type << ") ";
//        }
//        cout << endl;
//    }
//};


#endif //FROMPLUSESTOASSEMBLYLAB1_SCANNER_H
