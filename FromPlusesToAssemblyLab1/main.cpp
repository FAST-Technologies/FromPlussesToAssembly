#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>  // Для stringstream
#include <map>      // Для хранения переменных по типам

using namespace std;

bool isStringType(const string& type) {
    regex string_pattern(R"((?:std::)?(?:w)?string)");
    return regex_match(type, string_pattern);
}

// Функция для распознавания типов переменных
string recognizeType(const string& line, string& variableName, string& variableValue) {
    smatch match;
    regex int_pattern(R"((?:const\s+)?int(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*([^;]*);)");
    regex float_pattern(R"((?:const\s+)?float(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*([^;]*);)");
    regex double_pattern(R"((?:const\s+)?double(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*([^;]*);)");
    regex variable_pattern(R"((int|float|double|(?:std::)?(?:w)?string|char|bool)\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.+);)");
    regex char_pattern(R"((?:const\s+)?char(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*'(.*)';)");
    regex bool_pattern(R"((?:const\s+)?bool(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(true|false);)");
    regex string_pattern(R"((?:const\s+)?(?:std::)?(?:w)?string(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.+);)");

    regex vector_pattern(R"((?:const\s+)?(?:std::)?vector<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex array_pattern(R"((?:const\s+)?(?:std::)?array<([^,]+),\s*(\d+)\s*>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");

    regex list_pattern(R"((?:const\s+)?(?:std::)?list<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");

    regex unordered_map_pattern(R"((?:const\s+)?(?:std::)?unordered_map<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex forward_list_pattern(R"((?:const\s+)?(?:std::)?forward_list<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex deque_pattern(R"((?:const\s+)?(?:std::)?deque<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex queue_pattern(R"((?:const\s+)?(?:std::)?queue<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*);)");
    regex priority_queue_pattern(R"((?:const\s+)?(?:std::)?priority_queue<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*);)");
    regex stack_pattern(R"((?:const\s+)?(?:std::)?stack<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*);)");
    regex map_pattern(R"((?:const\s+)?(?:std::)?map<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex set_pattern(R"((?:const\s+)?(?:std::)?set<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex unordered_set_pattern(R"((?:const\s+)?(?:std::)?unordered_set<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex multimap_pattern(R"((?:const\s+)?(?:std::)?multimap<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex multiset_pattern(R"((?:const\s+)?(?:std::)?multiset<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*\{(.*)\};)");
    regex bitset_pattern(R"((?:const\s+)?(?:std::)?bitset<(\d+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)(?:\s*\((.*?)\))?;)");
    regex pair_pattern(R"((?:const\s+)?(?:std::)?pair<([^,]+),\s*([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\((.*)\);)");
    regex tuple_pattern(R"((?:const\s+)?(?:std::)?tuple<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\((.*)\);)");
    regex optional_pattern(R"((?:const\s+)?(?:std::)?optional<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.*);)");
    regex variant_pattern(R"((?:const\s+)?(?:std::)?variant<([^>]+)>(?:&\s*)?\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*(.*);)");


    if (regex_search(line, match, int_pattern)) {
        variableName = match[1].str();
        variableValue = match[2].str();
        return "int";
    }
    if (regex_search(line, match, float_pattern)) {
        variableName = match[1].str();
        variableValue = match[2].str();
        return "float";
    }
    if (regex_search(line, match, double_pattern)) {
        variableName = match[1].str();
        variableValue = match[2].str();
        return "double";
    }

//    if (regex_search(line, match, variable_pattern)) {
//        string type = match[1].str();
//        variableName = match[2].str();
//        variableValue = match[3].str();
//
//        if (isStringType(type)) {
//            // Удалить кавычки, если это строка
//            if (variableValue.length() >= 2 && variableValue[0] == '"' && variableValue.back() == '"') {
//                variableValue = variableValue.substr(1, variableValue.length() - 2);
//            }
//            return "string";
//        }
//    }

    if (regex_search(line, match, string_pattern)) {
        variableName = match[2].str(); // Изменено
        variableValue = match[3].str(); // Изменено

        // Удалить кавычки, если это строка
        if (variableValue.length() >= 2 && variableValue[0] == '"' && variableValue.back() == '"') {
            variableValue = variableValue.substr(1, variableValue.length() - 2);
            return "string";
        }

        if (variableValue.length() >= 3 && variableValue[0] == 'L' && variableValue[1] == '"' && variableValue.back() == '"') {
            variableValue = variableValue.substr(2, variableValue.length() - 3);
            return "wstring";
        }
        return "string";
    }

    if (regex_search(line, match, char_pattern)) {
        variableName = match[1].str();
        variableValue = match[2].str();
        return "char";
    }
    if (regex_search(line, match, bool_pattern)) {
        variableName = match[1].str();
        variableValue = match[2].str();
        return "bool";
    }
    if (regex_search(line, match, vector_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "vector";
    }
    if (regex_search(line, match, array_pattern)) {
        variableName = match[3].str();
        variableValue = match[4].str();
        return "array";
    }

    if (regex_search(line, match, forward_list_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "forward_list";
    }

    if (regex_search(line, match, list_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "list";
    }
    if (regex_search(line, match, unordered_map_pattern)) {
        variableName = match[3].str();
        variableValue = match[4].str();
        return "unordered_map";
    }


    if (regex_search(line, match, deque_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "deque";
    }
    if (regex_search(line, match, queue_pattern)) {
        variableName = match[1].str();
        variableValue = ""; // Значение не захватывается
        return "queue";
    }
    if (regex_search(line, match, priority_queue_pattern)) {
        variableName = match[1].str();
        variableValue = ""; // Значение не захватывается
        return "priority_queue";
    }
    if (regex_search(line, match, stack_pattern)) {
        variableName = match[1].str();
        variableValue = ""; // Значение не захватывается
        return "stack";
    }
    if (regex_search(line, match, unordered_set_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "unordered_set";
    }
    if (regex_search(line, match, multimap_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "multimap";
    }
    if (regex_search(line, match, multiset_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "multiset";
    }

    if (regex_search(line, match, map_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "map";
    }
    if (regex_search(line, match, set_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "set";
    }

    if (regex_search(line, match, bitset_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "bitset";
    }
    if (regex_search(line, match, pair_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "pair";
    }
    if (regex_search(line, match, tuple_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "tuple";
    }
    if (regex_search(line, match, optional_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "optional";
    }
    if (regex_search(line, match, variant_pattern)) {
        variableName = match[2].str();
        variableValue = match[3].str();
        return "variant";
    }

    return "unknown";
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
    map<string, vector<pair<string, string>>> variablesByType;

    while (getline(inputFile, line)) {
        string type;
        string variableName;
        string variableValue;
        type = recognizeType(line, variableName, variableValue);
        if (type != "unknown") {
            cout << "Variable declaration: " << line << " - Type: " << type << endl;
            variablesByType[type].emplace_back(variableName, variableValue);
        }
    }

    for (const auto& pair: variablesByType) {
        cout << pair.first << " : ";
        for (size_t i = 0; i < pair.second.size() ; i++) {
            cout << pair.second[i].first << ": " << pair.second[i].second;
            if (i < pair.second.size() - 1) {
                cout << "; ";
            }
        }
        cout << endl;
    }

    inputFile.close();

    return 0;
}
