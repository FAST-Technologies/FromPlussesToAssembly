#include "Lexeme.h"

#include <utility> // For std::move
#include <algorithm> //For std::fill

// Конструктор по умолчанию
lexeme::lexeme() : name(), type(0), dimension(1), is_init() {}

// Конструктор с заданием имени идентификатора или значения константы
//lexeme::lexeme(string new_name)
//{
//    name = std::move(new_name);
//    type = 0;
//    is_init.push_back(false);
//    dimension = 1;
//}

// Конструктор с заданием имени идентификатора или значения константы
lexeme::lexeme(string new_name) :
    name(std::move(new_name)),
    type(0),
    dimension(1),
    is_init(1, false) {}

// Деструктор
lexeme::~lexeme() {}

