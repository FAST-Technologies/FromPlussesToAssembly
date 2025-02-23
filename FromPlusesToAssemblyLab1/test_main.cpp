#include "ConstantTableV2_0/ConstantTableV2_0.h"
#include "VariableTableV2_0/VariableTableV2_0.h"
#include "SymbolTableV2_0.h"


// Функция установки цвета в консоли
void SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

int main() {

    setlocale(LC_ALL, "");
    cout.flags(ios::fixed);
    cout.setf(ios_base::fixed);

    // Использование лямбда - функции
    auto hello { [](){ cout << "Welcome to first Assembly lab V2_0!" << endl; } };
    hello();
    // **Constant Table Testing**
    try {
        ConstantTableV2_0 constantTableV2_0("txtFiles/constant_table_data.txt"); // Load from file

        cout << "Constant Table Contents:" << endl;
        vector<ConstantTableEntry> table = constantTableV2_0.getTable();
        int num = 0;
        for (const auto& entry : table) {
            num += 1;
            if (entry.lexemeTypeSize == 1) {
                if (static_cast<int>(entry.type) == 1) {
                    cout << num << " : ReservedWord: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (char)" << endl;
                } else if (static_cast<int>(entry.type) == 2) {
                    cout << num << " : Operator: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (char)" << endl;
                } else if (static_cast<int>(entry.type) == 3) {
                    cout << num << " : Separator: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (char)" << endl;
                }

            } else {
                if (static_cast<int>(entry.type) == 1) {
                    cout << num << " : ReservedWord: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (string)" << endl;
                } else if (static_cast<int>(entry.type) == 2) {
                    cout << num << " : Operator: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (string)" << endl;
                } else if (static_cast<int>(entry.type) == 3) {
                    cout << num << " : Separator: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (string)" << endl;
                }
            }
        }
        int indexVal;
        cout << "Type index for search: ";
        cin >> indexVal;
        try {
            optional<ConstantTableEntry> entry1 = constantTableV2_0.getElementAtIndex(indexVal);
            if (entry1.has_value()) {
                cout << "Finding element at index " + to_string(indexVal) << endl;
                string symbol = entry1->symbol; // Using the arrow operator (->)
                LanguageElementType type = entry1->type;
                int lexemeCode = entry1->lexemeCode;
                int lexemeTypeSize = entry1->lexemeTypeSize;
                string typed = constantTableV2_0.lexemeTypeToString(type);
                SetColor(16, 0);
                cout << "Symbol: " << symbol << endl;
                cout << "Lexeme Type: " << typed << endl;
                cout << "Lexeme Code: " << lexemeCode << endl;
                cout << "Lexeme Type Size: " << lexemeTypeSize << endl;
                SetColor(7, 0);
            }
        } catch (const exception& e) {
            cerr << "No element found at index " + to_string(indexVal) << endl;
            throw out_of_range("No element found at index " + to_string(indexVal) +
            " (finding the value out of range)");
        }

        // Test Contains
        cout << "Contains 'Begin': " << constantTableV2_0.contains("void") << endl;
        cout << "Contains 'Unknown': " << constantTableV2_0.contains("Unknown") << endl;
        cout << "Contains '::': " << constantTableV2_0.contains("::") << endl;
        // Test GetIndex
        int index = constantTableV2_0.getIndex("static_assert");
        cout << "Index of 'static_assert': " << index << endl;

        int index1 = constantTableV2_0.getIndex("%");
        cout << "Index of '%': " << index1 << endl;

        int lexemeCodo = constantTableV2_0.getLexemeCode(".*");
        cout << "Lexeme code of '.*': " << lexemeCodo << endl;

    } catch (const exception& e) {
        SetColor(14,0);
        cerr << "Exception during Constant Table testing: " << e.what() << endl;
        throw e;
    }

    // **Variable Table Testing**
    try {
        VariableTableV2_0 VariableTableV2_0(50);
        if (VariableTableV2_0.loadFromFile("txtFiles/my_data.txt")) {
            cout << "Successfully loaded from file!" << endl;
        } else {
            SetColor(14,0);
            cerr << "Failed to load from file." << endl;
        }
        SetColor(15,0);
        // Test AddLexeme
        VariableTableV2_0.addLexeme("myVariable", LexemeType::Int);
        VariableTableV2_0.addLexeme("-7.1", LexemeType::Float);
        VariableTableV2_0.addLexeme("Who", LexemeType::Undefined);
        VariableTableV2_0.addLexeme("3.86745635", LexemeType::Double);

        // Test ContainsLexeme
        cout << "Contains 'myVariable': " << VariableTableV2_0.containsLexeme("myVariable") << endl;
        cout << "Contains '7': " << VariableTableV2_0.containsLexeme("Who") << endl;
        cout << "Contains 'Who': " << VariableTableV2_0.containsLexeme("ab") << endl;
        cout << "Contains 'nonExistent': " << VariableTableV2_0.containsLexeme("nonExistent") << endl;

        VariableTableV2_0.printTable();

    } catch (const exception& e) {
        SetColor(14,0);
        cerr << "Exception during Variable Table testing: " << e.what() << endl;
        throw e;
    }

    return 0;
}