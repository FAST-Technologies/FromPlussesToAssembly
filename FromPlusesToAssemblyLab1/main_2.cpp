#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <list>
#include <exception>

using namespace std;

enum TokenType {
    KEYWORD = 10,
    DELIMITER = 20,
    IDENTIFIER = 30,
    CONSTANT = 40
};

struct Token {
    int type;
    int tableIndex;
    int hashIndex;
};

struct Identifier {
    string name;
    string type;
    vector<string> params;
    explicit Identifier(string n, string t = "unknown") : name(std::move(n)), type(std::move(t)) {}
    bool operator==(const Identifier& other) const { return name == other.name; }
};

struct Constant {
    int value;
    string type;
    explicit Constant(int v, string t = "int") : value(v), type(std::move(t)) {}
    bool operator==(const Constant& other) const { return value == other.value; }
};


template<typename T>
class Table {
public:
    virtual bool find(const T& elem) = 0;
    virtual T get(int index) = 0;
    virtual void add(const T& elem) = 0;
    virtual int size() = 0;
    virtual ~Table() = default;
};

class ConstTable : public Table<string> {
private:
    vector<string> data;
public:
    explicit ConstTable(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Warning: Could not open file " << filename << endl;
            return;
        }
        string entry;
        while (file >> entry) {
            data.push_back(entry);
        }
        file.close();
        sort(data.begin(), data.end());
    }

    bool find(const string& elem) override { return binary_search(data.begin(), data.end(), elem); }

    string get(int index) override {
        if (index >= 0 && index < data.size()) {
            return data[index];
        }
        throw out_of_range("Index out of bounds");
    }

    void add(const string& elem) override { throw runtime_error("Cannot add to constant table"); }

    int size() override { return static_cast<int>(data.size()); }

    int getIndex(const string& elem) {
        auto it = lower_bound(data.begin(), data.end(), elem);
        if (it != data.end() && *it == elem) {
            return static_cast<int>(distance(data.begin(), it));
        }
        return -1;
    }
};

template<typename T>
class HashTable : public Table<T> {
private:
    vector<list<pair<T, int>>> data;
    int capacity;
    int totalSize;

    int getHash(const T& elem) {
        if constexpr (is_same_v<T, string>) {
            return hash<string>{}(elem) % capacity;
        }
        else if constexpr (is_same_v<T, Constant>) {
            return elem.value % capacity;
        }
        else if constexpr (is_same_v<T, Identifier>) {
            return hash<string>{}(elem.name) % capacity;
        }
        static_assert(is_same_v<T, string> || is_same_v<T, Constant> || is_same_v<T, Identifier>,
                      "HashTable supports only string, Constant, and Identifier types");
        return 0;
    }

    void resize() {
        capacity *= 2;
        vector<list<pair<T, int>>> newData(capacity);
        int newIndex = 0;
        for (const auto& chain : data) {
            for (const auto& item : chain) {
                int idx = getHash(item.first);
                newData[idx].push_back({ item.first, newIndex++ });
            }
        }
        data = std::move(newData);
    }

public:
    explicit HashTable(int initialCapacity = 10) : capacity(initialCapacity), totalSize(0) {
        data.resize(capacity);
    }

    explicit HashTable(const string& filename, int initialCapacity = 10)
            : capacity(initialCapacity), totalSize(0) {
        data.resize(capacity);
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Warning: Could not open file " << filename << endl;
            return;
        }
        if constexpr (is_same_v<T, string>) {
            string entry;
            while (file >> entry) {
                add(entry);
            }
        }
        else if constexpr (is_same_v<T, Constant>) {
            int value;
            while (file >> value) {
                add(Constant(value));
            }
        }
        else if constexpr (is_same_v<T, Identifier>) {
            string name;
            while (file >> name) {
                add(Identifier(name));
            }
        }
        file.close();
    }

    bool find(const T& elem) override {
        int index = getHash(elem);
        for (const auto& item : data[index]) {
            if (item.first == elem) return true;
        }
        return false;
    }

    T get(int index) override {
        for (const auto& chain : data) {
            for (const auto& item : chain) {
                if (item.second == index) return item.first;
            }
        }
        throw out_of_range("Element not found");
    }

    void add(const T& elem) override {
        int index = getHash(elem);
        if (!find(elem)) {
            data[index].push_back({ elem, totalSize++ });
            if (size() > capacity * 0.75) {
                resize();
            }
        }
    }

    int size() override { return totalSize; }

    pair<int, int> getPosition(const T& elem) {
        int index = getHash(elem);
        int pos = 0;
        for (const auto& item : data[index]) {
            if (item.first == elem) {
                return { item.second, pos };
            }
            pos++;
        }
        return { -1, -1 };
    }

    void setAttribute(int index, const string& type, const vector<string>& params = {}) {
        for (auto& chain : data) {
            for (auto& item : chain) {
                if (item.second == index) {
                    if constexpr (is_same_v<T, Identifier>) {
                        item.first.type = type;
                        item.first.params = params;
                    }
                    return;
                }
            }
        }
    }
};

class Scanner {
private:
    ConstTable keywords;
    ConstTable delimiters;
    HashTable<Identifier> identifiers;
    HashTable<Constant> constants;
    ofstream tokenFile;

public:
    Scanner() :
            keywords("txtFiles1/keywords.txt"),
            delimiters("txtFiles1/delimiters.txt"),
            identifiers("txtFiles1/identifiers.txt"),
            constants("txtFiles1/constants.txt"),
            tokenFile("txtFiles1/tokens.txt") {
        if (!tokenFile.is_open()) {
            cerr << "Error: Could not open tokens.txt" << endl;
        }
    }

    ~Scanner() {
        if (tokenFile.is_open()) {
            tokenFile.close();
        }
    }

    Token analyzeToken(const string& token) {
        int keywordIndex = keywords.getIndex(token);
        if (keywordIndex != -1) {
            saveToken({ KEYWORD, keywordIndex, -1 });
            return { KEYWORD, keywordIndex, -1 };
        }
        int delimiterIndex = delimiters.getIndex(token);
        if (delimiterIndex != -1) {
            saveToken({ DELIMITER, delimiterIndex, -1 });
            return { DELIMITER, delimiterIndex, -1 };
        }
        if (isdigit(token[0])) {
            int value = stoi(token);
            constants.add(Constant(value));
            auto pos = constants.getPosition(Constant(value));
            saveToken({ CONSTANT, pos.first, pos.second });
            return { CONSTANT, pos.first, pos.second };
        }
        identifiers.add(Identifier(token));
        auto pos = identifiers.getPosition(Identifier(token));
        saveToken({ IDENTIFIER, pos.first, pos.second });
        return { IDENTIFIER, pos.first, pos.second };
    }

    void processCode(const string& code) {
        string token;
        bool inFunction = false;
        vector<string> params;

        for (char ch : code) {
            if (isalnum(ch) || ch == '_') {
                token += ch;
            }
            else {
                if (!token.empty()) {
                    Token t = analyzeToken(token);
                    printToken(t, token);
                    if (t.type == KEYWORD && token == "int" && !inFunction) {
                        inFunction = true;
                    }
                    else if (inFunction && t.type == IDENTIFIER) {
                        if (ch == '(') {
                            identifiers.setAttribute(t.tableIndex, "function", params);
                            params.clear();
                        }
                        else if (ch != ')') {
                            params.push_back(token);
                        }
                    }
                    token.clear();
                }
                if (!isspace(ch)) {
                    string delim(1, ch);
                    Token t = analyzeToken(delim);
                    printToken(t, delim);
                    if (delim == ")") inFunction = false;
                }
            }
        }
        if (!token.empty()) {
            Token t = analyzeToken(token);
            printToken(t, token);
        }
    }

    void saveToken(const Token& t) {
        if (tokenFile.is_open()) {
            tokenFile << t.type << " " << t.tableIndex << " " << t.hashIndex << endl;
        }
    }

    static void printToken(const Token& t, const string& value) {
        cout << "Token: " << value << " Type: " << t.type;
        if (t.tableIndex >= 0) {
            cout << " Table Index: " << t.tableIndex;
        }
        if (t.hashIndex >= 0) {
            cout << " Hash Index: " << t.hashIndex;
        }
        cout << endl;
    }

    void printTableContents() {
        cout << "Keywords:" << endl;
        for (int i = 0; i < keywords.size(); i++) {
            cout << keywords.get(i) << " ";
        }
        cout << "\nDelimiters:" << endl;
        for (int i = 0; i < delimiters.size(); i++) {
            cout << delimiters.get(i) << " ";
        }
        cout << "\nIdentifiers:" << endl;
        for (int i = 0; i < identifiers.size(); i++) {
            Identifier id = identifiers.get(i);
            cout << id.name << " (" << id.type;
            if (!id.params.empty()) {
                cout << ", params: ";
                for (const auto& p : id.params) cout << p << " ";
            }
            cout << ") ";
        }
        cout << "\nConstants:" << endl;
        for (int i = 0; i < constants.size(); i++) {
            Constant c = constants.get(i);
            cout << c.value << " (" << c.type << ") ";
        }
        cout << endl;
    }
};

int main(int argc, char* argv[]) {
    Scanner scanner;
    if (argc != 2) {
        cerr << "Usage: program_name <cpp_file.cpp>" << endl;
        return 1;
    }
    string filename = argv[1];
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    stringstream buffer;
    buffer << inputFile.rdbuf();
    string code = buffer.str();
    inputFile.close();
    cout << code << endl;
    cout << "Processing code: " << code << endl << endl;
    scanner.processCode(code);
    cout << "\nTable contents after processing:" << endl;
    scanner.printTableContents();
    return 0;
}
