/// <summary>
/// [EN] Linked libraries and headers
/// [RU] Подключаемые библиотеки и заголовки
/// </summary>
#include "ConstantTableV2_0/ConstantTableV2_0.h"
#include "VariableTableV2_0/VariableTableV2_0.h"
#include "OutputModuleV2_0/OutputModuleV2_0.h"
#include "SymbolTableV2_0.h"
#include "Constants.h"
/// Library for work with xlsx files under C++
#include <xlsxwriter.h>

int main()
{
    setlocale(LC_ALL, "");
    cout.flags(ios::fixed);
    cout.setf(ios_base::fixed);

    /// Использование лямбда - функции
    auto hello { [](){ cout << "Welcome to first Assembly lab V2_0!" << endl; } };
    hello();

    const char* cConstantTableXLSX = GlobalPaths::ConstantTableXLSX.c_str();
    const char* cVariableTableXLSX = GlobalPaths::VariableTableXLSX.c_str();
    // **Constant Table Testing**
    try {
        ConstantTableV2_0 constantTableV2_0(GlobalPaths::ConstantTableD);

        cout << "Constant Table Contents: " << endl;
        vector<ConstantTableEntry> table = constantTableV2_0.getTable();
        int num = 0;
        for (const auto& entry : table) {
            num += 1;
            outputConstantTableEntry(num, entry);
        }

        lxw_workbook* workbook1 = workbook_new(cConstantTableXLSX);
        if (!workbook1) {
            cerr << "Error creating workbook!" << endl;
            return 1;
        }

        lxw_worksheet* worksheet1 = workbook_add_worksheet(workbook1, "Constant Data");
        if (!worksheet1) {
            cerr << "Error creating worksheet!" << endl;
            workbook_close(workbook1);
            lxw_workbook_free(workbook1);
            return 1;
        }

        vector<ConstantTableEntry> table1 = constantTableV2_0.getTable();

        worksheet_write_string(worksheet1, 0, 0, "Name", nullptr);
        worksheet_write_string(worksheet1, 0, 1, "Type", nullptr);
        worksheet_write_string(worksheet1, 0, 2, "Lexeme Code", nullptr);
        worksheet_write_string(worksheet1, 0, 3, "Lexeme Type Size", nullptr);

        int row = 1;

        for (const auto& entry : table1) {
            worksheet_write_string(worksheet1, row, 0, entry.symbol.c_str(), nullptr);
            string typeString = ConstantTableV2_0::lexemeTypeToString(entry.type);
            worksheet_write_string(worksheet1, row, 1, typeString.c_str(), nullptr);
            worksheet_write_number(worksheet1, row, 2, entry.lexemeCode, nullptr);
            worksheet_write_number(worksheet1, row, 3, entry.lexemeTypeSize, nullptr);
            row += 1;
        }

        workbook_close(workbook1);
        cout << "Successfully wrote data to " + GlobalPaths::ConstantTableXLSX << endl;

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
        cout << "Contains 'void': " << constantTableV2_0.contains("void") << endl;
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
        if (VariableTableV2_0.loadFromFile(GlobalPaths::VariableTableD)) {
            cout << "Successfully loaded from file!" << endl;
        } else {
            SetColor(14,0);
            cerr << "Failed to load from file." << endl;
        }
        SetColor(15,0);

        lxw_workbook* workbook = workbook_new(cVariableTableXLSX);
        if (!workbook) {
            cerr << "Error creating workbook!" << endl;
            return 1;
        }

        lxw_worksheet* worksheet = workbook_add_worksheet(workbook, "Variable Data");
        if (!worksheet) {
            cerr << "Error creating worksheet!" << endl;
            workbook_close(workbook);
            lxw_workbook_free(workbook);
            return 1;
        }

        worksheet_write_string(worksheet, 0, 0, "Name", nullptr);
        worksheet_write_string(worksheet, 0, 1, "Type", nullptr);
        worksheet_write_string(worksheet, 0, 2, "Initialized", nullptr);
        worksheet_write_string(worksheet, 0, 3, "Lexeme Code", nullptr);
        worksheet_write_string(worksheet, 0, 4, "Lexeme Type Size", nullptr);
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
                    int sized = static_cast<int>(sizeof(string) * name.size());
                    VariableTableV2_0.set_size(name, sized);
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
                default: cout << "Unknown"; break;
            }
            cout << endl;
            cout << "  Initialized: " << retrievedAttributes.initialized << endl;
            cout << "  Lexeme Code: " << retrievedAttributes.lexemeCode << endl;
            cout << "  Lexeme Type Size: " << retrievedAttributes.lexemeTypeSize << endl;

            vector<string> names1 = VariableTableV2_0.get_all_names();

            int row = 1;
            for (const string& name : names) {
                LexemeAttributes attributes;
                if (VariableTableV2_0.getAttribute(name, attributes)) {
                    worksheet_write_string(worksheet, row, 0, name.c_str(), nullptr);

                    string typeString;
                    switch (attributes.type) {
                        case LexemeType::Int: typeString = "Int"; break;
                        case LexemeType::Float: typeString = "Float"; break;
                        case LexemeType::String: typeString = "String"; break;
                        case LexemeType::Bool: typeString = "Bool"; break;
                        case LexemeType::Char: typeString = "Char"; break;
                        case LexemeType::Double: typeString = "Double"; break;
                        default: typeString = "Undefined"; break;
                    }
                    worksheet_write_string(worksheet, row, 1, typeString.c_str(), nullptr); // Type

                    worksheet_write_boolean(worksheet, row, 2, attributes.initialized, nullptr); // Initialized
                    worksheet_write_number(worksheet, row, 3, attributes.lexemeCode, nullptr);  // Lexeme Code
                    worksheet_write_number(worksheet, row, 4, attributes.lexemeTypeSize, nullptr); // Lexeme Type Size

                    row++;
                }
            }

            workbook_close(workbook);
            cout << "Successfully wrote data to " + GlobalPaths::VariableTableXLSX<< endl;

            VariableTableV2_0.remove_lexeme("myVariable");
            cout << "myVariable was successfully removed" << endl;

            VariableTableV2_0.printTable();

            VariableTableV2_0.remove_lexeme_without_name(40, LexemeType::Int, false);

            cout << "All data was successfully removed" << endl;
            VariableTableV2_0.printTable();

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