#include "Scanner.h"
#include "../VariableTableV2_0/VariableTableV2_0.h"
#include <iostream>
#include <cctype>

Scanner::Scanner(ConstantTableV2_0& kw, ConstantTableV2_0& sep,
                 VariableTableV2_0& id, VariableTableV2_0& con)
        : keywords(kw), separators(sep), identifiers(id), constants(con) {
    tokenFile.open("token.txt");
    if (!tokenFile.is_open()) {
        throw std::runtime_error("Unable to open token.txt for writing");
    }
}

Scanner::~Scanner() {
    if (tokenFile.is_open()) {
        tokenFile.close();
    }
}

bool Scanner::isLetter(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Scanner::isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Scanner::isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' ||
            c == '=' || c == '^' || c == '<' || c == '>' || c == '!');
}

std::string Scanner::getTokenType(const std::string& token) {
    if (keywords.contains(token)) return "Keyword";
    if (separators.contains(token)) return "Separator";

    // Проверка на числовую константу
    bool hasDecimal = false;
    bool isNumber = true;
    for (char c : token) {
        if (c == '.') {
            if (hasDecimal) {
                isNumber = false;
                break;
            }
            hasDecimal = true;
        } else if (!isDigit(c) && c != '-') {
            isNumber = false;
            break;
        }
    }
    if (isNumber) return "Constant";

    // Проверка на булево значение
    if (token == "true" || token == "false") return "Constant";

    // Если ничего не подошло - это идентификатор
    return "Identifier";
}

void Scanner::processCode(const std::string& code) {
    std::string token;
    size_t i = 0;

    while (i < code.length()) {
        char c = code[i];

        // Пропускаем пробелы
        if (std::isspace(static_cast<unsigned char>(c))) {
            if (!token.empty()) {
                processToken(token, i, code);
                token.clear();
            }
            i++;
            continue;
        }

        // Обработка операторов и разделителей
        if (isOperator(c) || separators.contains(std::string(1, c))) {
            if (!token.empty()) {
                processToken(token, i, code);
                token.clear();
            }
            std::string op(1, c);
            if (separators.contains(op)) {
                int index = separators.getIndex(op);
                tokenFile << Separator << " " << index << std::endl;
            } else {
                int lexemeCode = Constant;
                constants.addLexeme(op, LexemeType::Char);
                size_t hashIndex = constants.hash(op);
                tokenFile << lexemeCode << " " << hashIndex << std::endl;
            }
            i++;
            continue;
        }

        // Собираем токен
        token += c;
        i++;
    }

    // Обработка последнего токена
    if (!token.empty()) {
        processToken(token, i, code);
    }
}

void Scanner::processToken(const std::string& token, size_t& pos, const std::string& code) {
    std::string type = getTokenType(token);

    if (type == "Keyword") {
        int index = keywords.getIndex(token);
        tokenFile << KeyWord << " " << index << std::endl;
    }
    else if (type == "Separator") {
        int index = separators.getIndex(token);
        tokenFile << Separator << " " << index << std::endl;
    }
    else if (type == "Constant") {
        LexemeType lexType;
        int lexemeCode = Constant;
        if (token == "true" || token == "false") {
            lexType = LexemeType::Bool;
        } else if (token.find('.') != std::string::npos) {
            lexType = LexemeType::Float;
        } else {
            lexType = LexemeType::Int;
        }
        int lexemeTypeSize = 0;
        switch (lexType) {
            case LexemeType::Bool: lexemeTypeSize = sizeof(bool); break;
            case LexemeType::Float: lexemeTypeSize = sizeof(float); break;
            case LexemeType::Int: lexemeTypeSize = sizeof(int); break;
        }

        constants.addLexeme(token, lexType);
        constants.set_code(token, lexemeCode);
        constants.set_size(token, lexemeTypeSize);
        size_t hashIndex = constants.hash(token);
        tokenFile << lexemeCode << " " << hashIndex << std::endl;
    }
    else if (type == "Identifier") {
        LexemeType lexType = LexemeType::Undefined;
        int lexemeCode = Identifier; // 30

        // Простой контекстный анализ для определения типа
        size_t prevPos = pos - token.length() - 1;
        std::string prevToken;
        while (prevPos > 0 && std::isspace(code[prevPos])) prevPos--;
        while (prevPos > 0 && !std::isspace(code[prevPos]) &&
               !separators.contains(std::string(1, code[prevPos]))) {
            prevToken = code[prevPos] + prevToken;
            prevPos--;
        }

        if (prevToken == "int") lexType = LexemeType::Int;
        else if (prevToken == "float") lexType = LexemeType::Float;
        else if (prevToken == "bool") lexType = LexemeType::Bool;

        int lexemeTypeSize = 0;
        switch (lexType) {
            case LexemeType::Int: lexemeTypeSize = sizeof(int); break;
            case LexemeType::Float: lexemeTypeSize = sizeof(float); break;
            case LexemeType::Bool: lexemeTypeSize = sizeof(bool); break;
            default: lexemeTypeSize = sizeof(int); break; // по умолчанию
        }

        identifiers.addLexeme(token, lexType);
        identifiers.set_code(token, lexemeCode);
        identifiers.set_size(token, lexemeTypeSize);
        size_t hashIndex = identifiers.hash(token);
        tokenFile << lexemeCode << " " << hashIndex << std::endl;
    }
}

void Scanner::printTableContents(ofstream& logger) {
    std::cout << "Keywords Table (lexemeCode = " << KeyWord << "):" << std::endl;
    for (const auto& entry : keywords.getTable()) {
        std::cout << "Symbol: " << entry.symbol
                  << ", Type: " << ConstantTableV2_0::lexemeTypeToString(entry.type)
                  << ", Code: " << entry.lexemeCode
                  << ", Size: " << entry.lexemeTypeSize << std::endl;
    }

    std::cout << "\nSeparators Table (lexemeCode = " << Separator << "):" << std::endl;
    for (const auto& entry : separators.getTable()) {
        std::cout << "Symbol: " << entry.symbol
                  << ", Type: " << ConstantTableV2_0::lexemeTypeToString(entry.type)
                  << ", Code: " << entry.lexemeCode
                  << ", Size: " << entry.lexemeTypeSize << std::endl;
    }

    std::cout << "\nIdentifiers Table (lexemeCode = " << Identifier << "):" << std::endl;
    identifiers.printTable(logger);

    std::cout << "\nConstants Table (lexemeCode = " << Constant << "):" << std::endl;
    constants.printTable(logger);
}
