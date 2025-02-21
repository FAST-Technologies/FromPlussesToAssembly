#ifndef FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLE_H
#define FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLE_H


//class VariableTable {
//
//};
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <memory> // For std::unique_ptr
#include "../Lexeme/Lexeme.h"

using namespace std;

// Размер хэш-таблицы по умолчанию
//#define default_hashnum 100
static const int default_hashnum = 100;
// Класс переменных таблиц
class VariableTable
{
private:
    // Размер таблицы
    int hashnum;
    // Указатель на массив цепочек
    vector<lexeme> *table;
    // Подсчет хэша
    [[nodiscard]] int get_hash(const string& name) const;
    // Подсчет номера в цепочке
    [[nodiscard]] int get_chain(const string& name) const;
public:
    // Конструктор с размером таблицы по умолчанию
    VariableTable();
    // Конструктор с пользовательским размером таблицы
    explicit VariableTable(int new_hashnum);
    // Деструктор
    ~VariableTable();
    // Определение хэша и номера в цепочке
    bool get_location(const string& name, int &hash, int &chain);
    // Проверка есть ли элемент в таблице
    //inline bool contains(const string& name);
    [[nodiscard]] bool contains(const std::string& name) const;
    // Добавление нового имени идентификатора или значения константы
    bool add(const string& name);
    // Задание типа по хэшу и номеру в цепочке
    bool set_type(int hash, int chain, int type);
    // Задание типа по имени идентификатора или значению константы
    bool set_type(const string& name, int type);
    // Задание размерности по хэшу и номеру в цепочке
    bool set_dimension(int hash, int chain, int dimension);
    // Задание размерности по имени идентификатора или значению константы
    bool set_dimension(const string& name, int dimension);
    // Задание флага инициализации по хэшу и номеру в цепочке
    bool set_is_init(int hash, int chain, bool is_init);
    // Задание флага инициализации по имени идентификатора или значению константы
    bool set_is_init(const string& name, bool is_init);
    // Задание флага инициализации для массивов по хэшу и номеру в цепочке
    bool set_is_init(int hash, int chain, bool is_init, int init_index);
    // Задание флага инициализации для массивов по имени идентификатора или значению константы
    bool set_is_init(const string& name, bool is_init, int init_index);
    // Получение структуры lexeme по хэшу и номеру в цепочке
    bool get_lexeme(int hash, int chain, lexeme &lexeme);
    // Получение структуры lexeme по имени идентификатора или значению константы
    bool get_lexeme(const string& name, lexeme &lexeme);
};




#endif //FROMPLUSESTOASSEMBLYLAB1_VARIABLETABLE_H
