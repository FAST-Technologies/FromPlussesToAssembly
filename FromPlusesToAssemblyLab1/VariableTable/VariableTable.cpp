#include <sstream>
#include "VariableTable.h"

// Конструктор с размером таблицы по умолчанию
//VariableTable::VariableTable()
//{
//    hashnum=default_hashnum;
//    table = new vector<lexeme> [hashnum];
//}
VariableTable::VariableTable() : VariableTable(default_hashnum) {}

// Конструктор с пользовательским размером таблицы
//VariableTable::VariableTable(int new_hashnum)
//{
//    hashnum=new_hashnum;
//    table = new vector<lexeme> [hashnum];
//}

VariableTable::VariableTable(int new_hashnum) : hashnum(new_hashnum), table(new vector<Lexeme>[new_hashnum]) {}

// Деструктор
//VariableTable::~VariableTable()
//{
//    for(int i = 0; i < hashnum; i++)
//        table[i].clear();
//    delete [] table;
//}

VariableTable::~VariableTable() {
    table->clear();
    cout << "Object VariableTable is deleted!" << endl;
}

// Подсчет хэша
int VariableTable::get_hash(const string& name) const
{
    if (name.empty()) return 0;
    int hash = 0;
    for (char c : name) {
        hash += 31 * hash + c;
    }
    return hash % hashnum;
}

void VariableTable::rehash()
{
    int newHash = hashnum * 2;  // Double the capacity
    auto *newTable(reinterpret_cast<vector<Lexeme> *>(newHash));


//    for (const auto& entry : table[h]) {
//        if (entry.name == name) return false;
//    }
    // Rehash all existing elements to the new table
    for (const auto& entry : table[hashnum]) {
        size_t newIndex = get_hash(entry.name) % newHash;
        size_t originalIndex = newIndex;
        newIndex = (newIndex + 1) % newHash;
        if (newIndex == originalIndex) break;
    }

    // Replace the old table with the new table
    table = std::move(newTable);
    hashnum = newHash;
}

//int VariableTable::get_hash(const string& ind_name) const {
//    if (ind_name.empty()) return 0; // Handle empty strings
//    if('0' <= ind_name[0] && ind_name[0] <= '9')
//        return ind_name[0] - '0';
//    if('a' <= ind_name[0] && ind_name[0] <= 'z')
//        return ind_name[0] - 'a' + 10;
//    if('A' <= ind_name[0] && ind_name[0] <= 'Z')
//        return ind_name[0] - 'A' + 36;
//    return 0;
//}

// Подсчет номера в цепочке
int VariableTable::get_chain(const string& name) const
{
    int h = get_hash(name);
    for (size_t i = 0; i < table[h].size(); ++i) {
        if (name == table[h][i].name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}


// Проверка есть ли элемент в таблице
//inline bool VariableTable::contains(const string& name)

bool VariableTable::contains(const string& name) const {
    return get_chain(name) != -1;
}

// Добавление нового имени идентификатора или значения константы
bool VariableTable::add(const string& name, const int& lexemeCode)
{
    if (contains(name)) {
        cerr << "This identification is already added!" << endl;
        return false;
    }
    int h = get_hash(name);
    for (const auto& entry : table[h]) {
        if (entry.name == name) return false;
    }
    table[h].emplace_back(name);
    return true;
}

bool VariableTable::loadFromFile(const string& filename)
{
    auto file = ifstream(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }

    string elem;
    int lexemeCode;
    while (file >> elem >> lexemeCode) {  // Correct EOF handling
        // Try to determine if the lexeme is an int, float, or string
//        LexemeType type = determineLexemeType(line);

        // Decide whether to add as an identifier (30) or constant (40)
//        int lexemeCode;
//
//        if (type == LexemeType::Int || type == LexemeType::Float) {
//            lexemeCode = 40; // Assume numeric literals are constants
//        } else {
//            lexemeCode = 30; // Treat other as identifiers
//        }

        //add(line, lexemeCode);
        add(elem, lexemeCode);
    }

    file.close();
    return true;
}


// Задание кода лексемы по хэшу и номеру в цепочке
bool VariableTable::set_lexeme_code(int hash, int chain, int lexemeCode)
{
    if (chain == -1 || hash < 0 || hash >= hashnum || hash > default_hashnum) {
        cerr << "Hash was counted not correct!" << endl;
        return false;
    }
    if (chain < 0 || chain >= static_cast<int>(table[hash].size())) {
        cerr << "Chain number is incorrect!" << endl;
        return false;
    }
    table[hash][chain].lexemeCode = lexemeCode;
    return true;
}

// Задание кода лексемы по имени идентификатора или значению константы
bool VariableTable::set_lexeme_code(const string& name, int lexemeCode)
{
    int hash = get_hash(name);
    int chain = get_chain(name);
    return set_lexeme_code(hash, chain, lexemeCode);
}

// Задание типа по хэшу и номеру в цепочке
bool VariableTable::set_type(int hash, int chain, int type)
{
    if (chain == -1 || hash < 0 || hash >= hashnum || hash > default_hashnum) {
        cerr << "Hash was counted not correct!" << endl;
        return false;
    }
    if (chain < 0 || chain >= static_cast<int>(table[hash].size())) {
        cerr << "Chain number is incorrect!" << endl;
        return false;
    }
    table[hash][chain].type = type;
    return true;
}

// Задание типа по имени идентификатора или значению константы
bool VariableTable::set_type(const string& name, int type)
{
    int hash = get_hash(name);
    int chain = get_chain(name);
    return set_type(hash, chain, type);
}

// Задание размерности по хэшу и номеру в цепочке
bool VariableTable::set_dimension(int hash, int chain, int dimension)
{
    if (chain == -1 || hash < 0 || hash >= hashnum) {
        cerr << "Hash was counted not correct!" << endl;
        return false;
    }
    if (chain < 0 || chain >= table[hash].size()) {
        cerr << "Chain number is incorrect!" << endl;
        return false;
    }
    table[hash][chain].dimension = dimension;
    table[hash][chain].is_init.resize(dimension,false);
    return true;
}

// Задание размерности по имени идентификатора или значению константы
bool VariableTable::set_dimension(const string& name, int dimension)
{
    int hash = get_hash(name);
    int chain = get_chain(name);
    return set_dimension(hash, chain, dimension);
}

// Задание флага инициализации для массивов по хэшу и номеру в цепочке
bool VariableTable::set_is_init(int hash, int chain, bool is_init, int init_index)
{
    if (chain == -1 || hash < 0 || hash >= hashnum) {
        cerr << "Hash was counted not correct!" << endl;
        return false;
    }
    if (chain < 0 || chain >= static_cast<int>(table[hash].size())) {
        cerr << "Chain number is incorrect!" << endl;
        return false;
    }
    if (init_index < 0 || init_index >= static_cast<int>(table[hash][chain].is_init.size())) {
        cerr << "Init index is not in current interval!" << endl;
        return false;
    }
    table[hash][chain].is_init[init_index] = is_init;
    return true;
}

// Задание флага инициализации для массивов по имени идентификатора или значению константы
bool VariableTable::set_is_init(const string& name, bool is_init, int init_index)
{
    int hash = get_hash(name);
    int chain = get_chain(name);
    return set_is_init(hash, chain, is_init, init_index);
}

// Задание флага инициализации по хэшу и номеру в цепочке
bool VariableTable::set_is_init(int hash, int chain, bool is_init)
{
    return set_is_init(hash, chain, is_init, 0);
}

// Задание флага инициализации по имени идентификатора или значению константы
bool VariableTable::set_is_init(const string& name, bool is_init)
{
    return set_is_init(name, is_init, 0);
}

// Определение хэша и номера в цепочке
bool VariableTable::get_location(const string& name, int &hash, int &chain)
{
    int h = get_hash(name), c = get_chain(name);
    if (chain == -1) {
        cerr << "The element of chain is missed!" << endl;
        return false;
    }

    hash = h;
    chain = c;
    return true;
}

// Получение структуры lexeme по хэшу и номеру в цепочке
bool VariableTable::get_lexeme(int hash, int chain, Lexeme &lexeme) const
{
    if (chain == -1 || hash < 0 || hash >= hashnum) {
        cerr << "Hash was counted not correct!" << endl;
        return false;
    }
    if (chain < 0 || chain >= static_cast<int>(table[hash].size())) {
        cerr << "Chain number is incorrect!" << endl;
        return false;
    }
    lexeme = table[hash][chain];
    return true;
}

// Получение структуры lexeme по имени идентификатора или значению константы
bool VariableTable::get_lexeme(const string& name, Lexeme &lexeme) const
{
    int hash = get_hash(name), chain = get_chain(name);
    return get_lexeme(hash, chain, lexeme);
}

int VariableTable::getLexemeCode(int& code_num) const {
    return code_num;
}

//LexemeType VariableTable::determineLexemeType(const string& value) const {
//    stringstream ss(value);
//    int intValue;
//    float floatValue;
//
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
//}




