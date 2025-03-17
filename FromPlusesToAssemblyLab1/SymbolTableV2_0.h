#ifndef FROMPLUSESTOASSEMBLYLAB1_SYMBOLTABLEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_SYMBOLTABLEV2_0_H

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
#include <windows.h>

/// <summary>
/// [EN] Global namespaces
/// [RU] Пространства глобальных имён
/// </summary>
using namespace std;

/// <summary>
/// [EN] Abstract base class for symbol tables
/// [RU] Абстрактный базовый класс для символьных таблиц
/// </summary>
class SymbolTable {
public:
    /// <summary>
    /// [EN] Destructor for virtual SymbolTable class
    /// [RU] Деструктор виртуального класса SymbolTable
    /// </summary>
    virtual ~SymbolTable() = default;

    /// <summary>
    /// [EN] Virtual bool function of adding element to table
    /// [RU] Виртуальная булева функция добавления элемента в таблицу
    /// </summary>
    virtual bool add(const string& symbol) = 0;

    /// <summary>
    /// [EN] Virtual bool function of contain element in table
    /// [RU] Виртуальная булева функция содержания элемента в таблице
    /// </summary>
    virtual bool contains(const string& symbol) const = 0;

    /// <summary>
    /// [EN] Virtual int function of getting index of element in table
    /// [RU] Виртуальная целочисленная функция получения индекса элемента в таблице
    /// </summary>
    virtual int getIndex(const string& symbol) const = 0;

    /// <summary>
    /// [EN] Virtual int function of getting lexeme code of element in table
    /// [RU] Виртуальная целочисленная функция получения кода лексемы элемента в таблице
    /// </summary>
    virtual int getLexemeCode(const string& symbol) const = 0;

    virtual int size() = 0;
};

#endif //FROMPLUSESTOASSEMBLYLAB1_SYMBOLTABLEV2_0_H
