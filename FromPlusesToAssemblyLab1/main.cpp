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
        SetColor(14,0);
        cerr << "Exception with reading and processing current file occurred: " << e.what() << endl;
        throw e;
    }

    try {
        Lexeme lexeme, lexeme1;
        VariableTable hashTable;
        VariableTable identifiersNew;
        VariableTable constantsNew;

        if (constantsNew.loadFromFile(GlobalPaths::constantsPathNew)) {
            cout << "Successfully loaded from file!" << endl;
        } else {
            SetColor(14,0);
            cerr << "Failed to load from file." << endl;
        }
        SetColor(7,0);

        // Now you can test other methods like containsLexeme, getAttribute, etc.
        cout << "Contains '100': " << constantsNew.contains("100") << endl;
        cout << "Contains 'abc': " << constantsNew.contains("abc") << endl;
        cout << "Contains '200': " << constantsNew.contains("200") << endl;

        hashTable.add("variable", 10);
        hashTable.add("vairable", 20);
        hashTable.add("vairalbe", 10);
        hashTable.add("viriable", 20);
        int hash, chain;
        hashTable.get_location("variable", hash, chain);
        cout << "b.get_location(\"variable\", hash, chain): hash = " << hash << "  chain = " << chain <<
             endl;

        hashTable.set_type("variable", 2);
        hashTable.set_lexeme_code("variable", 10);
        hashTable.set_dimension("variable", 3);
        hashTable.set_is_init("variable", true);
        hashTable.set_is_init("variable", false, 1);
        hashTable.set_is_init("variable", true, 2);

        hashTable.get_lexeme("variable", lexeme);
        SetColor(15,0);
        cout << "c.name = " << lexeme.name << endl;
        cout << "c.type = " << lexeme.type << endl;
        cout << "c.lexemeCode = " << lexeme.lexemeCode << endl;
        cout << "c.is_init[0] = " << lexeme.is_init[0] << endl;
        cout << "c.is_init[1] = " << lexeme.is_init[1] << endl;
        cout << "c.is_init[2] = " << lexeme.is_init[2] << endl;

        lexeme.print();

        hashTable.set_type("vairable", 3);
        hashTable.set_lexeme_code("vairable", 20);
        hashTable.set_dimension("vairable", 2);
        hashTable.set_is_init("vairable", true);
        hashTable.set_is_init("vairable", false, 1);
        hashTable.set_is_init("vairable", true, 2);

        hashTable.get_lexeme("vairable", lexeme1);

        cout << "c.name = " << lexeme1.name << endl;
        cout << "c.type = " << lexeme1.type << endl;
        cout << "c.lexemeCode = " << lexeme1.lexemeCode << endl;
        cout << "c.is_init[0] = " << lexeme1.is_init[0] << endl;
        cout << "c.is_init[1] = " << lexeme1.is_init[1] << endl;
        cout << "c.is_init[2] = " << lexeme1.is_init[2] << endl;

        lexeme1.print();
    } catch (const exception& e) {
        cerr << "Error with processing Variable Table occurred: " << e.what() << endl;
        throw e;
    }

    return 0;
}
