#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool LessThanOrEqualPrecedence(char op, char stacktop);
bool isValidInfix(string infix);
void infixToPostfix(string infix, string& postfix);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
    if (! isValidInfix(infix))  // If the infix is not a syntactically valid infix expression, return 1
        return 1;
    
    infixToPostfix(infix, postfix); // Infix to postfix conversion
    //cerr << postfix << endl;
    
    // Check that all of infix's lowercase letters are contained in the map
    for (int k = 0; k < postfix.size(); k++)
    {
        char ch;
        if (islower(postfix[k]))
            ch = postfix[k];
        
        if (! values.contains(ch))
            return 2;
    }
    
    // Evaluate the postfix expression
    stack<int> operandStack;
    int op1;
    int op2;
    
    for (int k = 0; k < postfix.size(); k++)
    {
        char ch = postfix[k];
        
        if (islower(ch))
        {
            int mappedValue = 0;
            values.get(ch, mappedValue);
            operandStack.push(mappedValue);
        }
        else
        {
            op2 = operandStack.top();
            operandStack.pop();
            
            op1 = operandStack.top();
            operandStack.pop();
            
            int value = 0;
            
            switch(postfix[k])
            {
                case '+':
                    value = op1 + op2;
                    operandStack.push(value);
                    break;
                    
                case '-':
                    value = op1 - op2;
                    operandStack.push(value);
                    break;
                
                case '*':
                    value = op1 * op2;
                    operandStack.push(value);
                    break;
                    
                case '/':
                    if (op2 == 0)
                        return 3;
                    else
                    {
                        value = op1 / op2;
                        operandStack.push(value);
                        break;
                    }
            }
        }
    }
    
    result = operandStack.top();
    
    return 0;
}
// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.

void infixToPostfix(string infix, string& postfix)
{
    postfix = "";   // Initialize postfix to empty
    stack<char> opstack;    // Initialize the operator stack to empty
    
    for (int k = 0; k < infix.size(); k++)
    {
        if (islower(infix[k]))
            postfix += infix[k];
        
        else if (infix[k] == '(')
            opstack.push(infix[k]);
        
        else if (infix[k] == ')')
        {
            while (opstack.top() != '(')
            {
                postfix += opstack.top();
                opstack.pop();
            }
            opstack.pop();
        }
        
        else if (infix[k] == '+' || infix[k] == '-' || infix[k] == '*' || infix[k] == '/')
        {
            while (! opstack.empty() && opstack.top() != '('
                   && LessThanOrEqualPrecedence(infix[k], opstack.top()) )
            {
                postfix += opstack.top();
                opstack.pop();
            }
            
            opstack.push(infix[k]);
        }
    }
    
    while (! opstack.empty())
    {
        postfix += opstack.top();
        opstack.pop();
    }
}

bool isValidInfix(string infix)
{
    if (infix == "")    // If infix is empty we return false;
        return false;
    
    int numOperands = 0;
    int numOperators = 0;
    int openParen = 0;
    int closeParen = 0;
    
    for (int k = 0; k < infix.size(); k++)
    {
        if (isupper(infix[k]))      // If infix contains operands that aren't lowercase letters, return false
            return false;
        
        else if (islower(infix[k])) // If the character is a lower case letter, then we increment numOperands
            numOperands++;
        
        else if (infix[k] == '+' || infix[k] == '-' || infix[k] == '*' || infix[k] == '/')   // If character is valid binary operator, then increment numOperators
            numOperators++;
        
        else if (infix[k] == ' ')   // If the character is a space, we ignore it
            continue;
        
        else if (infix[k] == '(')
            openParen++;
        
        else if (infix[k] == ')')
            closeParen++;
        
        else
            return false;
    }
    
    if (numOperands - numOperators != 1)    // The number of operands to operators should always be 1 for a syntactically valid infix expression
        return false;
    if (openParen != closeParen)        // Open and close parentheses must always match
        return false;
    
    return true;
}

bool LessThanOrEqualPrecedence(char op, char stacktop)
{
    int opPrecedence = 0;
    int stacktopPrecedence = 0;
    
    if (op == '*' || op == '/')
        opPrecedence = 2;
    else if (op == '+' || op == '-')
        opPrecedence = 1;
    
    if (stacktop == '*' || stacktop == '/')
        stacktopPrecedence = 2;
    else if (stacktop == '+' || stacktop == '-')
        stacktopPrecedence = 1;
    
    return (opPrecedence <= stacktopPrecedence);
}


int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
           pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
    // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
           pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
           pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
           pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
           pf == "a"  &&  answer == 3);
    assert(evaluate("a / u", m, pf, answer) == 0 &&
           pf == "au/" && answer == 0);
    assert(evaluate("a / a", m, pf, answer) == 0 &&
           pf == "aa/" && answer == 1);
    cout << "Passed all tests" << endl;
}

