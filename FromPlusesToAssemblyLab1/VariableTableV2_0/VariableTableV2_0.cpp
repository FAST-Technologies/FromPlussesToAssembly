/// <summary>
/// [EN] Linked libraries and headers
/// [RU] Подключаемые библиотеки и заголовки
/// </summary>
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
        lexemeTypeSize = static_cast<int>(sizeof(string) * symbol.size());
    } else if (typeStr == "Wstring") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Vector") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Array") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "List") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Unordered_map") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Forward_list") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Deque") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Queue") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Priority_queue") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Stack") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Map") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Set") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Unordered_set") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Multimap") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Multiset") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Bitset") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Pair") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Tuple") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Optional") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
    } else if (typeStr == "Variant") {
        lexemeTypeSize = static_cast<int>(sizeof(wstring) * symbol.size());
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
        if (typeStr == "Int" || typeStr == "Float" || typeStr == "Double") {
            lexemeCode = Constants::Constant;
        } else {
            lexemeCode = Constants::Identifier;
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
    if (static_cast<double>(size) / static_cast<int>(capacity) >= loadFactor) {
        rehash();
    }
    size_t index = hash(name);
    size_t originalIndex = index;
    while (table[index] != nullptr) {
        index = (index + 1) % capacity;
        if (index == originalIndex) return false;
    }
    LexemeType type = determineLexemeType(name);
    table[index] = make_unique<Entry>(Entry{name, {type, false, lexemeCode, lexemeTypeSize}}); // Initial attributes
    size++;
    return true;
}

/// <summary>
/// [EN] Function for removing lexeme from hash table by lexeme name
/// [RU] Функция для удаления лексемы из хэш-таблицы по её имени
/// </summary>
void VariableTableV2_0::remove_lexeme(const string& name)
{
    size_t index = hash(name);
    if (table[index]) {
        table[index].reset();
        table[index] = nullptr;
        size--;
    }
}

/// <summary>
/// [EN] Function for removing lexeme from hash table by all parameters except of lexeme name
/// [RU] Функция для удаления лексемы из хэш-таблицы по её параметрам за исключением имени лексемы
/// </summary>
void VariableTableV2_0::remove_lexeme_without_name(int lexemeCode, LexemeType lexemeType, bool initialized)
{
    for (int i = 0; i < capacity; ++i) {
        if (table[i]) {
            if (table[i]->attributes.lexemeCode == lexemeCode &&
                table[i]->attributes.type == lexemeType &&
                table[i]->attributes.initialized == initialized) {
                table[i].reset();
                table[i] = nullptr;
                size--;
            }
        }
    }
}

/// <summary>
/// [EN] Function for removing lexeme from hash table by lexeme type
/// [RU] Функция для удаления лексемы из хэш-таблицы по её типу
/// </summary>
void VariableTableV2_0::remove_lexeme_by_lexemeType(LexemeType lexemeType)
{
    for (int i = 0; i < capacity; ++i) {
        if (table[i]) {
            if (table[i]->attributes.type == lexemeType) {
                table[i].reset();
                table[i] = nullptr;
                size--;
            }
        }
    }
}

/// <summary>
/// [EN] Function for removing lexeme from hash table by her initialisation status
/// [RU] Функция для удаления лексемы из хэш-таблицы по её статусу инициализации
/// </summary>
void VariableTableV2_0::remove_lexeme_by_initialized_status(bool initialized)
{
    for (int i = 0; i < capacity; ++i) {
        if (table[i]) {
            if (table[i]->attributes.initialized == initialized) {
                table[i].reset();
                table[i] = nullptr;
                size--;
            }
        }
    }
}

/// <summary>
/// [EN] Function for removing lexeme from hash table by her lexeme code
/// [RU] Функция для удаления лексемы из хэш-таблицы по её коду
/// </summary>
void VariableTableV2_0::remove_lexeme_by_lexemeCode(int lexemeCode)
{
    for (int i = 0; i < capacity; ++i) {
        if (table[i]) {
            if (table[i]->attributes.lexemeCode == lexemeCode) {
                table[i].reset();
                table[i] = nullptr;
                size--;
            }
        }
    }
}

/// <summary>
/// [EN] Function for adding the explicit lexeme to hash table
/// [RU] Функция для добавления лексемы в хэш-таблицу
/// </summary>
bool VariableTableV2_0::addLexeme(const string& name, LexemeType type)
{
    int lexemeCode;
    int lexemeTypeSize;

    switch (type) {
        case LexemeType::Int:
            lexemeCode = Constants::Constant;
            lexemeTypeSize = sizeof(int);
            break;
        case LexemeType::Float:
            lexemeCode = Constants::Constant;
            lexemeTypeSize = sizeof(float);
            break;
        case LexemeType::String:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = static_cast<int>(sizeof(string) * name.size());
            break;
        case LexemeType::WString:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = static_cast<int>(sizeof(wstring) * name.size());
            break;
        case LexemeType::Bool:
            lexemeCode = Constants::Constant;
            lexemeTypeSize = sizeof(bool);
            break;
        case LexemeType::Double:
            lexemeCode = Constants::Constant;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Char:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Vector:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Array:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::List:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Unordered_map:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Forward_list:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Deque:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Queue:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Priority_queue:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Stack:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Map:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Set:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Unordered_set:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Multimap:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Multiset:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Bitset:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Pair:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Tuple:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Optional:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        case LexemeType::Variant:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(double);
            break;
        default:
            lexemeCode = Constants::Identifier;
            lexemeTypeSize = sizeof(bool);
            break;
    }
    string typeString = lexemeTypeToString(type);
    return addLexeme(name, typeString, lexemeCode, lexemeTypeSize);
}

/// <summary>
/// [EN] Function that able to get all names of variables
/// [RU] Функция, которая позволяет получать все имена переменных
/// </summary>
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
        case LexemeType::Undefined: return "Undefined";
        case LexemeType::Int: return "Int";
        case LexemeType::Float: return "Float";
        case LexemeType::String: return "String";
        case LexemeType::Double: return "Double";
        case LexemeType::Bool: return "Bool";
        case LexemeType::Char: return "Char";
        case LexemeType::WString: return "WString";
        case LexemeType::Vector: return "Vector";
        case LexemeType::Array: return "Array";
        case LexemeType::List: return "List";
        case LexemeType::Unordered_map: return "Unordered_map";
        case LexemeType::Forward_list: return "Forward_list";
        case LexemeType::Deque: return "Deque";
        case LexemeType::Queue: return "Queue";
        case LexemeType::Priority_queue: return "Priority_queue";
        case LexemeType::Stack: return "Stack";
        case LexemeType::Map: return "Map";
        case LexemeType::Set: return "Set";
        case LexemeType::Unordered_set: return "Unordered_set";
        case LexemeType::Multimap: return "Multimap";
        case LexemeType::Multiset: return "Multiset";
        case LexemeType::Bitset: return "Bitset";
        case LexemeType::Pair: return "Pair";
        case LexemeType::Tuple: return "Tuple";
        case LexemeType::Optional: return "Optional";
        case LexemeType::Variant: return "Variant";
        case LexemeType::Function: return "Function";
        default: return "Unknown";
    }
}

/// <summary>
/// [EN] Function for adding the attribute of lexeme to hash table
/// [RU] Функция для добавления атрибутов лексемы в хэш-таблицу
/// </summary>
bool VariableTableV2_0::addAttribute(const string& name, LexemeAttributes attributes)
{
    size_t index = hash(name);
    size_t originalIndex = index;
    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            table[index]->attributes = attributes;
            return true;
        }
        index = (index + 1) % capacity;

        if (index == originalIndex) break;
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

        if (index == originalIndex) break;
    }
    return false;
}

/// <summary>
/// [EN] Function for counting the hash sum
/// [RU] Функция для подсчёта хэш-суммы
/// </summary
size_t VariableTableV2_0::hash(const string& key) const
{
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

    for (size_t i = 0; i < oldCapacity; ++i) {
        if (table[i] != nullptr) {
            size_t newIndex = hash(table[i]->name) % capacity;
            size_t originalNewIndex = newIndex;

            while (newTable[newIndex] != nullptr) {
                newIndex = (newIndex + 1) % capacity;
                if (newIndex == originalNewIndex) break;
            }
            newTable[newIndex] = std::move(table[i]);
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
            return true;
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break;
    }
    return false;
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
            return true;
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break;
    }
    return false;
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
            return true;
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break;
    }
    return false;
}

/// <summary>
/// [EN] Function that sets different code for variable
/// [RU] Функция, устанавливающая другой код лексемы
/// </summary>
bool VariableTableV2_0::set_code(const string& name, int lexemeCode)
{
    size_t index = hash(name);
    size_t originalIndex = index;

    while (table[index] != nullptr) {
        if (table[index]->name == name) {
            table[index]->attributes.lexemeCode = lexemeCode;
            return true;
        }
        index = (index + 1) % capacity;
        if (index == originalIndex) break;
    }
    return false;
}


/// <summary>
/// [EN] Helper function to determine LexemeType
/// [RU] Вспомогательная функция для вычленения LexemeType
/// </summary
LexemeType VariableTableV2_0::determineLexemeType(const string& value)
{
    if (!isdigit(value[0]) && !(value[0] == '-' && value.size() > 1 && isdigit(value[1])) && value.length() != 1) {
        return LexemeType::Undefined; // Идентификаторы не должны определяться здесь
    }

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
    if (ss >> intValue && ss.eof()) {
        return LexemeType::Int;
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
                } else {
                    return LexemeType::Float;
                }
            }
        }
    }

//    ss.clear();
//    ss.str(value);
//    if (ss >> floatValue) {
//        if (ss.eof()) {
//            return LexemeType::Float;
//        }
//    }

    return LexemeType::Undefined;
}

/// <summary>
/// [EN] Function that prints the hash table
/// [RU] Функция для вывода данных хэш-таблицы
/// </summary>
void VariableTableV2_0::printTable(ofstream& logger) const {
    logger << "Variable Table Contents:" << endl;
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] != nullptr) {
            logger << "Index: " << i << endl;
            logger << "  Name: " << table[i]->name << endl;
            logger << "  Lexeme Code: " << table[i]->attributes.lexemeCode << endl;
            logger << "  Type: ";
            switch (table[i]->attributes.type) {
                case LexemeType::Int: logger << "Int"; break;
                case LexemeType::Float: logger << "Float"; break;
                case LexemeType::String: logger << "String"; break;
                case LexemeType::Bool: logger << "Bool"; break;
                case LexemeType::Double: logger << "Double"; break;
                case LexemeType::Char: logger << "Char"; break;
                case LexemeType::WString: logger << "WString"; break;
                case LexemeType::Vector: logger << "Vector"; break;
                case LexemeType::Array: logger << "Array"; break;
                case LexemeType::List: logger << "List"; break;
                case LexemeType::Unordered_map: logger << "Unordered_map"; break;
                case LexemeType::Forward_list: logger << "Forward_list"; break;
                case LexemeType::Deque: logger << "Deque"; break;
                case LexemeType::Queue: logger << "Queue"; break;
                case LexemeType::Priority_queue: logger << "Priority_queue"; break;
                case LexemeType::Stack: logger << "Stack"; break;
                case LexemeType::Map: logger << "Map"; break;
                case LexemeType::Set: logger << "Set"; break;
                case LexemeType::Unordered_set: logger << "Unordered_set"; break;
                case LexemeType::Multimap: logger << "Multimap"; break;
                case LexemeType::Multiset: logger << "Multiset"; break;
                case LexemeType::Bitset: logger << "Bitset"; break;
                case LexemeType::Pair: logger << "Pair"; break;
                case LexemeType::Tuple: logger << "Tuple"; break;
                case LexemeType::Optional: logger << "Optional"; break;
                case LexemeType::Variant: logger << "Variant"; break;
                case LexemeType::Undefined: logger << "Undefined"; break;
                default: logger << "Unknown"; break;
            }
            logger << endl;
            logger << "Lexeme type size: " << table[i]->attributes.lexemeTypeSize << endl;
            logger << "  Initialized: " << (table[i]->attributes.initialized ? "Yes" : "No") << endl;
            logger << "----------------------" << endl;
        }
    }
}


