#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>

struct Stack {
    int data[100];
    int top;
    
    Stack() : top(-1) {}
    
    void push(int x) {
        data[++top] = x;
    }
    
    int pop() {
        return data[top--];
    }
    
    bool empty() const {
        return top == -1;
    }
};

int roman_to_int(const char* s) {
    int result = 0;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == 'M') {
            result += 1000;
        } else if (s[i] == 'D') {
            result += 500;
        } else if (s[i] == 'C') {
            if (s[i+1] == 'M') {
                result += 900;
                i++;
            } else if (s[i+1] == 'D') {
                result += 400;
                i++;
            } else {
                result += 100;
            }
        } else if (s[i] == 'L') {
            result += 50;
        } else if (s[i] == 'X') {
            if (s[i+1] == 'C') {
                result += 90;
                i++;
            } else if (s[i+1] == 'L') {
                result += 40;
                i++;
            } else {
                result += 10;
            }
        } else if (s[i] == 'V') {
            result += 5;
        } else if (s[i] == 'I') {
            if (s[i+1] == 'X') {
                result += 9;
                i++;
            } else if (s[i+1] == 'V') {
                result += 4;
                i++;
            } else {
                result += 1;
            }
        }
        i++;
    }
    return result;
}

void int_to_roman(int num, char* result) {
    if (num <= 0 || num >= 4000) {
        strncpy(result, "ERR", 4);
        result[3] = '\0';
        return;
    }
    
    const int values[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
    const char* symbols[] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
    
    result[0] = '\0';

    for (int i = 0; i < 13; i++) {
        while (num >= values[i]) {
            strncat(result, symbols[i], 49 - strlen(result));
            num -= values[i];
        }
    }
}

bool is_operator(const char* s) {
    return (strcmp(s, "+") == 0 || strcmp(s, "-") == 0 || 
            strcmp(s, "*") == 0 || strcmp(s, "/") == 0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);

    if (!input.is_open() || !output.is_open()) {
        return 1;
    }

    char c;
    while (input.get(c)) {
        char line_buf[1000];
        int len = 0;
        bool has_newline = false;

        if (c == '\n') {
            has_newline = true;
        } else {
            line_buf[len++] = c;
            while (input.get(c)) {
                if (c == '\n') {
                    has_newline = true;
                    break;
                }
                if (len < 999) {
                    line_buf[len++] = c;
                }
            }
        }
        line_buf[len] = '\0';

        if (len == 0 && !has_newline) {
            continue;
        }

        char line_copy[1000];
        strncpy(line_copy, line_buf, sizeof(line_copy) - 1);
        line_copy[sizeof(line_copy) - 1] = '\0';

        char* tokens[200];
        int cnt = 0;
        char* token = strtok(line_copy, " ");
        while (token != nullptr && cnt < 200) {
            tokens[cnt++] = token;
            token = strtok(nullptr, " ");
        }

        Stack st;
        bool error = false;

        for (int i = 0; i < cnt; i++) {
            if (is_operator(tokens[i])) {
                if (st.top < 1) {
                    error = true;
                    break;
                }
                int b = st.pop();
                int a = st.pop();
                int res;
                if (strcmp(tokens[i], "+") == 0) {
                    res = a + b;
                } else if (strcmp(tokens[i], "-") == 0) {
                    res = a - b;
                } else if (strcmp(tokens[i], "*") == 0) {
                    res = a * b;
                } else {
                    if (b == 0) {
                        error = true;
                        break;
                    }
                    res = a / b;
                }

                if (res < 1 || res > 3999) {
                    error = true;
                    break;
                }
                st.push(res);
            } else {
                int val = roman_to_int(tokens[i]);
                if (val < 1 || val > 3999) {
                    error = true;
                    break;
                }
                st.push(val);
            }
        }

        output << line_buf << " = ";
        if (error || st.top != 0) {
            output << "ERR";
        } else {
            char roman_result[50];
            int_to_roman(st.pop(), roman_result);
            output << roman_result;
        }

        if (has_newline) {
            output << '\n';
        }
    }

    return 0;
}
