#include "ConstantTableV2_0.h"

ConstantTableV2_0::ConstantTableV2_0(const string& filename) {
    if (!loadFromFile(filename)) {
        cerr << "Error loading constant table from file: " << filename << endl;
        // Handle error appropriately (e.g., throw exception)
    }
}

bool ConstantTableV2_0::loadFromFile(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    string symbol;
    string typeStr;
    int lexemeCode;

    while (file >> symbol >> typeStr >> lexemeCode) {
        LanguageElementType type;

        if (typeStr == "Alphabet")       type = LanguageElementType::Alphabet;
        else if (typeStr == "ReservedWord") type = LanguageElementType::ReservedWord;
        else if (typeStr == "OperatorSign") type = LanguageElementType::OperatorSign;
        else if (typeStr == "Delimiter")  type = LanguageElementType::Delimiter;
        else                              type = LanguageElementType::Other;
        // Add more type parsing as needed
        int symbolSize = static_cast<int>(symbol.size());
        int lexemeTypeSize = (symbolSize == 1 ? static_cast<int>(sizeof(char)) : static_cast<int>(sizeof(string) * symbolSize));
        add(symbol, type, lexemeCode, lexemeTypeSize);
    }

    file.close();
    sort(table.begin(), table.end()); // Sort for binary search
    return true;
}

bool ConstantTableV2_0::add(const string& symbol, LanguageElementType type, int lexemeCode, int lexemeTypeSize) {
    if (contains(symbol)) return false;
    table.push_back({symbol, type, lexemeCode, lexemeTypeSize});
    sort(table.begin(), table.end()); // Maintain sorted order
    return true;
}

bool ConstantTableV2_0::contains(const string& symbol) const {
    return binarySearch(symbol) != -1; // Use binary search for efficiency
}

int ConstantTableV2_0::getIndex(const string& symbol) const {
    int index = binarySearch(symbol);
    return index;
}

int ConstantTableV2_0::getLexemeCode() const {
    return 0; // General-purpose constant table, lexeme code not always relevant
}

// Binary Search Algorithm (for sorted table)
int ConstantTableV2_0::binarySearch(const string& symbol) const {
    int low = 0;
    int high = static_cast<int>(table.size() - 1);

    while (low <= high) {
        int mid = low + (high - low) / 2; // Prevent potential overflow

        if (table[mid].symbol == symbol) {
            return mid;
        } else if (table[mid].symbol < symbol) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1; // Not found
}

bool ConstantTableV2_0::add(const string& symbol) {
    return false; //Implementation of other abstract method
}
