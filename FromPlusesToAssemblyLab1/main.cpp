#include "Constants.h"

// Функция установки цвета в консоли
void SetColor(int textColor, int bgColor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

int main()
{
    setlocale(LC_ALL, "");
    cout.flags(ios::fixed);
    cout.setf(ios_base::fixed);

    // Использование лямбда - функции
    auto hello { [](){ cout << "Welcome to first Assembly lab!" << endl; } };
    hello();

    try {
        ConstantTable<string> constTable;
        ConstantTable<string> reservedWordsNew;
        ConstantTable<string> identifiersNew;
        ConstantTable<string> separatorsNew;
        ConstantTable<int> constantsNew;
        string str;
        constTable.read_file(GlobalPaths::wordsPath);
        reservedWordsNew.read_file(GlobalPaths::wordPathNew);
        identifiersNew.read_file(GlobalPaths::identPathNew);
        separatorsNew.read_file(GlobalPaths::divisorPathNew);
        constantsNew.read_file(GlobalPaths::constantsPathNew);
        cout << "a.contains(\"int\") = " << constTable.contains("int") << endl;
        cout << "a.contains(\"double\") = " << constTable.contains("double") << endl;

        int num;
        constTable.get_num("return", num);
        cout << "a.get_num(\"return\", num): num = " << num << endl;

        constTable.get_num("main", num);
        cout << "a.get_num(\"main\", num): num = " << num << endl;

        constTable.get_num("void", num);
        cout << "a.get_num(\"void\", num): num = " << num << endl;

        constTable.get_num("for", num);
        cout << "a.get_num(\"for\", num): num = " << num << endl;

        constTable.printTable();

        constTable.get_val(num, str);
        cout << "a.get_val(num, str): str = " << str << endl;
    } catch (const exception& e) {
        cerr << "Exception with reading and processing current file occurred: " << e.what() << endl;
        throw e;
    }

    try {
        Lexeme lexeme;
        VariableTable hashTable;
        VariableTable identifiersNew;
        VariableTable constantsNew;
        hashTable.add("variable");
        hashTable.add("vairable");
        hashTable.add("vairalbe");
        hashTable.add("viriable");
        int hash, chain;
        hashTable.get_location("variable", hash, chain);
        cout << "b.get_location(\"variable\", hash, chain): hash = " << hash << "  chain = " << chain <<
             endl;

        hashTable.set_type("variable", 2);
        hashTable.set_dimension("variable", 3);
        hashTable.set_is_init("variable", true);
        hashTable.set_is_init("variable", false, 1);
        hashTable.set_is_init("variable", true, 2);

        hashTable.get_lexeme("variable", lexeme);

        cout << "c.name = " << lexeme.name << endl;
        cout << "c.type = " << lexeme.type << endl;
        cout << "c.is_init[0] = " << lexeme.is_init[0] << endl;
        cout << "c.is_init[1] = " << lexeme.is_init[1] << endl;
        cout << "c.is_init[2] = " << lexeme.is_init[2] << endl;
    } catch (const exception& e) {
        cerr << "Error with processing Variable Table occurred: " << e.what() << endl;
        throw e;
    }

    return 0;
}
