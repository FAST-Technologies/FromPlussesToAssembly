#include "Lexeme.h"

// Конструктор по умолчанию
Lexeme::Lexeme() : name(), type(0), dimension(1), lexemeCode(10), is_init() {}

// Конструктор с заданием имени идентификатора или значения константы
//lexeme::lexeme(string new_name)
//{
//    name = std::move(new_name);
//    type = 0;
//    is_init.push_back(false);
//    dimension = 1;
//}

// Конструктор с заданием имени идентификатора или значения константы
Lexeme::Lexeme(string new_name) :
    name(std::move(new_name)),
    type(0),
    dimension(1),
    lexemeCode(10),
    is_init(1, false) {}

// Деструктор
Lexeme::~Lexeme() {
    cout << "Object Lexeme is deleted" << endl;
}

