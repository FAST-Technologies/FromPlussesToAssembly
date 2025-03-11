/// <summary>
/// [EN] Linked libraries and headers
/// [RU] Подключаемые библиотеки и заголовки
/// </summary>
#include "ConstantTableV2_0.h"

/// <summary>
/// [EN] Constructor for ConstantTableV2_0 class
/// [RU] Конструктор класса ConstantTableV2_0
/// </summary>
ConstantTableV2_0::ConstantTableV2_0(const string& filename, ofstream& logger)
{
    if (!loadFromFile(filename, logger)) {
        cerr << "Error loading constant table from file: " << filename << endl;
        throw runtime_error("Error loading constant table from file: " + filename);
    }
}

/// <summary>
/// [EN] Destructor for ConstantTableV2_0 class
/// [RU] Деструктор класса ConstantTableV2_0
/// </summary>
ConstantTableV2_0::~ConstantTableV2_0()
{
    table.clear();
    cout << "Object ConstantTable is deleted" << endl;
}

/// <summary>
/// [EN] Bool function of getting constant table from file
/// [RU] Булева функция получения таблицы из файла
/// </summary>
bool ConstantTableV2_0::loadFromFile(const string& filename, ofstream& logger)
{
    auto file = ifstream(filename);
    if (!file.is_open()) {
        return false;
    }
    string symbol, typeStr;
    int lexemeCode;

    while (file >> symbol >> typeStr >> lexemeCode) {
        LanguageElementType type;

        if (typeStr == "Alphabet")       type = LanguageElementType::Alphabet;
        else if (typeStr == "ReservedWord") type = LanguageElementType::ReservedWord;
        else if (typeStr == "OperatorSign") type = LanguageElementType::OperatorSign;
        else if (typeStr == "Delimiter")  type = LanguageElementType::Delimiter;
        else                              type = LanguageElementType::Other;

        int symbolSize = static_cast<int>(symbol.size());
        int lexemeTypeSize =
                (symbolSize == 1 ?
                static_cast<int>(sizeof(char)) :
                static_cast<int>(sizeof(string) * symbolSize));
        add(symbol, type, lexemeCode, lexemeTypeSize, logger);
    }

    if (file.bad()) {
        cerr << "Error reading from file: " << filename << endl;
        throw runtime_error("The error with reading file " + filename + " occurred!");
    }

    file.close();
    sort(table.begin(), table.end());
    return true;
}

/// <summary>
/// [EN] String function that gets a type of element from constant table
/// [RU] Строчная функция получения типа элемента из таблицы констант
/// </summary>
string ConstantTableV2_0::lexemeTypeToString(LanguageElementType type)
{
    switch (type) {
        case LanguageElementType::Alphabet: return "Alphabet";
        case LanguageElementType::ReservedWord: return "ReservedWord";
        case LanguageElementType::OperatorSign: return "OperatorSign";
        case LanguageElementType::Delimiter: return "Delimiter";
        case LanguageElementType::Other: return "Other";
        default: return "Unknown";
    }
}

/// <summary>
/// [EN] Function for adding element to constant table
/// [RU] Функция добавления элемента в упорядоченную таблицу
/// </summary>
bool ConstantTableV2_0::add(const string& symbol, LanguageElementType type, int lexemeCode, int lexemeTypeSize, ofstream& logger)
{
    if (contains(symbol)) {
        logger << "Error: The current element is already in the table! "  << symbol << endl;
        return false;
    }
    table.push_back({symbol, type, lexemeCode, lexemeTypeSize});
    sort(table.begin(), table.end());
    return true;
}

/// <summary>
/// [EN] Function to check if element already contains in constant table
/// [RU] Функция проверки, что элемент уже содержится в упорядоченной таблице
/// </summary>
bool ConstantTableV2_0::contains(const string& symbol) const
{
    if (!check_empty()) return false;
    return binarySearch(symbol) != -1;
}

/// <summary>
/// [EN] Function to take an index of element from constant table
/// [RU] Функция получения индекса элемента в упорядоченной таблице
/// </summary>
int ConstantTableV2_0::getIndex(const string& symbol) const
{
    int index = binarySearch(symbol);
    return index;
}

/// <summary>
/// [EN] Function for checking constant table on empty condition
/// [RU] Функция проверки упорядоченной таблицы на пустоту
/// </summary>
bool ConstantTableV2_0::check_empty() const
{
    return !table.empty();
}

/// <summary>
/// [EN] Function to take code of lexeme in constant table by her name
/// [RU] Функция получения кода лексемы в упорядоченной таблице по её названию
/// </summary>
int ConstantTableV2_0::getLexemeCode(const string& symbol) const
{
    int index = binarySearch(symbol);
    return table[index].lexemeCode;
}

/// <summary>
/// [EN] Function to take element and his fields from constant table
/// [RU] Функция получения элемента и его полей по индексу в упорядоченной таблице
/// </summary>
optional<ConstantTableEntry> ConstantTableV2_0::getElementAtIndex(int index) const
{
    if (index < 0 || index >= static_cast<int>(table.size())) {
        return std::nullopt;
    }
    return table[index];
}

/// <summary>
/// [EN] Function to check element in constant table
/// [RU] Функция проверки содержания элемента в упорядоченной таблице
/// </summary>
bool ConstantTableV2_0::check(const string& symbol) const
{
    if (binarySearch(symbol) == -1) {
        throw out_of_range("Element not found in ConstantTable: " + symbol);
    }
    return true;
}

/// <summary>
/// [EN] Function of binary search in constant table
/// [RU] Функция бинарного поиска в упорядоченной таблице
/// </summary>
int ConstantTableV2_0::binarySearch(const string& symbol) const
{
    int low = 0;
    int high = static_cast<int>(table.size() - 1);
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (table[mid].symbol == symbol) return mid;
        else if (table[mid].symbol < symbol) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/// <summary>
/// [EN] Bool function of addition element for abstract method
/// [RU] Булевая функция добавления для работы абстрактного метода
/// </summary>
bool ConstantTableV2_0::add(const string& symbol)
{
    return false;
}
