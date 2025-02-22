#ifndef FROMPLUSESTOASSEMBLYLAB1_CONSTANTS_H
#define FROMPLUSESTOASSEMBLYLAB1_CONSTANTS_H

/// <summary>
/// [EN] Libraries that we need to use
/// [RU] Библиотеки, необходимые для использования
/// </summary>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <windows.h>
#include "ConstantTable/ConstantTable.h"
#include "VariableTable/VariableTable.h"
#include "Lexeme/Lexeme.h"

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
    const string filesDir = "./txtFiles/";
    const string wordsPath = filesDir + "reserved_words.txt";
    const string wordPathNew = filesDir + "reserved_words_new.txt";
    const string identPathNew = filesDir + "identifiers_new.txt";
    const string divisorPathNew = filesDir + "separators_new.txt";
    const string constantsPathNew = filesDir + "constants_new.txt";
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
