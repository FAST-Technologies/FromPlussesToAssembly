#include "ConstantTableV2_0/ConstantTableV2_0.h"
#include "VariableTableV2_0/VariableTableV2_0.h"
#include "SymbolTableV2_0.h"
//#include <xlsxwriter.h>


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
                string typed = ConstantTableV2_0::lexemeTypeToString(type);
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
        cout << "Contains '7': " << VariableTableV2_0.containsLexeme("7") << endl;
        cout << "Contains 'Who': " << VariableTableV2_0.containsLexeme("Who") << endl;
        cout << "Contains 'nonExistent': " << VariableTableV2_0.containsLexeme("nonExistent") << endl;

        vector<string> names = VariableTableV2_0.get_all_names();

        cout << "Changing types: " << endl;
        for (const string& name : names) {
            LexemeAttributes attributes;
            if (VariableTableV2_0.getAttribute(name, attributes)) {
                if (attributes.type == LexemeType::String) {
                    int sized = sizeof(string) * static_cast<int>(name.size());
                    VariableTableV2_0.set_size(name, sized);
//                    if (VariableTableV2_0.set_type(name, LexemeType::Double)) {
//                        std::cout << "Changed type of '" << name << "' from String to Double." << std::endl;
//                    } else {
//                        std::cerr << "Failed to set type of '" << name << "'." << std::endl;
//                    }
                }
            }
        }

        VariableTableV2_0.printTable();

        if (VariableTableV2_0.set_type("Who", LexemeType::Double)) {
            cout << "Successfully changed 'Who' to Double." << endl;
        } else {
            cout << "Failed to change 'Who' to Double (not found)." << endl;
        }

        if (VariableTableV2_0.set_init("ab", true)) {
            cout << "Successfully initialized 'ab'." << endl;
        } else {
            cout << "Failed to initialize 'ab' (not found)." << endl;
        }

        VariableTableV2_0.printTable();

        // 1. Add a variable
        VariableTableV2_0.addLexeme("myVal", LexemeType::Bool);

        VariableTableV2_0.printTable();

        // 2. Create a LexemeAttributes object with the desired attributes
        LexemeAttributes myIntAttributes;
        myIntAttributes.type = LexemeType::Int;
        myIntAttributes.initialized = true;
        myIntAttributes.lexemeCode = 40;  // Constant
        myIntAttributes.lexemeTypeSize = sizeof(int);

        // 3. Add the attributes to the variable "myInt"
        if (VariableTableV2_0.addAttribute("myVal", myIntAttributes)) {
            cout << "Successfully added attributes to 'myVal'." << endl;
        } else {
            cout << "Failed to add attributes to 'myVal' (not found)." << endl;
        }

        VariableTableV2_0.printTable();

        // 4. Retrieve the attributes of "myInt"
        LexemeAttributes retrievedAttributes;
        if (VariableTableV2_0.getAttribute("myVal", retrievedAttributes)) {
            cout << "Successfully retrieved attributes for 'myVal'." << endl;
            cout << "  Type: ";
            switch (retrievedAttributes.type) {
                case LexemeType::Int: cout << "Int"; break;
                case LexemeType::Float: cout << "Float"; break;
                case LexemeType::String: cout << "String"; break;
                    // ... handle other LexemeType values ...
                default: cout << "Unknown"; break;
            }
            cout << endl;
            cout << "  Initialized: " << retrievedAttributes.initialized << endl;
            cout << "  Lexeme Code: " << retrievedAttributes.lexemeCode << endl;
            cout << "  Lexeme Type Size: " << retrievedAttributes.lexemeTypeSize << endl;
        } else {
            cout << "Failed to retrieve attributes for 'myInt' (not found)." << endl;
        }

        //5. Attempt to retrieve attributes for a non-existing variable
        LexemeAttributes nonExistingAttributes;
        if (VariableTableV2_0.getAttribute("nonExistingVar", nonExistingAttributes)) {
            cout << "Successfully retrieved attributes for 'nonExistingVar'." << endl;
        } else {
            cout << "Failed to retrieve attributes for 'nonExistingVar' (not found)." << endl;
        }

        VariableTableV2_0.printTable();

    } catch (const exception& e) {
        SetColor(14,0);
        cerr << "Exception during Variable Table testing: " << e.what() << endl;
        throw e;
    }

    return 0;
}