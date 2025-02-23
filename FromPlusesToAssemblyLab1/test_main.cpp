#include "ConstantTableV2_0/ConstantTableV2_0.h"
#include "VariableTableV2_0/VariableTableV2_0.h"
#include "SymbolTableV2_0.h"
#include <iostream>

int main() {
    // **Constant Table Testing**
    try {
        ConstantTableV2_0 constantTableV2_0("txtFiles/constant_table_data.txt"); // Load from file

        cout << "Constant Table Contents:" << endl;
        vector<ConstantTableEntry> table = constantTableV2_0.getTable();
        int num = 0;
        for (const auto& entry : table) {
            num += 1;
            if (entry.lexemeTypeSize == 1) {
                if (entry.lexemeCode == 10) {
                    cout << num << " : ReservedWord: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (char)" << endl;
                } else if (entry.lexemeCode == 20) {
                    cout << num << " : Separator: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (char)" << endl;
                }

            } else {
                if (entry.lexemeCode == 10) {
                    cout << num << " : ReservedWord: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (string)" << endl;
                } else if (entry.lexemeCode == 20) {
                    cout << num << " : Separator: " << entry.symbol << " Type: " << static_cast<int>(entry.type) << " Lexeme Code: " <<  entry.lexemeCode << " Lexeme Type Size: " << entry.lexemeTypeSize << " (string)" << endl;
                }
            }
        }

        // Test Contains
        cout << "Contains 'Begin': " << constantTableV2_0.contains("void") << endl;
        cout << "Contains 'Unknown': " << constantTableV2_0.contains("Unknown") << endl;

        // Test GetIndex
        int index = constantTableV2_0.getIndex("static_assert");
        cout << "Index of 'Program': " << index << endl;
    } catch (const exception& e) {
        cerr << "Exception during Constant Table testing: " << e.what() << endl;
        throw e;
    }

    // **Variable Table Testing**
    try {
        VariableTableV2_0 VariableTableV2_0(50);
        if (VariableTableV2_0.loadFromFile("txtFiles/my_data.txt")) {
            cout << "Successfully loaded from file!" << endl;
        } else {
            cerr << "Failed to load from file." << endl;
        }

        // Test AddLexeme
        VariableTableV2_0.addLexeme("myVariable", LexemeType::Int);
        VariableTableV2_0.addLexeme("7", LexemeType::Float);
        VariableTableV2_0.addLexeme("Who", LexemeType::Undefined);
        VariableTableV2_0.addLexeme("3.86745635", LexemeType::Double);

        // Test ContainsLexeme
        cout << "Contains 'myVariable': " << VariableTableV2_0.containsLexeme("myVariable") << endl;
        cout << "Contains '7': " << VariableTableV2_0.containsLexeme("Who") << endl;
        cout << "Contains 'Who': " << VariableTableV2_0.containsLexeme("ab") << endl;
        cout << "Contains 'nonExistent': " << VariableTableV2_0.containsLexeme("nonExistent") << endl;

        VariableTableV2_0.printTable();

        cout << sizeof(string) << endl;
        string as = "asd";
        cout << as.size()*sizeof(string) << endl;
        double x1 = 100.35657;
        cout << sizeof(x1) << endl;
        cout << sizeof(double) << endl;
        cout << sizeof(char) << endl;
    } catch (const exception& e) {
        cerr << "Exception during Variable Table testing: " << e.what() << endl;
        throw e;
    }

    return 0;
}