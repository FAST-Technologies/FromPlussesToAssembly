#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>  // Для stringstream
#include <map>      // Для хранения переменных по типам

using namespace std;

struct Declaration {
    string type;
    string name;
    string value;
    string templateType;
};

void parseParameter(const string& param, string& paramType, string& paramName, string& paramTemplateType) {
    // Patterns for parameter types
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
        paramType = "int";
        paramName = match[1].str();
    } else if (regex_match(param, match, float_param_pattern)) {
        paramType = "float";
        paramName = match[1].str();
    } else if (regex_match(param, match, double_param_pattern)) {
        paramType = "double";
        paramName = match[1].str();
    } else if (regex_match(param, match, char_param_pattern)) {
        paramType = "char";
        paramName = match[1].str();
    } else if (regex_match(param, match, bool_param_pattern)) {
        paramType = "bool";
        paramName = match[1].str();
    } else if (regex_match(param, match, string_param_pattern)) {
        paramType = (param.find("wstring") != string::npos) ? "wstring" : "string";
        paramName = match[1].str();
    } else if (regex_match(param, match, vector_param_pattern)) {
        paramType = "vector";
        paramTemplateType = match[1].str();
        paramName = match[2].str();
    } else if (regex_match(param, match, map_param_pattern)) {
        paramType = "map";
        paramTemplateType = match[1].str();
        paramName = match[3].str();
    } else {
        paramType = "unknown";
        paramName = param;
    }
}

vector<string> splitParameters(const string& paramList) {
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

bool isStringType(const string& type) {
    regex string_pattern(R"((?:std::)?(?:w)?string)");
    return regex_match(type, string_pattern);
}

// Функция для распознавания типов переменных
vector<Declaration> recognizeType(const string& line) {
    vector<Declaration> declarations;
    smatch match;
    regex int_pattern(R"((?:const\s+)?int(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([^;]*))?;)");
    regex float_pattern(R"((?:const\s+)?float(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([^;]*))?;)");
    regex double_pattern(R"((?:const\s+)?double(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*([^;]*))?;)");
    regex char_pattern(R"((?:const\s+)?char(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*'(.*)')?;)");
    regex bool_pattern(R"((?:const\s+)?bool(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(true|false))?;)");
    regex string_pattern(R"((?:const\s+)?(?:std::)?(?:w)?string(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*(.+))?;)");

    regex vector_pattern(R"((?:const\s+)?(?:std::)?vector<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex array_pattern(R"((?:const\s+)?(?:std::)?array<([^,]+),\s*(\d+)\s*>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
    regex list_pattern(R"((?:const\s+)?(?:std::)?list<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*=\s*\{(.*)\})?;)");
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

    // Check for function declarations first
    if (regex_search(line, match, int_function_pattern)) {
        Declaration funcDecl;
        funcDecl.type = "int";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        // Parse parameters
        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "float";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "double";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "char";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "bool";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "string";
        funcDecl.name = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[2].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "vector";
        funcDecl.name = match[2].str();
        funcDecl.templateType = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[3].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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
        funcDecl.type = "map";
        funcDecl.name = match[3].str();
        funcDecl.templateType = match[1].str();
        funcDecl.value = "";
        declarations.push_back(funcDecl);

        string paramList = match[4].str();
        vector<string> params = splitParameters(paramList);
        for (const auto& param : params) {
            string paramType, paramName, paramTemplateType;
            parseParameter(param, paramType, paramName, paramTemplateType);
            if (paramType != "unknown") {
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

    // Check for variable declarations
    if (regex_search(line, match, int_pattern)) {
        Declaration decl;
        decl.type = "int";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, float_pattern)) {
        Declaration decl;
        decl.type = "float";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, double_pattern)) {
        Declaration decl;
        decl.type = "double";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, char_pattern)) {
        Declaration decl;
        decl.type = "char";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, bool_pattern)) {
        Declaration decl;
        decl.type = "bool";
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, string_pattern)) {
        Declaration decl;
        decl.name = match[1].str();
        decl.value = match[2].matched ? match[2].str() : "";
        if (decl.value.length() >= 2 && decl.value[0] == '"' && decl.value.back() == '"') {
            decl.value = decl.value.substr(1, decl.value.length() - 2);
            decl.type = "string";
        } else if (decl.value.length() >= 3 && decl.value[0] == 'L' && decl.value[1] == '"' && decl.value.back() == '"') {
            decl.value = decl.value.substr(2, decl.value.length() - 3);
            decl.type = "wstring";
        } else {
            decl.type = "string";
        }
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, vector_pattern)) {
        Declaration decl;
        decl.type = "vector";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, array_pattern)) {
        Declaration decl;
        decl.type = "array";
        decl.templateType = match[1].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, list_pattern)) {
        Declaration decl;
        decl.type = "list";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, unordered_map_pattern)) {
        Declaration decl;
        decl.type = "unordered_map";
        decl.templateType = match[1].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, forward_list_pattern)) {
        Declaration decl;
        decl.type = "forward_list";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, deque_pattern)) {
        Declaration decl;
        decl.type = "deque";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, queue_pattern)) {
        Declaration decl;
        decl.type = "queue";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, priority_queue_pattern)) {
        Declaration decl;
        decl.type = "priority_queue";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, stack_pattern)) {
        Declaration decl;
        decl.type = "stack";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, map_pattern)) {
        Declaration decl;
        decl.type = "map";
        decl.templateType = match[1].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, set_pattern)) {
        Declaration decl;
        decl.type = "set";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, unordered_set_pattern)) {
        Declaration decl;
        decl.type = "unordered_set";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, multimap_pattern)) {
        Declaration decl;
        decl.type = "multimap";
        decl.templateType = match[2].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, multiset_pattern)) {
        Declaration decl;
        decl.type = "multiset";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, bitset_pattern)) {
        Declaration decl;
        decl.type = "bitset";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, pair_pattern)) {
        Declaration decl;
        decl.type = "pair";
        decl.templateType = match[2].str();
        decl.name = match[3].str();
        decl.value = match[4].matched ? match[4].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, tuple_pattern)) {
        Declaration decl;
        decl.type = "tuple";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, optional_pattern)) {
        Declaration decl;
        decl.type = "optional";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }
    if (regex_search(line, match, variant_pattern)) {
        Declaration decl;
        decl.type = "variant";
        decl.templateType = match[1].str();
        decl.name = match[2].str();
        decl.value = match[3].matched ? match[3].str() : "";
        declarations.push_back(decl);
        return declarations;
    }

    return declarations;
}

string preprocessFile(ifstream& inputFile) {
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string content = buffer.str();

    // Remove single-line comments
    content = regex_replace(content, regex(R"(//.*?\n)"), "\n");
    // Remove multi-line comments
    content = regex_replace(content, regex(R"(/\*.*?\*/)", regex::extended), "");
    // Normalize whitespace
    content = regex_replace(content, regex(R"(\s+)"), " ");
    // Replace semicolons and braces with semicolons followed by newlines to ensure one declaration per line
    content = regex_replace(content, regex(R"(;\s*)"), ";\n");
    content = regex_replace(content, regex(R"(\{\s*)"), "{\n");
    content = regex_replace(content, regex(R"(\}\s*)"), "}\n");
    return content;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: program_name <cpp_file.cpp>" << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    cout << "Analyzing file: " << filename << endl;


    string line;
    map<string, vector<tuple<string, string, string>>> variablesByType;

    while (getline(inputFile, line)) {
        vector<Declaration> decls = recognizeType(line);
        for (const auto& decl : decls) {
            cout << "Declaration: " << line << " - Type: " << decl.type << ", Name: " << decl.name << endl;
            variablesByType[decl.type].emplace_back(decl.name, decl.value, decl.templateType);
        }
    }

    for (const auto& pair : variablesByType) {
        cout << pair.first << " : ";
        for (size_t i = 0; i < pair.second.size(); i++) {
            const auto& [name, value, tmplType] = pair.second[i];
            if (pair.first == "vector" || pair.first == "list" || pair.first == "forward_list" ||
                pair.first == "deque" || pair.first == "queue" || pair.first == "priority_queue" ||
                pair.first == "stack" || pair.first == "set" || pair.first == "unordered_set" ||
                pair.first == "multiset" || pair.first == "optional" || pair.first == "variant") {
                cout << tmplType << ": " << name;
            } else if (pair.first == "map" || pair.first == "unordered_map" || pair.first == "multimap" || pair.first == "pair") {
                cout << tmplType << ": " << name;
            } else {
                cout << name;
            }
            if (!value.empty()) {
                cout << ": " << value;
            }
            if (i < pair.second.size() - 1) {
                cout << "; ";
            }
        }
        cout << endl;
    }

    inputFile.close();

    return 0;
}
