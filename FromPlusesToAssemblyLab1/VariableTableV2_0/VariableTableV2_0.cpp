#include "VariableTableV2_0.h"

/// <summary>
/// [EN] Constructor for VariableTableV2_0 class
/// [RU] Конструктор класса VariableTableV2_0
/// </summary>
VariableTableV2_0::VariableTableV2_0(int initialCapacity) :
    capacity(initialCapacity),
    size(0),
    table(initialCapacity) {}

/// <summary>
/// [EN] Destructor for VariableTableV2_0 class
/// [RU] Деструктор класса VariableTableV2_0
/// </summary>
VariableTableV2_0::~VariableTableV2_0()
{
    //table.clear();
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) {
            table[i].reset();
        }
    }
    cout << "Object VariableTable is deleted!" << endl;
}

/// <summary>
/// [EN] Function that return the lexeme size
/// [RU] Функция, возвращающая размер лексемы
/// </summary>
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

/// <summary>
/// [EN] Function for loading table data for hash table
/// [RU] Функция для загрузки данных хэш таблицы
/// </summary>
bool VariableTableV2_0::loadFromFile(const string& filename)
{
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

bool VariableTableV2_0::addLexeme(const string& name, int& lexemeCode, int& lexemeTypeSize)
{
    return addLexeme(name, "Undefined", lexemeCode, lexemeTypeSize);
}

/// <summary>
/// [EN] Function for adding the explicit lexeme to hash table
/// [RU] Функция для добавления лексемы в хэш-таблицу
/// </summary>
bool VariableTableV2_0::addLexeme(const string& name, const string& typeStr, int& lexemeCode, int& lexemeTypeSize)
{
    if (containsLexeme(name)) return false;
    if (static_cast<double>(size) / capacity >= loadFactor) {
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

/// <summary>
/// [EN] Function for adding the explicit lexeme to hash table
/// [RU] Функция для добавления лексемы в хэш-таблицу
/// </summary>
bool VariableTableV2_0::addLexeme(const string& name, LexemeType type)
{
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


vector<string> VariableTableV2_0::get_all_names() const
{
    vector<string> names;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) {
            names.push_back(table[i]->name);
        }
    }
    return names;
}

/// <summary>
/// [EN] Function that transform the lexeme type to string
/// [RU] Функция для приведения типа лексемы к строчному виду
/// </summary>
string VariableTableV2_0::lexemeTypeToString(LexemeType type)
{
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
            return "Unknown";
    }
}

/// <summary>
/// [EN] Function for adding the attribute of lexeme to hash table
/// [RU] Функция для добавления атрибутов лексемы в хэш-таблицу
/// </summary>
bool VariableTableV2_0::addAttribute(const string& name, LexemeAttributes attributes)
{
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

/// <summary>
/// [EN] Function that checks the containing of lexeme in hash table
/// [RU] Функция для проверки содержания лексемы в хэш-таблице
/// </summary>
bool VariableTableV2_0::containsLexeme(const string& name) const
{
    size_t index = hash(name);
    size_t originalIndex = index;
    while (table[index] != nullptr) {
        if (table[index]->name == name) return true;
        index = (index + 1) % capacity;

        if (index == originalIndex) break;
    }
    return false;
}

/// <summary>
/// [EN] Function that gets the attribute of lexeme in hash table
/// [RU] Функция для получения атрибута лексемы в хэш-таблице
/// </summary>
bool VariableTableV2_0::getAttribute(const string& name, LexemeAttributes& attributes) const
{
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

/// <summary>
/// [EN] Function for counting the hash sum
/// [RU] Функция для подсчёта хэш-суммы
/// </summary
size_t VariableTableV2_0::hash(const string& key) const {
    size_t hashValue = 0;
    for (char c : key) {
        hashValue = (hashValue * 31) + c;
    }
    return hashValue % capacity;
}

/// <summary>
/// [EN] Function for rehashing
/// [RU] Функция рехеширования
/// </summary
void VariableTableV2_0::rehash() {
    size_t newCapacity = capacity * 2;
    vector<unique_ptr<Entry>> newTable(newCapacity);
    size_t oldCapacity = capacity;
    capacity = newCapacity;

    // Rehash all existing elements to the new table
    for (size_t i = 0; i < oldCapacity; ++i) {
        if (table[i] != nullptr) {
            size_t newIndex = hash(table[i]->name) % capacity; //Recalculate hash based on new capacity
            size_t originalNewIndex = newIndex;

            while (newTable[newIndex] != nullptr) {
                newIndex = (newIndex + 1) % capacity;  // Linear probing
                if (newIndex == originalNewIndex) break; //Table full!  This should never happen.
            }
            newTable[newIndex] = std::move(table[i]);  // Move to the new table
        }
    }

    // Replace the old table with the new table
    table = std::move(newTable);
}

/// <summary>
/// [EN] Function that sets different type for variable
/// [RU] Функция, устанавливающая другой тип лексемы
/// </summary>
bool VariableTableV2_0::set_type(const string& name, LexemeType newType)
{
    size_t index = hash(name);
    size_t originalIndex = index;

    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            table[index]->attributes.type = newType;
            return true; // Successfully updated the type
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break; // Element not found
    }
    return false; // Element not found
}

/// <summary>
/// [EN] Function that sets different init status for variable
/// [RU] Функция, устанавливающая другой статус инициализации лексемы
/// </summary>
bool VariableTableV2_0::set_init(const string& name, bool isInitialized)
{
    size_t index = hash(name);
    size_t originalIndex = index;

    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            table[index]->attributes.initialized = isInitialized;
            return true; // Successfully updated the initialization status
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break; // Element not found
    }
    return false; // Element not found
}

/// <summary>
/// [EN] Function that sets different size for variable
/// [RU] Функция, устанавливающая другой размер лексемы
/// </summary>
bool VariableTableV2_0::set_size(const string& name, int lexemeTypeSize)
{
    size_t index = hash(name);
    size_t originalIndex = index;

    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            table[index]->attributes.lexemeTypeSize = lexemeTypeSize;
            return true; // Successfully updated the initialization status
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break; // Element not found
    }
    return false; // Element not found
}


/// <summary>
/// [EN] Helper function to determine LexemeType
/// [RU] Вспомогательная функция для вычленения LexemeType
/// </summary
LexemeType VariableTableV2_0::determineLexemeType(const string& value)
{
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
}

/// <summary>
/// [EN] Function that prints the hash table
/// [RU] Функция для вывода данных хэш-таблицы
/// </summary>
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


