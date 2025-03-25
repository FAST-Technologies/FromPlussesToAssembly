#include "Scanner.h"
#include "../VariableTableV2_0/VariableTableV2_0.h"
#include "../ConstantTableV2_0/ConstantTableV2_0.h"
#include "../Constants.h"
#include <iostream>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>

Scanner::~Scanner()
{
    if (tokenFile.is_open()) {
        tokenFile.close();
    }
    if (errorFile.is_open()) {
        errorFile.close();
    }
    cout << "Object Scanner is deleted" << endl;
}

vector<string> Scanner::splitBySpace(const string str)
{
    vector<string> tokens;
    istringstream iss(str);
    string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void Scanner::errorOutput(const string& message,
                          ofstream& errorLogger,
                          basic_string<char> token)
{
    errorLogger << "Error: " << message << endl;
    errorLogger << "Illegal token: " << token << endl;
}

LexemeType Scanner::stringToLexemeType(const string& typeStr)
{
    if (typeStr == "Int") return LexemeType::Int;
    if (typeStr == "Float") return LexemeType::Float;
    if (typeStr == "Double") return LexemeType::Double;
    if (typeStr == "Char") return LexemeType::Char;
    if (typeStr == "Bool") return LexemeType::Bool;
    if (typeStr == "String") return LexemeType::String;
    if (typeStr == "WString") return LexemeType::WString;
    if (typeStr == "Vector") return LexemeType::Vector;
    if (typeStr == "Array") return LexemeType::Array;
    if (typeStr == "List") return LexemeType::List;
    if (typeStr == "Unordered_map") return LexemeType::Unordered_map;
    if (typeStr == "Forward_list") return LexemeType::Forward_list;
    if (typeStr == "Deque") return LexemeType::Deque;
    if (typeStr == "Queue") return LexemeType::Queue;
    if (typeStr == "Priority_queue") return LexemeType::Priority_queue;
    if (typeStr == "Stack") return LexemeType::Stack;
    if (typeStr == "Map") return LexemeType::Map;
    if (typeStr == "Set") return LexemeType::Set;
    if (typeStr == "Unordered_set") return LexemeType::Unordered_set;
    if (typeStr == "Multimap") return LexemeType::Multimap;
    if (typeStr == "Multiset") return LexemeType::Multiset;
    if (typeStr == "Bitset") return LexemeType::Bitset;
    if (typeStr == "Pair") return LexemeType::Pair;
    if (typeStr == "Tuple") return LexemeType::Tuple;
    if (typeStr == "Optional") return LexemeType::Optional;
    if (typeStr == "Variant") return LexemeType::Variant;
    if (typeStr == "Function") return LexemeType::Function;
    return LexemeType::Undefined;
}

void Scanner::parseParameter(const string& param,
                             string& paramType,
                             string& paramName,
                             string& paramTemplateType)
{
    regex int_param_pattern(R"((?:const\s+)?int(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex float_param_pattern(R"((?:const\s+)?float(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex double_param_pattern(R"((?:const\s+)?double(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex char_param_pattern(R"((?:const\s+)?char(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex bool_param_pattern(R"((?:const\s+)?bool(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex string_param_pattern(R"((?:const\s+)?(?:std::)?(?:w)?string(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex vector_param_pattern(R"((?:const\s+)?(?:std::)?vector<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    regex map_param_pattern(R"((?:const\s+)?(?:std::)?map<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*))");

    smatch match;
    if (regex_match(param, match, int_param_pattern)) {
        paramType = "Int";
        paramName = match[1].str();
    } else if (regex_match(param, match, float_param_pattern)) {
        paramType = "Float";
        paramName = match[1].str();
    } else if (regex_match(param, match, double_param_pattern)) {
        paramType = "Double";
        paramName = match[1].str();
    } else if (regex_match(param, match, char_param_pattern)) {
        paramType = "Char";
        paramName = match[1].str();
    } else if (regex_match(param, match, bool_param_pattern)) {
        paramType = "Bool";
        paramName = match[1].str();
    } else if (regex_match(param, match, string_param_pattern)) {
        paramType = (param.find("wstring") != string::npos) ? "WString" : "String";
        paramName = match[1].str();
    } else if (regex_match(param, match, vector_param_pattern)) {
        paramType = "Vector";
        paramTemplateType = match[1].str();
        paramName = match[2].str();
    } else if (regex_match(param, match, map_param_pattern)) {
        paramType = "Map";
        paramTemplateType = match[1].str();
        paramName = match[3].str();
    } else {
        paramType = "Unknown";
        paramName = param;
    }
}

vector<string> Scanner::splitParameters(const string& paramList)
{
    vector<string> params;
    if (paramList.empty()) {
        return params;
    }

    // Split on commas, but avoid commas inside template types (e.g., std::pair<int, int>)
    regex param_splitter(R"(,\s*(?![^<]*>))");
    sregex_token_iterator iter(paramList.begin(), paramList.end(), param_splitter, -1);
    sregex_token_iterator end;
    while (iter != end) {
        string param = iter->str();
        // Trim whitespace
        param.erase(0, param.find_first_not_of(" \t"));
        param.erase(param.find_last_not_of(" \t") + 1);
        if (!param.empty()) {
            params.push_back(param);
        }
        ++iter;
    }
    return params;
}

// Функция для распознавания типов переменных
vector<Declaration> Scanner::recognizeType(const string& line,
                                           ofstream& errorLogger,
                                           const string& token)
{

    vector<Declaration> declarations;
    smatch match;

    string message;
    regex int_pattern(R"((?:const\s+)?int(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([^;]*))?;)");
    regex float_pattern(R"((?:const\s+)?float(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([^;]*))?;)");
    regex double_pattern(R"((?:const\s+)?double(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([^;]*))?;)");
    regex char_pattern(R"((?:const\s+)?char(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*'(.*)')?;)");
    regex bool_pattern(R"((?:const\s+)?bool(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(true|false))?;)");
    regex string_pattern(R"((?:const\s+)?(?:std::)?(?:w)?string(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(.+))?;)");

    regex vector_pattern(R"((?:const\s+)?(?:std::)?vector<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex array_pattern(R"((?:const\s+)?(?:std::)?array<([^,]+),\s*(\d+)\s*>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex list_pattern(R"((?:const\s+)?(?:std::)?list<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([a-zA-Z_][a-zA-Z0-9_]*|\{.*\}))?;)");
    regex unordered_map_pattern(R"((?:const\s+)?(?:std::)?unordered_map<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex forward_list_pattern(R"((?:const\s+)?(?:std::)?forward_list<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex deque_pattern(R"((?:const\s+)?(?:std::)?deque<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex queue_pattern(R"((?:const\s+)?(?:std::)?queue<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*);)");
    regex priority_queue_pattern(R"((?:const\s+)?(?:std::)?priority_queue<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*);)");
    regex stack_pattern(R"((?:const\s+)?(?:std::)?stack<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*);)");
    regex map_pattern(R"((?:const\s+)?(?:std::)?map<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex set_pattern(R"((?:const\s+)?(?:std::)?set<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex unordered_set_pattern(R"((?:const\s+)?(?:std::)?unordered_set<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex multimap_pattern(R"((?:const\s+)?(?:std::)?multimap<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex multiset_pattern(R"((?:const\s+)?(?:std::)?multiset<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex bitset_pattern(R"((?:const\s+)?(?:std::)?bitset<(\d+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*\((.*?)\))?;)");
    regex pair_pattern(R"((?:const\s+)?(?:std::)?pair<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*\((.*)\))?;)");
    regex tuple_pattern(R"((?:const\s+)?(?:std::)?tuple<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*\((.*)\))?;)");
    regex optional_pattern(R"((?:const\s+)?(?:std::)?optional<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(.*))?;)");
    regex variant_pattern(R"((?:const\s+)?(?:std::)?variant<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(.*))?;)");

    // Function Declaration Patterns
    regex int_function_pattern(R"((?:const\s+)?int(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex float_function_pattern(R"((?:const\s+)?float(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex double_function_pattern(R"((?:const\s+)?double(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex char_function_pattern(R"((?:const\s+)?char(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex bool_function_pattern(R"((?:const\s+)?bool(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex string_function_pattern(R"((?:const\s+)?(?:std::)?(?:w)?string(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex vector_function_pattern(R"((?:const\s+)?(?:std::)?vector<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex map_function_pattern(R"((?:const\s+)?(?:std::)?map<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)(?:\s*(?:const|noexcept|override|final))*\s*(?:;|\{))");
    regex method_call_pattern(R"(([a-zA-Z_][a-zA-Z0-9_]*)\.([a-zA-Z_][a-zA-Z0-9_]*)\s*\(([^)]*)\)\s*;)");

//    vector<string> tokens = splitBySpace(line);
//    cout << "Current line: " << line << endl;
//    if (tokens.size()>1) {
//        string token1 = tokens[0];
//        string token2 = tokens[1];
//        transform(token1.begin(), token1.end(), token1.begin(), ::tolower);
//        transform(token2.begin(), token2.end(), token2.begin(), ::tolower);
//
//        if((keywords.binarySearch(token1)==-1 && line.find('=')!=std::string::npos) || (keywords.binarySearch(token1)!=-1 && keywords.binarySearch(token1)==-1 && line.find('=')!=std::string::npos)) {
//            cout << "Error line: " << line << endl;
//            cout << "Token1: " << token1 << endl;
//            cout << "Token2: " << token2 << endl;
//        }
//    }

//    if (tokens[0]!=string::find()) {
//
//    }
//    if (tokens[0]=="const") {
//
//    }

    // Check for method calls
    if (regex_search(line, match, method_call_pattern)) {
        // The object (e.g., myQueue2)
        string objectName = match[1].str();
        // The method (e.g., push)
        Declaration methodDecl;
        methodDecl.type = "Function";
        methodDecl.name = match[2].str();
        methodDecl.value = match[3].str(); // Parameters (e.g., 12)
        declarations.push_back(methodDecl);
        return declarations;
    }
    // Check for function declarations first
    if (regex_search(line, match, int_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Int";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        // Parse parameters
        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, float_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Float";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, double_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Double";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, char_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Char";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, bool_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Bool";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, string_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = (line.find("wstring") != string::npos) ? "WString" : "String";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, vector_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Vector";
        funcDecl.name = match[2].str();
        funcDecl.templateType = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[3].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }
    if (regex_search(line, match, map_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "Map";
        funcDecl.name = match[3].str();
        funcDecl.templateType = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[4].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "Unknown") {
                Declaration paramDecl;
                paramDecl.type = paramType;
                paramDecl.name = paramName;
                paramDecl.value = "";
                paramDecl.templateType = paramTemplateType;
                declarations.push_back(paramDecl);
            }
        }
        return declarations;
    }

    if (regex_search(line, match, unordered_map_pattern)) {
        Declaration decl;
        decl.type = "Unordered_map";
        decl.templateType = match[1].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, forward_list_pattern)) {
        Declaration decl;
        decl.type = "Forward_list";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }

    if (regex_search(line, match, priority_queue_pattern)) {
        Declaration decl;
        decl.type = "Priority_queue";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = "";
        declarations.push_back(decl);
        return declarations;
    }

    if (regex_search(line, match, unordered_set_pattern)) {
        Declaration decl;
        decl.type = "Unordered_set";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, multimap_pattern)) {
        Declaration decl;
        decl.type = "Multimap";
        decl.templateType = match[2].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, multiset_pattern)) {
        Declaration decl;
        decl.type = "Multiset";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    // Check for variable declarations
    if (regex_search(line, match, int_pattern)) {
        Declaration decl;
        int flag = 0;
        if (line.find('&')==std::string::npos) {
            string firstVal = match[1].str();
            string secondVal = match[2].str();
            if (isdigit(firstVal[0]) || (firstVal[0]=='_' && isdigit(firstVal[1]))) {
                message = "Error: not correct first variable name for current type";
                cout << message << endl;
                errorOutput(message, errorLogger, firstVal);
                flag = -1;
            }
            else if (!isdigit(secondVal[0]) && secondVal[0] != '-' || (secondVal[0] == '-' && !isdigit(secondVal[1]))) {
                message = "Error: not correct second variable name for to current type";
                cout << message << endl;
                errorOutput(message, errorLogger, secondVal);
                flag = -1;
            }
            else if (isdigit(secondVal[0]) || (secondVal[0] == '-' && isdigit(secondVal[1]))) {
                for (char c: secondVal) {
                    if (!isdigit(c)) {
                        message = "Error: not correct second variable name for to current type";
                        cout << message << endl;
                        errorOutput(message, errorLogger, secondVal);
                        flag = -1;
                        break;
                    }
                    else if (flag == -1) {
                        break;
                    }
                }
            }
            if (flag != -1) {
                decl.type = "Int";
                decl.name = match[1].str();
                decl.value = match[2].matched ? match[2].str() : "";
                declarations.push_back(decl);
            }
        }
        else {
            decl.type = "Int";
            decl.name = match[1].str();
            decl.value = match[2].matched ? match[2].str() : "";
            declarations.push_back(decl);
        }
        return declarations;
    }

    if (regex_search(line, match, float_pattern)) {
        Declaration decl;
        decl.type = "Float";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, double_pattern)) {
        Declaration decl;
        decl.type = "Double";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, char_pattern)) {
        Declaration decl;
        decl.type = "Char";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, bool_pattern)) {
        Declaration decl;
        decl.type = "Bool";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
//    if (regex_search(line, match, string_pattern)) {
//        Declaration decl;
//        decl.name = match[1].str();
//        decl.value = match[2].matched ? match[2].str() : "";
//
//        // Проверяем, есть ли в строке "wstring" или "Wstring"
//        if (line.find("wstring") != string::npos || line.find("Wstring") != string::npos) {
//            decl.type = "Wstring";
//            if (decl.value.length() >= 3 && decl.value[0] == 'L' && decl.value[1] == '"' && decl.value.back() == '"') {
//                decl.value = decl.value.substr(2, decl.value.length() - 3);
//            }
//        } else {
//            decl.type = "String";
//            if (decl.value.length() >= 2 && decl.value[0] == '"' && decl.value.back() == '"') {
//                decl.value = decl.value.substr(1, decl.value.length() - 2);
//            }
//        }
//        declarations.push_back(decl);
//        return declarations;
//    }
    if (regex_search(line, match, string_pattern)) {
        Declaration decl;
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";

        // Проверяем, есть ли в строке "wstring"
        if (line.find("wstring") != string::npos) {
            decl.type = "WString"; // Используем правильный формат для stringToLexemeType
            if (decl.value.length() >= 3 && decl.value[0] == 'L' && decl.value[1] == '"' && decl.value.back() == '"') {
                decl.value = decl.value.substr(2, decl.value.length() - 3);
            }
        } else {
            decl.type = "String";
            if (decl.value.length() >= 2 && decl.value[0] == '"' && decl.value.back() == '"') {
                decl.value = decl.value.substr(1, decl.value.length() - 2);
            }
        }
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, vector_pattern)) {
        Declaration decl;
        decl.type = "Vector";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, array_pattern)) {
        Declaration decl;
        decl.type = "Array";
        decl.templateType = match[1].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, list_pattern)) {
        Declaration decl;
        decl.type = "List";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }

    if (regex_search(line, match, deque_pattern)) {
        Declaration decl;
        decl.type = "Deque";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, queue_pattern)) {
        Declaration decl;
        decl.type = "Queue";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = "";
        declarations.push_back(decl);
        return declarations;
    }

    if (regex_search(line, match, stack_pattern)) {
        Declaration decl;
        decl.type = "Stack";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, map_pattern)) {
        Declaration decl;
        decl.type = "Map";
        decl.templateType = match[1].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, set_pattern)) {
        Declaration decl;
        decl.type = "Set";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }

    if (regex_search(line, match, bitset_pattern)) {
        Declaration decl;
        decl.type = "Bitset";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, pair_pattern)) {
        Declaration decl;
        decl.type = "Pair";
        decl.templateType = match[2].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, tuple_pattern)) {
        Declaration decl;
        decl.type = "Tuple";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, optional_pattern)) {
        Declaration decl;
        decl.type = "Optional";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, variant_pattern)) {
        Declaration decl;
        decl.type = "Variant";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }

    return declarations;
}

Scanner::Scanner(ConstantTableV2_0& kw,
                 ConstantTableV2_0& sep,
                 VariableTableV2_0& id,
                 VariableTableV2_0& con)
        : keywords(kw),
          separators(sep),
          identifiers(id),
          constants(con),
          tokenFile(GlobalPaths::TokenFile),
          errorFile(GlobalPaths::ErrorFile) {
    if (!tokenFile.is_open()) {
        cerr << "Error: Could not open " << GlobalPaths::TokenFile << " for writing tokens!" << endl;
        throw runtime_error("Failed to open file " + GlobalPaths::TokenFile);
    }
    if (!errorFile.is_open()) {
        cerr << "Error: Could not open " << GlobalPaths::ErrorFile << " for writing tokens!" << endl;
        throw runtime_error("Failed to open token file " + GlobalPaths::ErrorFile);
    }
}

int Scanner::levenshteinDistance(const string& s1, const string& s2) {
    int m = s1.length();
    int n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = min({dp[i - 1][j - 1] + 1, // замена
                                dp[i - 1][j] + 1,     // удаление
                                dp[i][j - 1] + 1});   // вставка
            }
        }
    }
    return dp[m][n];
}

// Анализирует токен и возвращает его тип, индекс в таблице и тип таблицы
Token Scanner::analyzeToken(const string& token,
                            ofstream& errorLogger)
{
    if (keywords.contains(token)) {
        int index = keywords.getIndex(token);
        Token KeyWord = { Constants::KeyWord, index, -1 };
        saveToken(KeyWord);
        return KeyWord;
    }

    if (separators.contains(token)) {
        int index = separators.getIndex(token);
        Token Separator = { Constants::Separator, index, -1 };
        saveToken(Separator);
        return Separator;
    }

    if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1 && isdigit(token[1]))) {
        for (char c : token) {
            if (!isdigit(c) && c != '.') {
                const string message = "Error: You tried to use string/char except of int/double/float!";
                errorOutput(message, errorLogger, const_cast<string &>(token));
            }
        }
        LexemeType type = VariableTableV2_0::determineLexemeType(token);
        int lexemeCode = Constants::Constant;
        int lexemeTypeSize = VariableTableV2_0::lexemeSize(VariableTableV2_0::lexemeTypeToString(type), token);
        constants.addLexeme(token, type);

        vector<string> constNames = constants.get_all_names();
        int index = -1;
        for (size_t i = 0; i < constNames.size(); ++i) {
            if (constNames[i] == token) {
                index = static_cast<int>(i);
                break;
            }
        }
        Token Const = { Constants::Constant, index, 0 };
        saveToken(Const);
        return Const;
    }

    // Не добавляем идентификатор в таблицу здесь, это сделает processCode после recognizeType
    int lexemeCode = Constants::Identifier;
    int index = -1;
    Token Identificatr = { Constants::Identifier, index, 0 };
    saveToken(Identificatr);
    return Identificatr;
}

// Обрабатывает входной код
void Scanner::processCode(const string& code,
                          const string& errorFile)
{
    auto errorLogFile = ofstream(errorFile);
    if (!errorLogFile.is_open()) {
        throw runtime_error("Error occurred while opening " + errorFile);
    }
    string token;
    string currentLine;
    string currentExpression;
    bool inFunction = false;
    bool inNumber = false;
    bool hasDot = false;
    vector<string> params;
    Declaration currentDecl;
    bool hasType = false;
    bool inMethodCall = false;
    stack<char> bracketStack;
    int lineNumber = 1;

    vector<string> keywordsToSkip = {"cout", "return", "for", "while", "if", "else", "switch", "case", "break", "continue"};

    for (size_t i = 0; i < code.length(); ++i) {
        char ch = code[i];

        if (ch == '\n') {
            lineNumber++;
            continue;
        }

        if (ch == '(' || ch == '{' || ch == '[') {
            bracketStack.push(ch);
        }
        else if (ch == ')' || ch == '}' || ch == ']') {
            if (bracketStack.empty()) {
                string message = "Unmatched closing bracket '" + string(1, ch) + "' at line " + to_string(lineNumber);
                errorOutput(message, errorLogFile, string(1, ch));
            }
            else {
                char top = bracketStack.top();
                if ((ch == ')' && top != '(') ||
                    (ch == '}' && top != '{') ||
                    (ch == ']' && top != '[')) {
                    string message = "Mismatched bracket '" + string(1, ch) + "' at line " + to_string(lineNumber) + ", expected closing for '" + string(1, top) + "'";
                    errorOutput(message, errorLogFile, string(1, ch));
                }
                bracketStack.pop();
            }
        }

        if (inNumber) {
            if (isdigit(ch) || (ch == '.' && !hasDot)) {
                if (ch == '.') hasDot = true;
                currentLine += ch;
                token += ch;
                currentExpression += ch;
                continue;
            } else {
                Token t = analyzeToken(token, errorLogFile);
                printToken(t, token);
                if (t.type == Constants::KeyWord && (token == "int" || token == "float" || token == "double" || token == "char" || token == "bool" || token == "string" || token == "wstring") && !inFunction) {
                    hasType = true;
                    currentDecl.type = token == "int" ? "Int" :
                                       token == "float" ? "Float" :
                                       token == "double" ? "Double" :
                                       token == "char" ? "Char" :
                                       token == "bool" ? "Bool" :
                                       token == "string" ? "String" :
                                       token == "wstring" ? "Wstring" : "Undefined";
                } else if (hasType && t.type == Constants::Identifier) {
                    currentDecl.name = token;
                    if (ch == '(') {
                        inFunction = true;
                        params.clear();
                    }
                    if (ch == ')') {
                        inFunction = false;
                        params.clear();
                    }
                } else if (t.type == Constants::Constant && !currentDecl.name.empty()) {
                    currentDecl.value = token;
                }
                if (inMethodCall) currentExpression += token;
                token.clear();
                inNumber = false;
                hasDot = false;
            }
        }

        if (isalnum(ch) || ch == '_' || (token.empty() && ch == '-')) {
            token += ch;
            currentLine += ch;
            if (!inMethodCall) currentExpression += ch;
            if (isdigit(ch) || (ch == '-' && i + 1 < code.length() && isdigit(code[i + 1]))) {
                inNumber = true;
            }
        } else if (ch == '.') {
            if (!token.empty()) {
                Token t = analyzeToken(token, errorLogFile);
                printToken(t, token);
                if (t.type == Constants::KeyWord && (token == "int" || token == "float" || token == "double" || token == "char" || token == "bool" || token == "string" || token == "wstring") && !inFunction) {
                    hasType = true;
                    currentDecl.type = token == "int" ? "Int" :
                                       token == "float" ? "Float" :
                                       token == "double" ? "Double" :
                                       token == "char" ? "Char" :
                                       token == "bool" ? "Bool" :
                                       token == "string" ? "String" :
                                       token == "wstring" ? "Wstring" : "Undefined";
                } else if (hasType && t.type == Constants::Identifier) {
                    currentDecl.name = token;
                } else if (t.type == Constants::Constant && !currentDecl.name.empty()) {
                    currentDecl.value = token;
                }
                if (inMethodCall) currentExpression += token;
                token.clear();
            }
            inMethodCall = true;
            currentExpression += ch;
            currentLine += ch;
        } else {
            string tokenVal = token;
            if (!token.empty()) {
                Token t = analyzeToken(token, errorLogFile);
                printToken(t, token);
                if (t.type == Constants::KeyWord && (token == "int" || token == "float" || token == "double" || token == "char" || token == "bool" || token == "string" || token == "wstring") && !inFunction) {
                    hasType = true;
                    currentDecl.type = token == "int" ? "Int" :
                                       token == "float" ? "Float" :
                                       token == "double" ? "Double" :
                                       token == "char" ? "Char" :
                                       token == "bool" ? "Bool" :
                                       token == "string" ? "String" :
                                       token == "wstring" ? "Wstring" : "Undefined";
                } else if (hasType && t.type == Constants::Identifier) {
                    currentDecl.name = token;
                    if (ch == '(') {
                        inFunction = true;
                        params.clear();
                    }
                    if (ch == ')') {
                        inFunction = false;
                        params.clear();
                    }
                } else if (t.type == Constants::Constant && !currentDecl.name.empty()) {
                    currentDecl.value = token;
                }
                if (inMethodCall) currentExpression += token;
                token.clear();
            }
            if (!isspace(ch)) {
                string delim(1, ch);
                Token t = analyzeToken(delim, errorLogFile);
                printToken(t, delim);
                currentLine += delim;
                if (delim == ")") {
                    inFunction = false;
                    if (inMethodCall) {
                        inMethodCall = false;
                        currentExpression.clear();
                    }
                }
                if (delim == "{") {
                    inFunction = false;
                }
                if (delim == ";") {
                    if (inMethodCall) {
                        vector<Declaration> decls = recognizeType(currentExpression + ";",
                                                                  errorLogFile,
                                                                  tokenVal);
                        for (const auto& decl : decls) {
                            LexemeAttributes attrs;
                            if (identifiers.getAttribute(decl.name, attrs)) {
                                attrs.type = stringToLexemeType(decl.type);
                                attrs.initialized = !decl.value.empty();
                                attrs.lexemeCode = Constants::Identifier;
                                attrs.lexemeTypeSize = VariableTableV2_0::lexemeSize(decl.type, decl.name);
                                identifiers.addAttribute(decl.name, attrs);
                            }
                        }
                        inMethodCall = false;
                        currentExpression.clear();
                    } else {
                        string trimmedLine = currentLine;
                        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t\n\r"));
                        trimmedLine.erase(trimmedLine.find_last_not_of(" \t\n\r") + 1);
                        bool skipLine = false;
                        for (const auto& keyword : keywordsToSkip) {
                            if (trimmedLine.find(keyword) == 0) {
                                skipLine = true;
                                break;
                            }
                        }
                        if (!skipLine && !currentLine.empty()) {
                            vector<Declaration> decls = recognizeType(currentLine,
                                                                      errorLogFile,
                                                                      tokenVal);
                            for (const auto& decl : decls) {
                                LexemeAttributes attrs;
                                // Добавляем идентификатор в таблицу только после определения типа
                                identifiers.addLexeme(decl.name, LexemeType::Undefined);
                                if (identifiers.getAttribute(decl.name, attrs)) {
                                    attrs.type = stringToLexemeType(decl.type);
                                    attrs.initialized = !decl.value.empty();
                                    attrs.lexemeCode = Constants::Identifier;
                                    attrs.lexemeTypeSize = VariableTableV2_0::lexemeSize(decl.type, decl.name);
                                    identifiers.addAttribute(decl.name, attrs);
                                }
                            }
                        }
                    }
                    currentLine.clear();
                    hasType = false;
                    currentDecl = Declaration();
                }
            } else {
                currentLine += ch;
                if (inMethodCall) currentExpression += ch;
            }
        }
    }

    if (!bracketStack.empty()) {
        char top = bracketStack.top();
        string message = "Unclosed bracket '" + string(1, top) + "' at the end of code";
        errorOutput(message, errorLogFile, string(1, top));
    }

    if (!token.empty()) {
        string tokenVal = token;
        Token t = analyzeToken(token, errorLogFile);
        printToken(t, token);
        currentLine += token;
        if (t.type == Constants::KeyWord && (token == "int" || token == "float" || token == "double" || token == "char" || token == "bool" || token == "string" || token == "wstring") && !inFunction) {
            hasType = true;
            currentDecl.type = token == "int" ? "Int" :
                               token == "float" ? "Float" :
                               token == "double" ? "Double" :
                               token == "char" ? "Char" :
                               token == "bool" ? "Bool" :
                               token == "string" ? "String" :
                               token == "wstring" ? "Wstring" : "Undefined";
        } else if (hasType && t.type == Constants::Identifier) {
            currentDecl.name = token;
        }
        if (!currentLine.empty() && !inMethodCall) {
            string trimmedLine = currentLine;
            trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t\n\r"));
            trimmedLine.erase(trimmedLine.find_last_not_of(" \t\n\r") + 1);
            bool skipLine = false;
            for (const auto& keyword : keywordsToSkip) {
                if (trimmedLine.find(keyword) == 0) {
                    skipLine = true;
                    break;
                }
            }
            if (!skipLine) {
                vector<Declaration> decls = recognizeType(currentLine + ";",
                                                          errorLogFile,
                                                          tokenVal);
                for (const auto& decl : decls) {
                    LexemeAttributes attrs;
                    identifiers.addLexeme(decl.name, LexemeType::Undefined);
                    if (identifiers.getAttribute(decl.name, attrs)) {
                        attrs.type = stringToLexemeType(decl.type);
                        attrs.initialized = !decl.value.empty();
                        attrs.lexemeCode = Constants::Identifier;
                        attrs.lexemeTypeSize = VariableTableV2_0::lexemeSize(decl.type, decl.name);
                        identifiers.addAttribute(decl.name, attrs);
                    }
                }
            }
        }
    }
    errorLogFile.close();
}

// Сохраняет токен в файл
void Scanner::saveToken(const Token& t)
{
    if (tokenFile.is_open()) {
        tokenFile << t.type << " " << t.tableIndex << " " << t.hashIndex << endl;
    }
}

// Выводит информацию о токене на экран
void Scanner::printToken(const Token& t, const string& value)
{
    cout << "Token: " << value << " Type: " << t.type;
    if (t.tableIndex >= 0) {
        cout << " Table Index: " << t.tableIndex;
    }
    if (t.hashIndex >= 0) {
        cout << " Hash Index: " << t.hashIndex;
    }
    cout << endl;
}

// Выводит содержимое таблиц
void Scanner::printTableContents(ofstream& logger)
{
    logger << "Keywords:" << endl;
    for (const auto& entry : keywords.getTable()) {
        logger << entry.symbol << " " << endl;
    }
    logger << "\nDelimiters:" << endl;
    for (const auto& entry : separators.getTable()) {
        logger << entry.symbol << " " << endl;
    }
    logger << "\nIdentifiers:" << endl;
    for (const string& name : identifiers.get_all_names()) {
        LexemeAttributes attrs;
        if (identifiers.getAttribute(name, attrs)) {
            logger << name << " (" << VariableTableV2_0::lexemeTypeToString(attrs.type);
            logger << ", initialized: " << (attrs.initialized ? "Yes" : "No") << ") " << endl;
        }
    }
    logger << "\nConstants:" << endl;
    for (const string& name : constants.get_all_names()) {
        LexemeAttributes attrs;
        if (constants.getAttribute(name, attrs)) {
            logger << name << " (" << VariableTableV2_0::lexemeTypeToString(attrs.type) << ") " << endl;
        }
    }
    logger << endl;
}
























//bool Scanner::isLetter(char c) {
//    return isalpha(static_cast<unsigned char>(c)) || c == '_';
//}
//
//bool Scanner::isDigit(char c) {
//    return isdigit(static_cast<unsigned char>(c));
//}
//
//bool Scanner::isOperator(char c) {
//    return (c == '+' || c == '-' || c == '*' || c == '/' ||
//            c == '=' || c == '^' || c == '<' || c == '>' || c == '!');
//}
//
//string Scanner::getTokenType(const string& token)
//{
//    if (keywords.contains(token)) return "Keyword";
//    if (separators.contains(token)) return "Separator";
//
//    // Проверка на числовую константу
//    bool hasDecimal = false;
//    bool isNumber = true;
//    for (char c : token) {
//        if (c == '.') {
//            if (hasDecimal) {
//                isNumber = false;
//                break;
//            }
//            hasDecimal = true;
//        } else if (!isDigit(c) && c != '-') {
//            isNumber = false;
//            break;
//        }
//    }
//    if (isNumber) return "Constant";
//
//    // Проверка на булево значение
//    if (token == "true" || token == "false") return "Constant";
//
//    // Если ничего не подошло - это идентификатор
//    return "Identifier";
//}
//
//void Scanner::processCode(const string& code) {
//    string token;
//    size_t i = 0;
//
//    while (i < code.length()) {
//        char c = code[i];
//
//        // Пропускаем пробелы
//        if (isspace(static_cast<unsigned char>(c))) {
//            if (!token.empty()) {
//                processToken(token, i, code);
//                token.clear();
//            }
//            i++;
//            continue;
//        }
//
//        // Обработка операторов и разделителей
//        if (isOperator(c) || separators.contains(string(1, c))) {
//            if (!token.empty()) {
//                processToken(token, i, code);
//                token.clear();
//            }
//            string op(1, c);
//            if (separators.contains(op)) {
//                int index = separators.getIndex(op);
//                tokenFile << Separator << " " << index << std::endl;
//            } else {
//                int lexemeCode = Constant;
//                constants.addLexeme(op, LexemeType::Char);
//                size_t hashIndex = constants.hash(op);
//                tokenFile << lexemeCode << " " << hashIndex << endl;
//            }
//            i++;
//            continue;
//        }
//
//        // Собираем токен
//        token += c;
//        i++;
//    }
//
//    // Обработка последнего токена
//    if (!token.empty()) {
//        processToken(token, i, code);
//    }
//}
//
//void Scanner::processToken(const string& token, size_t& pos, const string& code) {
//    string type = getTokenType(token);
//
//    if (type == "Keyword") {
//        int index = keywords.getIndex(token);
//        tokenFile << KeyWord << " " << index << endl;
//    }
//    else if (type == "Separator") {
//        int index = separators.getIndex(token);
//        tokenFile << Separator << " " << index << endl;
//    }
//    else if (type == "Constant") {
//        LexemeType lexType;
//        int lexemeCode = Constant;
//        if (token == "true" || token == "false") {
//            lexType = LexemeType::Bool;
//        } else if (token.find('.') != string::npos) {
//            lexType = LexemeType::Float;
//        } else {
//            lexType = LexemeType::Int;
//        }
//        int lexemeTypeSize = 0;
//        switch (lexType) {
//            case LexemeType::Bool: lexemeTypeSize = sizeof(bool); break;
//            case LexemeType::Float: lexemeTypeSize = sizeof(float); break;
//            case LexemeType::Int: lexemeTypeSize = sizeof(int); break;
//        }
//
//        constants.addLexeme(token, lexType);
//        constants.set_code(token, lexemeCode);
//        constants.set_size(token, lexemeTypeSize);
//        size_t hashIndex = constants.hash(token);
//        tokenFile << lexemeCode << " " << hashIndex << std::endl;
//    }
//    else if (type == "Identifier") {
//        LexemeType lexType = LexemeType::Undefined;
//        int lexemeCode = Identifier; // 30
//
//        // Простой контекстный анализ для определения типа
//        size_t prevPos = pos - token.length() - 1;
//        string prevToken;
//        while (prevPos > 0 && isspace(code[prevPos])) prevPos--;
//        while (prevPos > 0 && !isspace(code[prevPos]) &&
//               !separators.contains(string(1, code[prevPos]))) {
//            prevToken = code[prevPos] + prevToken;
//            prevPos--;
//        }
//
//        if (prevToken == "int") lexType = LexemeType::Int;
//        else if (prevToken == "float") lexType = LexemeType::Float;
//        else if (prevToken == "bool") lexType = LexemeType::Bool;
//
//        int lexemeTypeSize = 0;
//        switch (lexType) {
//            case LexemeType::Int: lexemeTypeSize = sizeof(int); break;
//            case LexemeType::Float: lexemeTypeSize = sizeof(float); break;
//            case LexemeType::Bool: lexemeTypeSize = sizeof(bool); break;
//            default: lexemeTypeSize = sizeof(int); break; // по умолчанию
//        }
//
//        identifiers.addLexeme(token, lexType);
//        identifiers.set_code(token, lexemeCode);
//        identifiers.set_size(token, lexemeTypeSize);
//        size_t hashIndex = identifiers.hash(token);
//        tokenFile << lexemeCode << " " << hashIndex << endl;
//    }
//}
//
//void Scanner::printTableContents(ofstream& logger) {
//    cout << "Keywords Table (lexemeCode = " << KeyWord << "):" << endl;
//    for (const auto& entry : keywords.getTable()) {
//        cout << "Symbol: " << entry.symbol
//                  << ", Type: " << ConstantTableV2_0::lexemeTypeToString(entry.type)
//                  << ", Code: " << entry.lexemeCode
//                  << ", Size: " << entry.lexemeTypeSize << endl;
//    }
//
//    cout << "\nSeparators Table (lexemeCode = " << Separator << "):" << endl;
//    for (const auto& entry : separators.getTable()) {
//        cout << "Symbol: " << entry.symbol
//                  << ", Type: " << ConstantTableV2_0::lexemeTypeToString(entry.type)
//                  << ", Code: " << entry.lexemeCode
//                  << ", Size: " << entry.lexemeTypeSize << endl;
//    }
//
//    cout << "\nIdentifiers Table (lexemeCode = " << Identifier << "):" << endl;
//    identifiers.printTable(logger);
//
//    cout << "\nConstants Table (lexemeCode = " << Constant << "):" << endl;
//    constants.printTable(logger);
//}
