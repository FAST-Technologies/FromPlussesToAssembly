#ifndef FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLE_H
#define FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLE_H

#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <stdexcept> // For exceptions
#include <algorithm> // For std::advance

using namespace std;

// Класс постоянных таблиц
template <typename type> class ConstantTable
{
private:
    set<type> table;
public:
    // Конструктор по умолчанию
    ConstantTable() = default;
    // Деструктор
    ~ConstantTable()
    {
        table.clear();
    }
    // Добавление элемента в таблицу
    inline void add(type elem)
    {
        table.insert(elem);
    }
    // Чтение таблицы из файла
    bool read_file(const string& name)
    {
//        ifstream fs(name.c_str(), ios::in);
//        if(!fs.is_open()) return false;
//        type elem;
//        while (!fs.eof())
//        {
//            fs >> elem;
//            add(elem);
//        }
//        return true;
        ifstream fs(name);  // Use string directly; modern C++
        if (!fs.is_open()) {
            cerr << "Error opening file: " << name << endl;
            return false; // Or throw an exception: throw std::runtime_error("...");
        }

        type elem;
        while (fs >> elem) {  // Correct EOF handling
            add(elem);
        }

        if (fs.bad()) { // Check for read errors *after* the loop
            cerr << "Error reading from file: " << name << endl;
            return false; // or throw exception
        }
        return true;
    }
    // Проверка есть ли элемент в таблице
    bool contains(type elem)
    {
//        typename set<type>::iterator it = table.find(elem);
//        if(it == table.end()) return false;
//        return true;
        return table.find(elem) != table.end();
    }
    // Поиск номера по значению
    bool get_num(type elem, int &num)
    {
        if(!contains(elem)) return false;
        num = distance(table.begin(), table.find(elem));
        return true;
    }
    // Поиск значения по номеру
    bool get_val(int num, type &elem)
    {
        if(num < 0 || num >= table.size()) return false;
//        typename set<type>::iterator it = table.begin();
//        for(int i = 0; i < num; i++)
//            it++;
//        elem = *it;
//        return true;
        auto it = table.begin();
        std::advance(it, num); // More efficient than looping

        elem = *it;
        return true;
    }
};



//class ConstantTable {
//
//};


#endif //FROMPLUSESTOASSEMBLYLAB1_CONSTANTTABLE_H
