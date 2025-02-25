#ifndef FROMPLUSESTOASSEMBLYLAB1_CONSTANTS_H
#define FROMPLUSESTOASSEMBLYLAB1_CONSTANTS_H

/// <summary>
/// [EN] Libraries that we need to use
/// [RU] Библиотеки, необходимые для использования
/// </summary>
#include <iostream>
#include <stdexcept>
#include <fstream>
/// Library for windows console and utils
#include <windows.h>


/// <summary>
/// [EN] Global namespaces
/// [RU] Пространства глобальных имён
/// </summary>
using namespace std;

/// <summary>
/// [EN] The paths to files
/// [RU] Пути до файлов
/// </summary>
namespace GlobalPaths {
    /// Paths to files (txt && xlsx)
    const string txtFilesDir = "./txtFiles/";
    const string xlsxFilesDir = "./xlsxFiles/";
    /// First attempt file
    const string wordsPath = txtFilesDir + "reserved_words.txt";

    /// How it should be (files divided by tables)
    const string wordPathNew = txtFilesDir + "reserved_words_new.txt";
    const string identPathNew = txtFilesDir + "identifiers_new.txt";
    const string divisorPathNew = txtFilesDir + "separators_new.txt";
    const string constantsPathNew = txtFilesDir + "constants_new.txt";

    /// Current test files
    const string ConstantTableD = txtFilesDir + "constant_table_data.txt";
    const string VariableTableD = txtFilesDir + "variable_table_data.txt";

    /// output XLSX files
    const string ConstantTableXLSX = xlsxFilesDir + "ConConstant_table.xlsx";
    const string VariableTableXLSX = xlsxFilesDir + "VarVariable_table.xlsx";
}

/// <summary>
/// [EN] Constants
/// [RU] Константы
/// </summary>
typedef enum Constants {
    KeyWord = 10,
    Separator = 20,
    Identifier = 30,
    Constant = 40
} Constants;

#endif //FROMPLUSESTOASSEMBLYLAB1_CONSTANTS_H
