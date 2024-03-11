#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <cctype>
#include <cmath>
#include <stdexcept>

// Функция для определения приоритета операции
int getOperatorPriority(char op) {
    switch (op) {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    default:
        throw std::invalid_argument("Unsupported operation");
    }
}

// Функция для выполнения операции
double applyOperation(char operation, double operand1, double operand2) {
    switch (operation) {
    case '+': return operand1 + operand2;
    case '-': return operand1 - operand2;
    case '*': return operand1 * operand2;
    case '/':
        if (operand2 == 0) {
            throw std::invalid_argument("Division by zero");
        }
        return operand1 / operand2;
    default:
        throw std::invalid_argument("Unsupported operation");
    }
}

// Функция для вычисления выражения
double calculate(const std::string& expression) {
    std::stack<double> operands;
    std::stack<char> operators;
    std::stringstream ss(expression);
    std::string number;

    auto processOperator = [&](char op) {
        while (!operators.empty() && operators.top() != '(' &&
            getOperatorPriority(op) <= getOperatorPriority(operators.top())) {
            double operand2 = operands.top();
            operands.pop();
            double operand1 = operands.top();
            operands.pop();
            char operation = operators.top();
            operators.pop();
            double result = applyOperation(operation, operand1, operand2);
            operands.push(result);
        }
        operators.push(op);
        };

    auto processNumber = [&](const std::string& numStr) {
        double num = std::stod(numStr);
        operands.push(num);
        };

    auto processClosedBracket = [&]() {
        while (!operators.empty() && operators.top() != '(') {
            double operand2 = operands.top();
            operands.pop();
            double operand1 = operands.top();
            operands.pop();
            char operation = operators.top();
            operators.pop();
            double result = applyOperation(operation, operand1, operand2);
            operands.push(result);
        }
        if (!operators.empty() && operators.top() == '(') {
            operators.pop();
        }
        else {
            throw std::invalid_argument("Mismatched parentheses");
        }
        };

    char c;
    while (ss >> c) {
        if (std::isdigit(c) || c == '.') {
            number += c;
        }
        else {
            if (!number.empty()) {
                processNumber(number);
                number.clear();
            }
            if (c == '(') {
                operators.push(c);
            }
            else if (c == ')') {
                processClosedBracket();
            }
            else if (std::string("+-*/").find(c) != std::string::npos) {
                processOperator(c);
            }
            else {
                throw std::invalid_argument("Invalid character: " + std::string(1, c));
            }
        }
    }

    if (!number.empty()) {
        processNumber(number);
    }

    while (!operators.empty()) {
        double operand2 = operands.top();
        operands.pop();
        double operand1 = operands.top();
        operands.pop();
        char operation = operators.top();
        operators.pop();
        double result = applyOperation(operation, operand1, operand2);
        operands.push(result);
    }

    if (operands.size() != 1) {
        throw std::invalid_argument("Invalid expression");
    }

    return operands.top();
}

int main() {
    std::string expression;
    std::cout << "Enter an expression (e.g., 5+5*10): ";
    std::getline(std::cin, expression);

    try {
        double result = calculate(expression);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}