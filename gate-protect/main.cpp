

#include <iostream>
#include <stack>
#include "parser.h"
#include "expression.h"

using namespace acme;
using namespace std;

node_list to_postfix_notation(const node_list& infix)
{
    // simple algorithm for turning an infix expression (without subexpresions)
    // to postfix notation. put all expression operands into the queue
    // and push operators onto stack. once expression is finished pop the operators
    // off the stack and tack them on the queue
    node_list postfix;
    stack<parse_node> ops;

    for (node_list::const_iterator it = infix.begin(); 
         it != infix.end(); ++it)
    {
        const parse_node& node = *it;
        if (is_operator(node))
            ops.push(node);
        else 
            postfix.push_back(node);
    }
    while (!ops.empty())
    {
        postfix.push_back(ops.top());
        ops.pop();
    }
    return postfix;
}

void evaluate_expression(const std::string& str)
{
    parser p;
    node_list nodes;
    if (!p.parse(str, nodes))
    {
        cout << "malformed expression.\n";
        return;
    }

    node_list postfix = to_postfix_notation(nodes);

    expression<double> exp;
    
    for (node_list::const_iterator it = postfix.begin();
         it != postfix.end(); ++it)
    {
        const parse_node& node = *it;
        // push the nodes into the expression
        switch (node.type)
        {
            case NODE_TYPE_OPERAND:
                exp.push(make_number<double>(node.value));
                break;
            case NODE_TYPE_OPERATOR_PLUS:
                exp.push(make_plus<double>());
                break;
            case NODE_TYPE_OPERATOR_MINUS:
                exp.push(make_minus<double>());
                break;
            case NODE_TYPE_OPERATOR_MULTIPLY:
                exp.push(make_multiply<double>());
                break;
            case NODE_TYPE_OPERATOR_DIVIDE:
                exp.push(make_divide<double>());
                break;
        }
    }
    expression_result ret = exp.evaluate();
    if (ret == EXPR_SUCCESS)
        cout << "Result: " << exp.result() << endl;
    else if (ret == EXPR_DIVISION_BY_ZERO)
        cout << "division by zero.\n";
    else 
        cout << "expression failed to evaluate\n";
}

int main(int argc, char* argv[])
{
    cout << "\nWelcome to Calculator application!\n\n";

    // enter a for-ever loop and read and process user input line by line
    for (;;)
    {
        cout << "Expression: ";
        string expression;
        getline(std::cin, expression);
        if (expression.empty())
        {
            cout << "empty expressions cannot be evaluated.\n";
        }
        else
        {
            evaluate_expression(expression);
        }
    }
    return 0;
}
