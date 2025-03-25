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
    const string logFilesDir = "./logFiles/";
    const string outputDir = "./outputFiles/";

    /// Current test files
    const string KeyWordsTable = txtFilesDir + "key_words.txt";
    const string SeparatorsTable = txtFilesDir + "separators.txt";
    const string IdentifiersTable = txtFilesDir + "identifiers.txt";
    const string ConstantsTable = txtFilesDir + "constants.txt";

    /// Output XLSX files
    const string KeyWordsTableXLSX = xlsxFilesDir + "Key_words.xlsx";
    const string SeparatorsTableXLSX = xlsxFilesDir + "Separators.xlsx";
    const string IdentifiersTableXLSX = xlsxFilesDir + "Identifiers.xlsx";
    const string ConstantsTableXLSX = xlsxFilesDir + "Constants.xlsx";

    /// Output logs files
    const string KeyWordsLogFile = logFilesDir + "KeyWordsOutput.log";
    const string SeparatorsLogFile = logFilesDir + "SeparatorsOutput.log";
    const string IdentifiersLogFile = logFilesDir + "IdentifiersOutput.log";
    const string ConstantsLogFile = logFilesDir + "ConstantsOutput.log";
    const string OutputsLogFile = logFilesDir + "Output.log";

    const string TokenFile = outputDir + "tokens.txt";
    const string ErrorFile = outputDir + "errorOutput.txt";
}

/// <summary>
/// [EN] Constants (Token type)
/// [RU] Константы (тип токена)
/// </summary>
typedef enum Constants {
    KeyWord = 10,
    Separator = 20,
    Identifier = 30,
    Constant = 40
} Constants;

/// <summary>
/// [EN] Token structure
/// [RU] Структура токена
/// </summary>
struct Token {
    int type;
    int tableIndex;
    int hashIndex;
};

#endif //FROMPLUSESTOASSEMBLYLAB1_CONSTANTS_H
