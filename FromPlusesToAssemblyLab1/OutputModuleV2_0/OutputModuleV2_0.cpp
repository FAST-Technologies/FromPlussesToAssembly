#include "OutputModuleV2_0.h"

/// <summary>
/// [EN] Function for installing color in console
/// [RU] Функция для установки цвета в консоли
/// </summary>
void SetColor(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

/// <summary>
/// [EN] Output function for constant table
/// [RU] Функция вывода для упорядоченной таблицы
/// </summary>
void outputConstantTableEntry(int num, const ConstantTableEntry& entry, ofstream& logger)
{
    stringstream output;
    output << num << " : ";
    switch (entry.type) {
        case LanguageElementType::ReservedWord:
            output << "ReservedWord: ";
            break;
        case LanguageElementType::OperatorSign:
            output << "Operator: ";
            break;
        case LanguageElementType::Delimiter:
            output << "Separator: ";
            break;
        default:
            output << "Other: ";
            break;
    }
    output << entry.symbol << " Type: " << static_cast<int>(entry.type)
           << " Lexeme Code: " << entry.lexemeCode
           << " Lexeme Type Size: " << entry.lexemeTypeSize << " (";
    if (entry.lexemeTypeSize == 1) {
        output << "char)";
    } else {
        output << "string)";
    }
    string message = output.str();
    logger << message << endl;
}

/// <summary>
/// [EN] Help function to check dot
/// [RU] Вспомогательная функция проверки на точку
/// </summary>
bool containsDot(const string& text)
{
    return text.find('.') != string::npos;
}
