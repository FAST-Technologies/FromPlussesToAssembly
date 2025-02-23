#include "VariableTableV2_0.h"

#include <iostream>

VariableTableV2_0::VariableTableV2_0(int initialCapacity) : capacity(initialCapacity), size(0), table(initialCapacity) {}


int VariableTableV2_0::lexemeSize(const string& typeStr, const string& symbol)
{
    int lexemeTypeSize = 0;
    if (typeStr == "Int") {
        lexemeTypeSize = sizeof(int);
    } else if (typeStr == "Float") {
        lexemeTypeSize = sizeof(float);
    } else if (typeStr == "Double") {
        lexemeTypeSize = sizeof(double);
    } else if (typeStr == "Char") {
        lexemeTypeSize = sizeof(char);
    } else if (typeStr == "String") {
        lexemeTypeSize = sizeof(string) * static_cast<int>(symbol.size());
    }
    return lexemeTypeSize;
}

bool VariableTableV2_0::loadFromFile(const string& filename) {
    auto file = ifstream(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    string symbol;
    string typeStr;
    int lexemeCode;
    while (file >> symbol >> typeStr >> lexemeCode) {
        // Decide whether to add as an identifier (30) or constant (40)

        if (typeStr == "Int" || typeStr == "Float" || typeStr == "Double") {
            lexemeCode = 40; // Assume numeric literals are constants
        } else {
            lexemeCode = 30; // Treat other as identifiers
        }
        int lexemeTypeSize = lexemeSize(typeStr, symbol);
        addLexeme(symbol, typeStr, lexemeCode, lexemeTypeSize);
    }
    file.close();
    return true;
}


bool VariableTableV2_0::addLexeme(const string& name, const string& typeStr, int& lexemeCode, int& lexemeTypeSize) {
    if (containsLexeme(name)) return false;
    if (size >= capacity * loadFactor) {
        rehash();
    }

    size_t index = hash(name);
    size_t originalIndex = index;
    while (table[index] != nullptr) {
        index = (index + 1) % capacity;  // Linear probing for collision resolution
        if (index == originalIndex) return false; // Table is full
    }

    LexemeType type = determineLexemeType(name); // Automatically figure out the type

    table[index] = make_unique<Entry>(Entry{name, {type, false, lexemeCode, lexemeTypeSize}}); // Initial attributes
    size++;
    return true;
}


bool VariableTableV2_0::addLexeme(const string& name, LexemeType type) {
    int lexemeCode;
    int lexemeTypeSize; // Default size

    switch (type) {
        case LexemeType::Int:
            lexemeCode = 40; // Constant
            lexemeTypeSize = sizeof(int); // Size of int
            break;
        case LexemeType::Float:
            lexemeCode = 40; // Constant
            lexemeTypeSize = sizeof(float); // Size of float
            break;
        case LexemeType::String:
            lexemeCode = 30; // Identifier
            lexemeTypeSize = sizeof(string) * static_cast<int>(name.size());
            break;
        case LexemeType::Bool:
            lexemeCode = 40; // Identifier
            lexemeTypeSize = sizeof(bool);
            break;
        case LexemeType::Double:
            lexemeCode = 40; // Identifier
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Char:
            lexemeCode = 30; // Identifier
            lexemeTypeSize = sizeof(double);
            break;
        default:
            lexemeCode = 30; // Default to Identifier for Undefined and other types
            lexemeTypeSize = sizeof(bool);
            break;
    }
    string typeString = lexemeTypeToString(type);
    return addLexeme(name, typeString, lexemeCode, lexemeTypeSize);
}

string VariableTableV2_0::lexemeTypeToString(LexemeType type) {
    switch (type) {
        case LexemeType::Undefined:
            return "Undefined";
        case LexemeType::Int:
            return "Int";
        case LexemeType::Float:
            return "Float";
        case LexemeType::String:
            return "String";
        case LexemeType::Double:
            return "Double";
        case LexemeType::Bool:
            return "Bool";
        case LexemeType::Char:
            return "Char";
        default:
            return "Unknown"; // Handle unexpected values
    }
}

bool VariableTableV2_0::addAttribute(const string& name, LexemeAttributes attributes) {
    size_t index = hash(name);
    size_t originalIndex = index; // Track starting point
    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            table[index]->attributes = attributes;
            return true;
        }
        index = (index + 1) % capacity;

        if (index == originalIndex) break; // Table is full
    }
    return false;
}

bool VariableTableV2_0::containsLexeme(const string& name) const {
    size_t index = hash(name);
    size_t originalIndex = index;
    while (table[index] != nullptr) {
        if (table[index]->name == name) return true;
        index = (index + 1) % capacity;

        if (index == originalIndex) break; // Full loop
    }
    return false;
}

bool VariableTableV2_0::getAttribute(const string& name, LexemeAttributes& attributes) const {
    size_t index = hash(name);
    size_t originalIndex = index;
    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            attributes = table[index]->attributes;
            return true;
        }
        index = (index + 1) % capacity;

        if (index == originalIndex) break; // Full loop
    }
    return false;
}

size_t VariableTableV2_0::hash(const string& key) const {
    size_t hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * 31) + c;
    }
    return hashValue % capacity;
}

void VariableTableV2_0::rehash() {
    size_t newCapacity = capacity * 2;  // Double the capacity
    vector<unique_ptr<Entry>> newTable(newCapacity);

    // Rehash all existing elements to the new table
    for (auto& entry : table) {
        if (entry != nullptr) {
            size_t newIndex = hash(entry->name) % newCapacity;
            size_t originalIndex = newIndex;

            while (newTable[newIndex] != nullptr) {
                newIndex = (newIndex + 1) % newCapacity;
                if (newIndex == originalIndex) break;
            }

            if (newTable[newIndex] == nullptr) {
                newTable[newIndex] = std::move(entry); // Move the entry
            }
        }
    }

    // Replace the old table with the new table
    table = std::move(newTable);
    capacity = newCapacity;
}

LexemeType VariableTableV2_0::determineLexemeType(const string& value) const {
    stringstream ss(value);
    int intValue;
    float floatValue;
    double doubleValue;
    char charValue;
    //bool boolValue;

//    // Check for bool first (as stream extraction can be tricky)
//    if (value == "true" || value == "false") {
//        return LexemeType::Bool;
//    }

    // Check for int
    ss.clear();
    ss.str(value);
    if (ss >> intValue) {
        if (ss.eof()) { // Ensure the entire string was consumed as an integer
            return LexemeType::Int;
        }
    }

    // Check for char next (single character)
    if (value.length() == 1) {
        ss.clear();
        ss.str(value);
        if (ss >> charValue) {
            return LexemeType::Char;
        }
    }

    // Check for double
    ss.clear();
    ss.str(value);
    if (ss >> doubleValue) {
        if (ss.eof()) {
            size_t decimalPos = value.find('.');
            if (decimalPos != string::npos) {
                size_t digitsAfterDecimal = value.length() - decimalPos - 1;
                if (digitsAfterDecimal > 7) {
                    return LexemeType::Double;
                }
            }
        }
    }

    // Check for float (after double, since a float can also be parsed as a double)
    ss.clear();
    ss.str(value);
    if (ss >> floatValue) {
        if (ss.eof()) {
            return LexemeType::Float;
        }
    }

    return LexemeType::String;
//    if (ss >> intValue) {
//        if (ss.eof()) { // Ensure the entire string was consumed as an integer
//            return LexemeType::Int;
//        } else {
//            ss.clear(); // Clear error flags
//            ss.str(value); // Reset stringstream
//            if (ss >> floatValue && ss.eof()) {
//                return LexemeType::Float;
//            } else {
//                return LexemeType::String; // Treat as a string
//            }
//        }
//    } else {
//        return LexemeType::String; // Treat as a string
//    }
}

void VariableTableV2_0::printTable() const {
    cout << "Variable Table Contents:" << endl;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) {
            cout << "Index: " << i << endl;
            cout << "  Name: " << table[i]->name << endl;
            cout << "  Lexeme Code: " << table[i]->attributes.lexemeCode << endl;
            cout << "  Type: ";
            switch (table[i]->attributes.type) {
                case LexemeType::Int:
                    cout << "Int";
                    break;
                case LexemeType::Float:
                    cout << "Float";
                    break;
                case LexemeType::String:
                    cout << "String";
                    break;
                case LexemeType::Bool:
                    cout << "Bool";
                    break;
                case LexemeType::Double:
                    cout << "Double";
                    break;
                case LexemeType::Char:
                    cout << "Char";
                    break;
                case LexemeType::Undefined:
                    cout << "Undefined";
                    break;
                default:
                    cout << "Unknown";
                    break;
            }
            cout << endl;
            cout << "Lexeme type size: " << table[i]->attributes.lexemeTypeSize << endl;
            cout << "  Initialized: " << (table[i]->attributes.initialized ? "Yes" : "No") << endl;
            cout << "----------------------" << endl;
        }
    }
}
