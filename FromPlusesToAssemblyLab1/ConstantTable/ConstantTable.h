#ifndef FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLE_H
#define FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLE_H

/// <summary>
/// [EN] Using directives of preprocessor
/// [RU] Используемые директивы препроцессора
/// </summary>
#pragma once

/// <summary>
/// [EN] Libraries that we need to use
/// [RU] Библиотеки, необходимые для использования
/// </summary>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <random>
#include <stdexcept> // For exceptions
#include <algorithm> // For std::advance

/// <summary>
/// [EN] Global namespaces
/// [RU] Пространства глобальных имён
/// </summary>
using namespace std;

/// <summary>
/// [EN] Class constant table
/// [RU] Класс постоянных таблиц ConstantTable
/// </summary>
template <typename type> class ConstantTable
{
private:
    set<type> table;
public:
    /// <summary>
    /// [EN] Constructor for ConstantTable class
    /// [RU] Конструктор класса для класса ConstantTable
    /// </summary>
    ConstantTable() = default;

    /// <summary>
    /// [EN] Destructor for ConstantTable class
    /// [RU] Деструктор класса для класса ConstantTable
    /// </summary>
    ~ConstantTable()
    {
        table.clear();
        cout << "Object ConstantTable is deleted" << endl;
    }
    // Добавление элемента в таблицу
    bool add(const type& elem)
    {
        if (contains(elem)) return false;
        table.insert(elem);
        return true;
    }
    // Чтение таблицы из файла
    bool read_file(const string& name)
    {
        auto fs = ifstream(name);  // Use string directly; modern C++
        if (!fs.is_open()) {
            cerr << "Error opening file: " << name << endl;
            throw runtime_error("The error with opening file " + name + " occurred!");
        }

        type elem;
        while (fs >> elem) {  // Correct EOF handling
            add(elem);
        }

        if (fs.bad()) {
            cerr << "Error reading from file: " << name << endl;
            throw runtime_error("The error with reading file " + name + " occurred!");
        }
        return true;
    }
    // Проверка есть ли элемент в таблице
    bool contains(const type& elem) const {
        return table.find(elem) != table.end();
    }

    // Получает номер элемента в таблице. Если элемента нет - выбрасывает
    // исключение.
    bool check(const type& elem) const
    {
        return table.count(elem) > 0;
    }

    // Поиск номера по значению
    bool get_num(const type& elem, int &num) const {
        if (!contains(elem) || !check(elem)) {
            cerr << "There is no element of table with current number!" << endl;
            throw out_of_range("There is no element of table with current number!");
        }
        num = static_cast<int>(distance(table.begin(), table.find(elem)));
        return true;
    }

    // Поиск значения по номеру
    bool get_val(int num, type &elem)
    {
        if (num < 0 || static_cast<size_t>(num) >= table.size()) {
            cerr << "Num is out of range!" << endl;
            return false;
        }
        auto it = table.begin();
        std::advance(it, num);
        elem = *it;
        return true;
    }

    // Function for output table of elements to console
    void printTable () const
    {
        int num;
        for (const auto& elem : *this) {
            if (get_num(elem, num)) {
                cout << "a.get_num(\"" << elem << "\", num): num = " << num << endl;
            } else {
                cerr << "Error: Could not get number for element: " << elem << endl;
            }
        }
    }

    int getIndex(const type& elem) const {
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i] == elem) return static_cast<int>(i);
        }
        return -1;
    }

    auto begin() const { return table.begin(); }
    auto end() const { return table.end(); }

    int getLexemeCode(int& code_num) const {
        return code_num;
    }
};

#endif //FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLE_H
