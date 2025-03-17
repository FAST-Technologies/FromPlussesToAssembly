#ifndef FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLEV2_0_H

/// <summary>
/// [EN] Using directives of preprocessor
/// [RU] Используемые директивы препроцессора
/// </summary>
#pragma once

/// <summary>
/// [EN] Libraries that we need to use
/// [RU] Библиотеки, необходимые для использования
/// </summary>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <limits>
#include <sstream>
#include <iostream>
#include "./../Constants.h"
#include "../SymbolTableV2_0.h"

/// <summary>
/// [EN] Global namespaces
/// [RU] Пространства глобальных имён
/// </summary>
using namespace std;

/// <summary>
/// [EN] Enum of lexeme types for Variable Tables
/// [RU] Enum различных типов лексем для переменных (хэш) таблиц
/// </summary>
enum class LexemeType {
    Undefined,
    Int,
    Float,
    Double,
    String,
    Char,
    Bool
};

/// <summary>
/// [EN] Struct of types of Variable Tables
/// [RU] Структура типов переменных (хэш) таблиц
/// </summary>
struct LexemeAttributes {
    /// Тип рассматриваемой переменной
    LexemeType type = LexemeType::Undefined;
    /// Вектор инициализации (по умолчанию false)
    bool initialized = false;
    /// Код лексемы (принимает 30 - для идентификаторов и 40 - для констант)
    int lexemeCode = 0;
    /// Размер типа лексемы (Рассматриваются int, float, double, char и string)
    int lexemeTypeSize = 0;
};

/// <summary>
/// [EN] Class of Variable Tables
/// [RU] Класс переменных (хэш таблиц)
/// </summary>
class VariableTableV2_0 {
public:
    /// <summary>
    /// [EN] Constructor for VariableTableV2_0 class
    /// [RU] Конструктор класса VariableTableV2_0
    /// </summary>
    explicit VariableTableV2_0(int initialCapacity = 100);

    /// <summary>
    /// [EN] Destructor for VariableTableV2_0 class
    /// [RU] Деструктор класса VariableTableV2_0
    /// </summary>
    ~VariableTableV2_0();

    /// <summary>
    /// [EN] Function for loading table data for hash table
    /// [RU] Функция для загрузки данных хэш таблицы
    /// </summary>
    bool loadFromFile(const string& filename);

    bool addLexeme(const string& name, int& lexemeCode, int& lexemeTypeSize); // Overloaded addLexeme

    /// <summary>
    /// [EN] Function for adding the explicit lexeme to hash table
    /// [RU] Функция для добавления лексемы в хэш-таблицу
    /// </summary>
    bool addLexeme(const string& name, LexemeType type = LexemeType::Undefined);

    /// <summary>
    /// [EN] Function for removing lexeme from hash table by lexeme name
    /// [RU] Функция для удаления лексемы из хэш-таблицы по её имени
    /// </summary>
    void remove_lexeme(const string& name);

    /// <summary>
    /// [EN] Function for removing lexeme from hash table by all parameters except of lexeme name
    /// [RU] Функция для удаления лексемы из хэш-таблицы по её параметрам за исключением имени лексемы
    /// </summary>
    void remove_lexeme_without_name(int lexemeCode, LexemeType lexemeType, bool initialized);

    /// <summary>
    /// [EN] Function for removing lexeme from hash table by lexeme type
    /// [RU] Функция для удаления лексемы из хэш-таблицы по её типу
    /// </summary>
    void remove_lexeme_by_lexemeType(LexemeType lexemeType);

    /// <summary>
    /// [EN] Function for removing lexeme from hash table by her initialisation status
    /// [RU] Функция для удаления лексемы из хэш-таблицы по её статусу инициализации
    /// </summary>
    void remove_lexeme_by_initialized_status(bool initialized);

    /// <summary>
    /// [EN] Function for removing lexeme from hash table by her lexeme code
    /// [RU] Функция для удаления лексемы из хэш-таблицы по её коду
    /// </summary>
    void remove_lexeme_by_lexemeCode(int lexemeCode);

    /// <summary>
    /// [EN] Function for adding the attribute of lexeme to hash table
    /// [RU] Функция для добавления атрибутов лексемы в хэш-таблицу
    /// </summary>
    bool addAttribute(const string& name, LexemeAttributes attributes);

    /// <summary>
    /// [EN] Function that checks the containing of lexeme in hash table
    /// [RU] Функция для проверки содержания лексемы в хэш-таблице
    /// </summary>
    [[nodiscard]] bool containsLexeme(const string& name) const;

    /// <summary>
    /// [EN] Function that gets the attribute of lexeme in hash table
    /// [RU] Функция для получения атрибута лексемы в хэш-таблице
    /// </summary>
    bool getAttribute(const string& name, LexemeAttributes& attributes) const;

    /// <summary>
    /// [EN] Function that prints the hash table
    /// [RU] Функция для вывода данных хэш-таблицы
    /// </summary>
    void printTable(ofstream& logger) const;

    /// <summary>
    /// [EN] Function that transform the lexeme type to string
    /// [RU] Функция для приведения типа лексемы к строчному виду
    /// </summary>
    static string lexemeTypeToString(LexemeType type);

    /// <summary>
    /// [EN] Function that return the lexeme size
    /// [RU] Функция, возвращающая размер лексемы
    /// </summary>
    static int lexemeSize(const string& typeStr, const string& symbol);

    /// <summary>
    /// [EN] Function that sets different type for variable
    /// [RU] Функция, устанавливающая другой тип лексемы
    /// </summary>
    bool set_type(const string& name, LexemeType newType);

    /// <summary>
    /// [EN] Function that sets different init status for variable
    /// [RU] Функция, устанавливающая другой статус инициализации лексемы
    /// </summary>
    bool set_init(const string& name, bool isInitialized);

    /// <summary>
    /// [EN] Function that sets different size for variable
    /// [RU] Функция, устанавливающая другой размер лексемы
    /// </summary>
    bool set_size(const string& name, int lexemeTypeSize);


    bool set_code(const string& name, int lexemeCode);

    /// <summary>
    /// [EN] Function that ables to get all names of variables
    /// [RU] Функция, которая позволяет получать все имена переменных
    /// </summary>
    [[nodiscard]] vector<string> get_all_names() const;


/// <summary>
/// [EN] Function for counting the hash sum
/// [RU] Функция для подсчёта хэш-суммы
/// </summary
[[nodiscard]] size_t hash(const string& key) const;

private:
    /// <summary>
    /// [EN] Hash table structure: variable name and attributes
    /// [RU] Структура хэш-таблицы: переменная и её атрибуты
    /// </summary>
    struct Entry {
        string name;
        LexemeAttributes attributes;
    };

    /// Хэш-таблица
    vector<unique_ptr<Entry>> table;

    /// <summary>
    /// [EN] Hash table capacity size
    /// [RU] Текущая вместимость хэш таблицы
    /// </summary>
    size_t capacity;

    /// <summary>
    /// [EN] Current size of hash table
    /// [RU] Текущий размер хэш таблицы
    /// </summary>
    size_t size;

    /// <summary>
    /// [EN] Coefficient of load factor for rehashing in hash table
    /// [RU] Коэффициент загрузки хэш таблицы
    /// </summary>
    double loadFactor = 0.75;

    /// <summary>
    /// [EN] Function for rehashing
    /// [RU] Функция рехеширования
    /// </summary
    void rehash();

    /// <summary>
    /// [EN] Helper function to determine LexemeType
    /// [RU] Вспомогательная функция для вычленения LexemeType
    /// </summary
    [[nodiscard]] static LexemeType determineLexemeType(const string& value) ;

    /// <summary>
    /// [EN] Function for adding the explicit lexeme to hash table
    /// [RU] Функция для добавления лексемы в хэш-таблицу
    /// </summary>
    bool addLexeme(const string& name, const string& typeStr, int& lexemeCode, int& lexemeTypeSize);
};


#endif //FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLEV2_0_H
