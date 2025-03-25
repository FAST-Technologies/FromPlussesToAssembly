#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

using namespace std;

class DFA {
private:
    int num_states;          // Количество состояний
    vector<char> alphabet; // Количество символов алфавита
    int start_state;         // Начальное состояние
    vector<int> accepting_states; // Множество принимающих состояний
    map<pair<int, char>, int> transition_function; // Функция переходов

public:
    // Конструктор
    DFA(int num_states, const vector<char>& alphabet, int start_state,
        const vector<int>& accepting_states,
        const map<pair<int, char>, int>& transition_function)
            : num_states(num_states),
              alphabet(alphabet),
              start_state(start_state),
              accepting_states(accepting_states),
              transition_function(transition_function) {

        // Проверка входных данных (минимум):
        if (start_state < 0 || start_state >= num_states) {
            throw invalid_argument("Invalid start state.");
        }
        for (int state : accepting_states) {
            if (state < 0 || state >= num_states) {
                throw invalid_argument("Invalid accepting state.");
            }
        }
    }

    // Метод для проверки, принимает ли автомат заданную строку
    bool accepts(const string& input) const {
        int current_state = start_state;

        for (char symbol : input) {
            // Проверяем, есть ли символ в алфавите (простое предположение)
            // В реальном коде может потребоваться более сложная проверка
//            if (symbol < 32 || symbol > (32 + num_alphabet_symbols)) { // Assuming ASCII range for alphabet symbols
//                std::cout << "Symbol is: " << symbol << std::endl;
//                throw std::invalid_argument("Invalid input symbol.");
//            }
//            if (symbol != '0' && symbol != '1') {
//                std::cout << "Symbol is: " << symbol << std::endl;
//                throw std::invalid_argument("Invalid input symbol.");
//            }
            bool valid_symbol = false;
            for (char alpha_symbol : alphabet) {
                if (symbol == alpha_symbol) {
                    valid_symbol = true;
                    break;
                }
            }
            if (!valid_symbol) {
                cout << "Symbol is: " << symbol << endl;
                throw invalid_argument("Invalid input symbol.");
            }

            // Находим следующее состояние на основе текущего состояния и символа
            auto transition = transition_function.find({current_state, symbol});
            if (transition == transition_function.end()) {
                // Переход не определен, автомат отвергает строку
                return false;
            }
            current_state = transition->second;
        }

        // Проверяем, является ли конечное состояние принимающим
        for (int accepting_state : accepting_states) {
            if (current_state == accepting_state) {
                return true;
            }
        }

        return false;
    }

    // Метод для вывода информации об автомате (для отладки)
    void print_info() const {
        cout << "Number of states: " << num_states << endl;
        cout << "Number of alphabet symbols: ";
        for (char state : alphabet) {
            cout << state << " ";
        }
        cout << endl;
        cout << "Start state: " << start_state << endl;

        cout << "Accepting states: ";
        for (int state : accepting_states) {
            cout << state << " ";
        }
        cout << endl;

        cout << "Transition function:" << endl;
        for (const auto& entry : transition_function) {
            cout << "  (" << entry.first.first << ", " << entry.first.second << ") -> " << entry.second << endl;
        }
    }
};

int main() {
    try {
        // Пример ДКА, принимающего строки, состоящие из четного числа '0' и '1'
        // Состояния: 0 (четное число нулей и единиц), 1 (нечетное число нулей, четное число единиц),
        //            2 (четное число нулей, нечетное число единиц), 3 (нечетное число нулей и единиц)
        // Алфавит: '0', '1'
        // Начальное состояние: 0
        // Принимающие состояния: 0

        map<pair<int, char>, int> transition_function = {
                {{0, '0'}, 1}, {{0, '1'}, 2},
                {{1, '0'}, 0}, {{1, '1'}, 3},
                {{2, '0'}, 3}, {{2, '1'}, 0},
                {{3, '0'}, 2}, {{3, '1'}, 1}
        };
        vector<int> accepting_states = {0};
        vector<char> alphabet = {'0', '1'};
        DFA dfa(4, alphabet, 0, accepting_states, transition_function); // Number of alphabet symbols is 2 since our alphabet consists of '0' and '1'.

        dfa.print_info(); // Output information about DFA

        cout << "Accepts \"00\": " << dfa.accepts("00") << endl;       // true
        cout << "Accepts \"11\": " << dfa.accepts("11") << endl;       // true
        cout << "Accepts \"0101\": " << dfa.accepts("0101") << endl;   // true
        cout << "Accepts \"0\": " << dfa.accepts("0") << endl;         // false
        cout << "Accepts \"1\": " << dfa.accepts("1") << endl;         // false
        cout << "Accepts \"01\": " << dfa.accepts("01") << endl;       // false
        cout << "Accepts \"001\": " << dfa.accepts("001") << endl;     // false
        cout << "Accepts \"\": " << dfa.accepts("") << endl;         // true (пустая строка также принимается)
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }

    return 0;
}
