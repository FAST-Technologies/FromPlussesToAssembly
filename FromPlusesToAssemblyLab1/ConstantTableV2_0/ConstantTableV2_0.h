#ifndef FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLEV2_0_H

/// <summary>
/// [EN] Using directives of preprocessor
/// [RU] Используемые директивы препроцессора
/// </summary>
#pragma once

/// <summary>
/// [EN] Libraries that we need to use
/// [RU] Библиотеки, необходимые для использования
/// </summary>
#include "../SymbolTableV2_0.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

/// <summary>
/// [EN] Global namespaces
/// [RU] Пространства глобальных имён
/// </summary>
using namespace std;

/// <summary>
/// [EN] Enum of different types for Constant Tables
/// [RU] Enum различных типов для постоянных (упорядоченных) таблиц
/// </summary>
enum class LanguageElementType {
    Alphabet,
    ReservedWord,
    OperatorSign,
    Delimiter,
    Other
};

/// <summary>
/// [EN] Struct of types of Constant Tables
/// [RU] Структура типов постоянных (упорядоченных) таблиц
/// </summary>
struct ConstantTableEntry {
    /// Символьная строка
    string symbol;
    /// Тип рассматриваемого языка
    LanguageElementType type;
    /// Код лексемы (принимает 10 - для ключевых слов и 20 - для разделителей)
    int lexemeCode = 0;
    /// Размер типа лексемы (В данный момент рассматривается char и string)
    int lexemeTypeSize = 0;
    /// Булевый оператор сравнения для поиска-сортировки
    bool operator<(const ConstantTableEntry& other) const {
        return symbol < other.symbol;
    }

    /// Булевый оператор присваивания для поиска-сортировки
    bool operator == (const ConstantTableEntry& other) const {
        return symbol == other.symbol;
    }
};

/// <summary>
/// [EN] Class of Constant Tables
/// [RU] Класс постоянных (упорядоченных таблиц)
/// </summary>
class ConstantTableV2_0 : public SymbolTable {
public:
    /// <summary>
    /// [EN] Constructor for ConstantTableV2_0 class
    /// [RU] Конструктор класса ConstantTableV2_0
    /// </summary>
    explicit ConstantTableV2_0(const string& filename);

    /// <summary>
    /// [EN] Destructor for ConstantTableV2_0 class
    /// [RU] Деструктор класса ConstantTableV2_0
    /// </summary>
    ~ConstantTableV2_0() override;

    /// <summary>
    /// [EN] Bool function of addition element for abstract method
    /// [RU] Булевая функция добавления для работы абстрактного метода
    /// </summary>
    bool add(const string& symbol) override;

    /// <summary>
    /// [EN] Function for adding element to constant table
    /// [RU] Функция добавления элемента в упорядоченную таблицу
    /// </summary>
    bool add(const string& symbol, LanguageElementType type, int lexemeCode, int lexemeTypeSize);

    /// <summary>
    /// [EN] String function that gets a type of element from constant table
    /// [RU] Строчная функция получения типа элемента из таблицы констант
    /// </summary>
    static string lexemeTypeToString(LanguageElementType type);

    /// <summary>
    /// [EN] Function to check if element already contains in constant table
    /// [RU] Функция проверки содержания элемента в упорядоченной таблице
    /// </summary>
    [[nodiscard]] bool contains(const string& symbol) const override;

    /// <summary>
    /// [EN] Function to take an index of element from constant table
    /// [RU] Функция получения индекса элемента в упорядоченной таблице
    /// </summary>
    [[nodiscard]] int getIndex(const string& symbol) const override;

    /// <summary>
    /// [EN] Function to take code of lexeme in constant table by her name (not implemented)
    /// [RU] Функция получения кода лексемы в упорядоченной таблице по её названию (в процессе)
    /// </summary>
    [[nodiscard]] int getLexemeCode(const string& symbol) const override;

    /// <summary>
    /// [EN] Function to take element and his fields from constant table
    /// [RU] Функция получения элемента и его полей по индексу в упорядоченной таблице
    /// </summary>
    [[nodiscard]] optional<ConstantTableEntry> getElementAtIndex(int index) const;

    /// <summary>
    /// [EN] Function of binary search in constant table
    /// [RU] Функция бинарного поиска в упорядоченной таблице
    /// </summary>
    [[nodiscard]] int binarySearch(const string& symbol) const;

    /// <summary>
    /// [EN] Function for checking constant table on empty condition
    /// [RU] Функция проверки упорядоченной таблицы на пустоту
    /// </summary>
    [[nodiscard]] bool check_empty() const;

    /// <summary>
    /// [EN] Function to check element in constant table
    /// [RU] Функция проверки содержания элемента в упорядоченной таблице
    /// </summary>
    [[nodiscard]] bool check(const string& symbol) const;

    /// <summary>
    /// [EN] Function to get constant table
    /// [RU] Функция получения упорядоченной таблицы
    /// </summary>
    [[nodiscard]] vector<ConstantTableEntry> getTable() const { return table; }

    [[nodiscard]] auto begin() const { return table.begin(); }
    [[nodiscard]] auto end() const { return table.end(); }

private:
    /// Вектор структур упорядоченных таблиц
    vector<ConstantTableEntry> table;

    /// <summary>
    /// [EN] Bool function of getting constant table from file
    /// [RU] Булева функция получения таблицы из файла
    /// </summary>
    bool loadFromFile(const string& filename);
};

#endif //FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLEV2_0_H
