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

    const char* cKeyWordsTableXLSX = GlobalPaths::KeyWordsTableXLSX.c_str();
    const char* cIdentifiersTableXLSX = GlobalPaths::IdentifiersTableXLSX.c_str();
    const char* cSeparatorsTableXLSX = GlobalPaths::SeparatorsTableXLSX.c_str();
    const char* cConstantsTableXLSX = GlobalPaths::ConstantsTableXLSX.c_str();
    try {
        auto KeyWordsLogger = ofstream(GlobalPaths::KeyWordsLogFile);
        auto SeparatorsLogger = ofstream(GlobalPaths::SeparatorsLogFile);
        if (!KeyWordsLogger.is_open()) {
            cerr << "Can not open " + GlobalPaths::KeyWordsLogFile + " for writing logs!" << endl;
            return 1;
        }
        if (!SeparatorsLogger.is_open()) {
            cerr << "Can not open " + GlobalPaths::SeparatorsLogFile + " for writing logs!" << endl;
            return 1;
        }
        ConstantTableV2_0 KeyWordsInit(GlobalPaths::KeyWordsTable, KeyWordsLogger);
        KeyWordsLogger << "Key Words Table Contents: " << endl;
        vector<ConstantTableEntry> KeyWordsTable = KeyWordsInit.getTable();
        int key_index = 0;
        for (const auto& entry : KeyWordsTable) {
            key_index += 1;
            outputConstantTableEntry(key_index, entry, KeyWordsLogger);
        }

        lxw_workbook* workbookC1 = workbook_new(cKeyWordsTableXLSX);
        if (!workbookC1) {
            cerr << "Error creating workbook with name " + GlobalPaths::KeyWordsTableXLSX << endl;
            return 1;
        }

        lxw_worksheet* worksheetKeyWord = workbook_add_worksheet(workbookC1, "Key Words Data");
        if (!worksheetKeyWord) {
            cerr << "Error creating worksheet!" << endl;
            workbook_close(workbookC1);
            lxw_workbook_free(workbookC1);
            return 1;
        }

        worksheet_write_string(worksheetKeyWord, 0, 0, "№", nullptr);
        worksheet_write_string(worksheetKeyWord, 0, 1, "Key Word", nullptr);
        worksheet_write_string(worksheetKeyWord, 0, 2, "Lexeme Code", nullptr);

        int KeyWordRow = 1;
        int KeyWordIndex = 0;
        for (const auto& entry : KeyWordsTable) {
            if (entry.lexemeCode == KeyWord) {
                worksheet_write_number(worksheetKeyWord, KeyWordRow, 0, KeyWordIndex++, nullptr);
                worksheet_write_string(worksheetKeyWord, KeyWordRow, 1, entry.symbol.c_str(), nullptr);
                worksheet_write_number(worksheetKeyWord, KeyWordRow, 2, entry.lexemeCode, nullptr);
            }
            else continue;
            KeyWordRow += 1;
        }
        workbook_close(workbookC1);
        cout << "Successfully wrote data to " + GlobalPaths::KeyWordsTableXLSX << endl;

        ConstantTableV2_0 SeparatorsInit(GlobalPaths::SeparatorsTable, SeparatorsLogger);
        SeparatorsLogger << "Separators Table Contents: " << endl;
        vector<ConstantTableEntry> SeparatorsTable = SeparatorsInit.getTable();
        int separate_index = 0;
        for (const auto& entry : SeparatorsTable) {
            separate_index += 1;
            outputConstantTableEntry(separate_index, entry, SeparatorsLogger);
        }

        lxw_workbook* workbookC2 = workbook_new(cSeparatorsTableXLSX);
        if (!workbookC2) {
            cerr << "Error creating workbook with name " + GlobalPaths::SeparatorsTableXLSX << endl;
            return 1;
        }
        lxw_worksheet* worksheetSeparator = workbook_add_worksheet(workbookC2, "Constant Details");
        if (!worksheetSeparator) {
            cerr << "Error creating worksheet!" << endl;
            workbook_close(workbookC2);
            lxw_workbook_free(workbookC2);
            return 1;
        }

        worksheet_write_string(worksheetSeparator, 0, 0, "№", nullptr);
        worksheet_write_string(worksheetSeparator, 0, 1, "Separator", nullptr);
        worksheet_write_string(worksheetSeparator, 0, 2, "Lexeme Code", nullptr);

        int SeparateRow = 1;
        int SeparateIndex = 0;
        for (const auto& entry : SeparatorsTable) {
            if (entry.lexemeCode == Separator) {
                worksheet_write_number(worksheetSeparator, SeparateRow, 0, SeparateIndex++, nullptr); // Write the index
                worksheet_write_string(worksheetSeparator, SeparateRow, 1, entry.symbol.c_str(), nullptr);
                worksheet_write_number(worksheetSeparator, SeparateRow, 2, entry.lexemeCode, nullptr);
            }
            else continue;
            SeparateRow += 1;
        }
        workbook_close(workbookC2);
        cout << "Successfully wrote data to " + GlobalPaths::SeparatorsTableXLSX << endl;

        ///* TESTING FUNCTION *///
        int indexVal = 5;
        KeyWordsLogger << "Type index for search: ";
        try {
            optional<ConstantTableEntry> KeyWordEntry = KeyWordsInit.getElementAtIndex(indexVal);
            if (KeyWordEntry.has_value()) {
                KeyWordsLogger << "Finding element at index " + to_string(indexVal) << endl;
                string symbol = KeyWordEntry->symbol;
                LanguageElementType type = KeyWordEntry->type;
                int lexemeCode = KeyWordEntry->lexemeCode;
                int lexemeTypeSize = KeyWordEntry->lexemeTypeSize;
                string typed = ConstantTableV2_0::lexemeTypeToString(type);
                KeyWordsLogger << "Symbol: " << symbol << endl;
                KeyWordsLogger << "Lexeme Type: " << typed << endl;
                KeyWordsLogger << "Lexeme Code: " << lexemeCode << endl;
                KeyWordsLogger << "Lexeme Type Size: " << lexemeTypeSize << endl;
            }
        } catch (const exception& e) {
            KeyWordsLogger << "No element found at index " + to_string(indexVal) << endl;
            throw out_of_range("No element found at index " + to_string(indexVal) +
            " (finding the value out of range)");
        }
        KeyWordsLogger << "Contains 'void': " << KeyWordsInit.contains("void") << endl;
        KeyWordsLogger << "Contains 'Unknown': " << KeyWordsInit.contains("Unknown") << endl;
        SeparatorsLogger << "Contains '::': " << SeparatorsInit.contains("::") << endl;
        int index = KeyWordsInit.getIndex("static_assert");
        KeyWordsLogger << "Index of 'static_assert': " << index << endl;
        int index1 = SeparatorsInit.getIndex("%");
        SeparatorsLogger << "Index of '%': " << index1 << endl;
        int lexemeCodo = SeparatorsInit.getLexemeCode(".*");
        SeparatorsLogger << "Lexeme code of '.*': " << lexemeCodo << endl;
        KeyWordsLogger.close();
        SeparatorsLogger.close();
    } catch (const exception& e) {
        SetColor(14,0);
        cerr << "Exception during Constant Table testing: " << e.what() << endl;
        throw e;
    }

    try {
        auto IdentifiersLogger = ofstream(GlobalPaths::IdentifiersLogFile);
        auto ConstantsLogger = ofstream(GlobalPaths::ConstantsLogFile);
        if (!IdentifiersLogger.is_open()) {
            cerr << "Can not open " + GlobalPaths::IdentifiersLogFile + " for writing logs!" << endl;
            return 1;
        }
        if (!ConstantsLogger.is_open()) {
            cerr << "Can not open " + GlobalPaths::ConstantsLogFile + " for writing logs!" << endl;
            return 1;
        }
        VariableTableV2_0 IdentifiersInit(10);
        VariableTableV2_0 ConstantsInit(10);
        SetColor(7,0);

        IdentifiersInit.addLexeme("myVariable", LexemeType::Int);
        ConstantsInit.addLexeme("-7.1", LexemeType::Float);
        IdentifiersInit.addLexeme("Who", LexemeType::Undefined);
        ConstantsInit.addLexeme("3.86745635", LexemeType::Double);
        IdentifiersInit.addLexeme("a", LexemeType::Char);
        IdentifiersInit.addLexeme("true", LexemeType::Bool);
        IdentifiersInit.addLexeme("ased", LexemeType::String);
        ConstantsInit.addLexeme("2.65", LexemeType::Float);
        ConstantsInit.addLexeme("2.87", LexemeType::Float);
        ConstantsInit.addLexeme("2.88", LexemeType::Float);
        ConstantsInit.addLexeme("2.89", LexemeType::Float);
        ConstantsInit.addLexeme("2.86", LexemeType::Float);
        ConstantsInit.addLexeme("2.85", LexemeType::Float);
        ConstantsInit.addLexeme("7", LexemeType::Int);
        ConstantsInit.addLexeme("2.84", LexemeType::Float);
        ConstantsInit.addLexeme("2.83", LexemeType::Float);
        ConstantsInit.addLexeme("2.82", LexemeType::Float);
        ConstantsInit.addLexeme("2.81", LexemeType::Float);
        ConstantsInit.addLexeme("2.80", LexemeType::Float);

        IdentifiersLogger << "Contains 'myVariable': " << IdentifiersInit.containsLexeme("myVariable") << endl;
        ConstantsLogger << "Contains '7': " << ConstantsInit.containsLexeme("7") << endl;
        IdentifiersLogger << "Contains 'Who': " << IdentifiersInit.containsLexeme("Who") << endl;
        ConstantsLogger << "Contains 'nonExistent': " << IdentifiersInit.containsLexeme("nonExistent") << endl;

        vector<string> IdentifiersNames = IdentifiersInit.get_all_names();
        vector<string> ConstantsNames = ConstantsInit.get_all_names();

        IdentifiersLogger << "Changing types for identifiers: " << endl;
        for (const string& name : IdentifiersNames) {
            LexemeAttributes attributes;
            if (IdentifiersInit.getAttribute(name, attributes)) {
                if (attributes.type == LexemeType::String) {
                    int sized = static_cast<int>(sizeof(string) * name.size());
                    IdentifiersInit.set_size(name, sized);
                    IdentifiersInit.set_code(name, Identifier);
                }
            }
        }

        IdentifiersInit.printTable(IdentifiersLogger);
        ConstantsInit.printTable(ConstantsLogger);

        if (IdentifiersInit.set_type("Who", LexemeType::Double)) {
            IdentifiersLogger << "Successfully changed 'Who' to Double." << endl;
        } else {
            IdentifiersLogger << "Failed to change 'Who' to Double (not found)." << endl;
        }

        if (IdentifiersInit.set_init("ab", true)) {
            IdentifiersLogger << "Successfully initialized 'ab'." << endl;
        } else {
            IdentifiersLogger << "Failed to initialize 'ab' (not found)." << endl;
        }

        IdentifiersInit.printTable(IdentifiersLogger);

        ConstantsInit.addLexeme("23", LexemeType::Bool);

        ConstantsInit.printTable(ConstantsLogger);

        LexemeAttributes myIntAttributes;
        myIntAttributes.type = LexemeType::Int;
        myIntAttributes.initialized = true;
        myIntAttributes.lexemeCode = Constant;
        myIntAttributes.lexemeTypeSize = sizeof(int);

        if (ConstantsInit.addAttribute("23", myIntAttributes)) {
            ConstantsLogger << "Successfully added attributes to '23'." << endl;
        } else {
            ConstantsLogger << "Failed to add attributes to '23' (not found)." << endl;
        }
        ConstantsInit.printTable(ConstantsLogger);

        lxw_workbook* workbookV1 = workbook_new(cIdentifiersTableXLSX);
        if (!workbookV1) {
            cerr << "Error creating workbook with name " + GlobalPaths::IdentifiersTableXLSX << endl;
            return 1;
        }

        lxw_worksheet* worksheetIdentifier = workbook_add_worksheet(workbookV1, "Identifiers Data");
        if (!worksheetIdentifier) {
            cerr << "Error creating worksheet!" << endl;
            workbook_close(workbookV1);
            lxw_workbook_free(workbookV1);
            return 1;
        }

        int IdentifierIndex = 0;
        worksheet_write_string(worksheetIdentifier, 0, 0, "№", nullptr);
        worksheet_write_string(worksheetIdentifier, 0, 1, "Identifier", nullptr);
        worksheet_write_string(worksheetIdentifier, 0, 2, "Lexeme Code", nullptr);

        lxw_workbook* workbookV2 = workbook_new(cConstantsTableXLSX);
        if (!workbookV2) {
            cerr << "Error creating workbook with name " + GlobalPaths::ConstantsTableXLSX << endl;
            return 1;
        }

        lxw_worksheet* worksheetConstants = workbook_add_worksheet(workbookV2, "Constants Data");
        if (!worksheetConstants) {
            cerr << "Error creating worksheet!" << endl;
            workbook_close(workbookV2);
            lxw_workbook_free(workbookV2);
            return 1;
        }
        int ConstantsIndex = 0;
        worksheet_write_string(worksheetConstants, 0, 0, "№", nullptr);
        worksheet_write_string(worksheetConstants, 0, 1, "Constant Value", nullptr);
        worksheet_write_string(worksheetConstants, 0, 2, "Lexeme Code", nullptr);

        double val;
        int IdentifiersRow = 1;
        int ConstantsRow = 1;
        for (const string& name : IdentifiersNames) {
            LexemeAttributes attributes;
            if (IdentifiersInit.getAttribute(name, attributes)) {
                if (attributes.lexemeCode == Identifier) {
                    worksheet_write_number(worksheetIdentifier, IdentifiersRow, 0, IdentifierIndex++, nullptr);
                    worksheet_write_string(worksheetIdentifier, IdentifiersRow, 1, name.c_str(), nullptr);
                    worksheet_write_number(worksheetIdentifier, IdentifiersRow, 2, attributes.lexemeCode, nullptr);  // Lexeme Code
                    IdentifiersRow++;
                }
                else if (attributes.lexemeCode == Constant) {
                    cout << "Not correct type for: " << name.c_str() << " " << Constant << endl;
                }
            }
        }
        for (const string& name : ConstantsNames) {
            LexemeAttributes attributes;
            if (ConstantsInit.getAttribute(name, attributes)) {
                if (attributes.lexemeCode == Constant) {
                    worksheet_write_number(worksheetConstants, ConstantsRow, 0, ConstantsIndex++, nullptr);
                    if (containsDot(name))
                    {
                        string NewString = name;
                        replace(NewString.begin(), NewString.end(), '.', ',');
                        val = stod(NewString);
                        worksheet_write_number(worksheetConstants, ConstantsRow, 1, val, nullptr);
                    }
                    else {
                        val = stod(name);
                        worksheet_write_number(worksheetConstants, ConstantsRow, 1, val, nullptr);
                    }
                    worksheet_write_number(worksheetConstants, ConstantsRow, 2, attributes.lexemeCode, nullptr);  // Lexeme Code
                    ConstantsRow++;
                }
                else if (attributes.lexemeCode == Identifier) {
                    cout << "Not correct type for: " << name.c_str() << " " << Identifier << endl;
                }
            }
        }

        workbook_close(workbookV1);
        cout << "Successfully wrote data to " + GlobalPaths::IdentifiersTableXLSX<< endl;
        workbook_close(workbookV2);
        cout << "Successfully wrote data to " + GlobalPaths::ConstantsTableXLSX << endl;

        LexemeAttributes retrievedAttributes;
        if (IdentifiersInit.getAttribute("Who", retrievedAttributes)) {
            IdentifiersLogger << "Successfully retrieved attributes for 'Who'." << endl;
            IdentifiersLogger << "  Type: ";
            string typed = VariableTableV2_0::lexemeTypeToString(retrievedAttributes.type);
            IdentifiersLogger << typed << endl;
            IdentifiersLogger << "  Initialized: " << retrievedAttributes.initialized << endl;
            IdentifiersLogger << "  Lexeme Code: " << retrievedAttributes.lexemeCode << endl;
            IdentifiersLogger << "  Lexeme Type Size: " << retrievedAttributes.lexemeTypeSize << endl;

            vector<string> names1 = IdentifiersInit.get_all_names();

            IdentifiersInit.remove_lexeme("myVariable");
            IdentifiersLogger << "myVariable was successfully removed" << endl;

            IdentifiersInit.printTable(IdentifiersLogger);

        } else {
            IdentifiersLogger << "Failed to retrieve attributes for 'Who' (not found)." << endl;
        }

        ConstantsInit.remove_lexeme_without_name(Constant, LexemeType::Float, false);

        ConstantsLogger << "All data was successfully removed" << endl;
        ConstantsInit.printTable(ConstantsLogger);

        LexemeAttributes nonExistingAttributes;
        if (ConstantsInit.getAttribute("nonExistingVar", nonExistingAttributes)) {
            ConstantsLogger << "Successfully retrieved attributes for 'nonExistingVar'." << endl;
        } else {
            ConstantsLogger << "Failed to retrieve attributes for 'nonExistingVar' (not found)." << endl;
        }

        ConstantsInit.printTable(ConstantsLogger);
        IdentifiersLogger.close();
        ConstantsLogger.close();
    } catch (const exception& e) {
        SetColor(14,0);
        cerr << "Exception during Variable Table testing: " << e.what() << endl;
        throw e;
    }
    return 0;
}