#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <unordered_map>
#include <forward_list>
#include <deque>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <tuple>
#include <optional>
#include <variant>

using namespace std;

int sad(int& num, const string& name) {
    cout << num << name << endl;
    return num;
}

const double ar = 1.23455;

int main() {
    int age = 30;
    int& myIntRef = age;
    float height = 1.75;
    std::string name = "Alice";
    string test = "flaf";
    std::wstring st = L"SomeText";
    wstring ter = L"sad";
    bool is_student = true;
    char initial = 'A';
    double pi = 3.14159;
    double& yes = pi;
    std::vector<int> myVector = {1, 2, 3};
    std::array<float, 5> myArray = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    std::list<std::string> myList = {"a", "b", "c"};
    std::unordered_map<std::string, int> myMap = {{"a", 1}, {"b", 2}};
    vector<int> myVector1 = {1, 2, 3};
    array<float, 5> myArray1 = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    list<std::string> myList1 = {"a", "b", "c"};
    list<int> myList2 = {1, 2, 3};
    const list<int> myConstList = {4, 5, 6};
    list<int>& myListRef = myList2;
    const list<int>& myConstListRef = myConstList;
    unordered_map<std::string, int> myMap1 = {{"a", 1}, {"b", 2}};
    std::forward_list<int> myForwardList = {9, 10, 11};
    forward_list<int> myForwardList1 = {9, 10, 11};
    std::deque<double> myDeque = {1.1, 2.2, 3.3};
    deque<double> myDeque1 = {1.1, 2.2, 3.3};

    std::queue<int> myQueue;
    myQueue.push(12);
    myQueue.push(13);
    queue<int> myQueue2;
    myQueue2.push(12);
    myQueue2.push(13);

    std::priority_queue<int> myPriorityQueue;
    myPriorityQueue.push(14);
    myPriorityQueue.push(15);
    priority_queue<int> myPriorityQueue2;
    myPriorityQueue2.push(14);
    myPriorityQueue2.push(15);

    std::stack<char> myStack;
    myStack.push('B');
    myStack.push('C');
    stack<char> myStack2;
    myStack2.push('B');
    myStack2.push('C');

    std::map<string, int> myMap3 = {{"one", 1}, {"two", 2}, {"three", 3}};
    map<string, int> myMap4 = {{"one", 1}, {"two", 2}, {"three", 3}};
    std::set<int> mySet = {16, 17, 18};
    set<int> mySet2 = {16, 17, 18};
    std::unordered_set<string> myUnorderedSet = {"grape", "kiwi", "lemon"};
    unordered_set<string> myUnorderedSet2 = {"grape", "kiwi", "lemon"};
    std::multimap<int, string> myMultimap = {{1, "a"}, {1, "b"}, {2, "c"}};
    multimap<int, string> myMultimap2 = {{1, "a"}, {1, "b"}, {2, "c"}};
    std::multiset<int> myMultiset = {19, 20, 19};
    multiset<int> myMultiset2 = {19, 20, 19};

    std::bitset<8> myBitset(128); // 10000000
    bitset<8> myBitset2(128); // 10000000

    std::pair<int, string> myPair(21, "orange");
    pair<int, string> myPair2(21, "orange");
    std::tuple<int, double, string> myTuple(22, 4.4, "mango");
    tuple<int, double, string> myTuple2(22, 4.4, "mango");

    std::optional<int> myOptional = 23;
    std::optional<int> myEmptyOptional;
    optional<int> myOptional2 = 23;
    optional<int> myEmptyOptional2;

    std::variant<int, string> myVariantInt = 24;
    std::variant<int, string> myVariantString = "pineapple";
    variant<int, string> myVariantInt2 = 24;
    variant<int, string> myVariantString2 = "pineapple";
    variant<int, string>& myVariantString3 = myVariantString2;
    return 0;
}