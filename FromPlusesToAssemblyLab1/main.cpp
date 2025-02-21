#include <iostream>
#include <cstdio>
#include "ConstantTable/ConstantTable.h"
#include "VariableTable/VariableTable.h"
#include "Lexeme/Lexeme.h"

using namespace std;

int main()
{
    ConstantTable<string> a;
    a.read_file("txtFiles/reserved_words.txt");
    cout << "a.contains(\"int\") = " << a.contains("int") << endl;
    cout << "a.contains(\"double\") = " << a.contains("double") << endl;

    int num;
    a.get_num("return", num);
    cout << "a.get_num(\"return\", num): num = " << num << endl;

    a.get_num("main", num);
    cout << "a.get_num(\"main\", num): num = " << num << endl;

    a.get_num("void", num);
    cout << "a.get_num(\"void\", num): num = " << num << endl;

    a.get_num("for", num);
    cout << "a.get_num(\"for\", num): num = " << num << endl;

    string str;
    a.get_val(num, str);
    cout << "a.get_val(num, str): str = " << str << endl;

    VariableTable b;
    b.add("variable");
    b.add("vairable");
    b.add("vairalbe");
    b.add("variable");
    int hash, chain;
    b.get_location("variable", hash, chain);
    cout << "b.get_location(\"variable\", hash, chain): hash = " << hash << "  chain = " << chain <<
         endl;

    b.set_type("variable", 2);
    b.set_dimension("variable", 3);
    b.set_is_init("variable", true);
    b.set_is_init("variable", false, 1);
    b.set_is_init("variable", true, 2);
    lexeme c;
    b.get_lexeme("variable", c);

    cout << "c.name = " << c.name << endl;
    cout << "c.type = " << c.type << endl;
    cout << "c.is_init[0] = " << c.is_init[0] << endl;
    cout << "c.is_init[1] = " << c.is_init[1] << endl;
    cout << "c.is_init[2] = " << c.is_init[2] << endl;

    return 0;
}


//int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
//}
