#include "Constants.h"

int main()
{
    try {
        ConstantTable<string> constTable;
        string str;
        constTable.read_file(GlobalPaths::wordsPath);
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
    }

    try {
        Lexeme lexeme;
        VariableTable hashTable;
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
    }



    return 0;
}
