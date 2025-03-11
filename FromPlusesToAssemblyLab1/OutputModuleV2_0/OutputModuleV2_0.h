#ifndef FROMPLUSESTOASSEMBLYLAB1_OUTPUTMODULEV2_0_H
#define FROMPLUSESTOASSEMBLYLAB1_OUTPUTMODULEV2_0_H

#include "../ConstantTableV2_0/ConstantTableV2_0.h"
#include <sstream>

using namespace std;

/// <summary>
/// [EN] Function for installing color in console
/// [RU] Функция для установки цвета в консоли
/// </summary>
void SetColor(int textColor, int bgColor);

/// <summary>
/// [EN] Output function for constant table
/// [RU] Функция вывода для упорядоченной таблицы
/// </summary>
void outputConstantTableEntry(int num, const ConstantTableEntry& entry, ofstream& logger);

/// <summary>
/// [EN] Help function to check dot
/// [RU] Вспомогательная функция проверки на точку
/// </summary>
bool containsDot(const string& text);


#endif //FROMPLUSESTOASSEMBLYLAB1_OUTPUTMODULEV2_0_H
