#ifndef FROMPLUSESTOASSEMBLYLAB1_LEXEME_H
#define FROMPLUSESTOASSEMBLYLAB1_LEXEME_H

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
#include <utility> // For std::move
#include <algorithm> //For std::fill
#include <iostream>
#include <random>

/// <summary>
/// [EN] Global namespaces
/// [RU] Пространства глобальных имён
/// </summary>
using namespace std;

// Класс для хранения идентификаторов и констант
class Lexeme
 {
public:
    // Имя идентификатора или значение константы
    string name;
    // Тип, 0 - не определен, 1 - int, 2 - float
    int type{0};
    // Массив флагов "инициализировано ли" размерности dimension
    vector<bool> is_init;
    // Размерность массива, для переменных и констант - 1.
    int dimension{1};

    int lexemeCode{10};
    // Конструктор по умолчанию
    Lexeme();
    // Конструктор с заданием имени идентификатора или значения константы
    explicit Lexeme(string new_name);
    // Деструктор
    ~Lexeme();
    // Оператор присваивания
    Lexeme &operator = (const Lexeme &other)
    {
        if (this != &other)
        {
            name = other.name;
            type = other.type;
            dimension = other.dimension;
            lexemeCode = other.lexemeCode;
            is_init = other.is_init;
        }
        return *this;
    }

    // Оператор < для использования в std::set и других контейнерах, требующих
    // сравнения
    bool operator < (const Lexeme& other) const{
        return name < other.name;
    }

    // Оператор == (только имя, нужно ли сравнивать и другие поля?)
    bool operator == (const Lexeme& other) const {
        return name == other.name;
    }

    // Copy Constructor (Important!)
    Lexeme(const Lexeme& other) : name(other.name), type(other.type), dimension(other.dimension), lexemeCode(other.lexemeCode), is_init(other.is_init) {}

    void print() const {
        cout << "Name: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "Dimension: " << dimension << endl;
        cout << "Lexeme Code: " << lexemeCode << endl;
        cout << "Is Initialized: ";
        for (bool init : is_init) {
            cout << init << " ";
        }
        cout << endl;
    }
 };


//class Lexeme {
//
//};


#endif //FROMPLUSESTOASSEMBLYLAB1_LEXEME_H
