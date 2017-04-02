
// normally I would use the unit testing libraries from boost.test
// but since this is as standalone as possible I've "rolled my own"


// simply undef NDEBUG so assert macro works in "relese" build as well
#undef NDEBUG
#include <cassert>
#include <iostream>

#include "../expression.h"
#include "../parser.h"

// define a simple macro to help us "enhance" the expression evaluation later
// to something else if needed
#define TEST_REQUIRE assert

using namespace acme;

//
// Synopsis: Test and verify the tokenizer 
//
// Expected: Generated list of tokens matches test data
//
void test0()
{
    {
        token_list tokens = tokenize_string("");
        TEST_REQUIRE(tokens.empty());
    }
    
    {
        token_list tokens = tokenize_string("one");
        TEST_REQUIRE(tokens.size() == 1);
        TEST_REQUIRE(tokens[0] == "one");
        
    }
    {
        token_list tokens = tokenize_string("one two three");
        TEST_REQUIRE(tokens.size() == 3);
        TEST_REQUIRE(tokens[0] == "one");
        TEST_REQUIRE(tokens[1] == "two");
        TEST_REQUIRE(tokens[2] == "three");
    }
    {
        token_list tokens = tokenize_string("one\t\t    two\tthree");
        TEST_REQUIRE(tokens.size() == 3);
        TEST_REQUIRE(tokens[0] == "one");
        TEST_REQUIRE(tokens[1] == "two");
        TEST_REQUIRE(tokens[2] == "three");
    }
}

//
// Synopsis: Test and verify expression evaluation
//
// Expected: expressions are evaluted correctly
//           malformed expressions are reported as such
//
void test1()
{
    {
        expression<int> exp;
        exp.push(make_number(123));
        TEST_REQUIRE(exp.evaluate() == EXPR_SUCCESS);
        TEST_REQUIRE(exp.result() == 123);
    }
    
    {
        expression<int> exp;
        exp.push(make_number(4));
        exp.push(make_number(5));
        exp.push(make_plus<int>());
        TEST_REQUIRE(exp.evaluate() == EXPR_SUCCESS);
        TEST_REQUIRE(exp.result() == 9);
    }
    

    {
        expression<int> exp;
        exp.push(make_number(5));
        exp.push(make_number(10));
        exp.push(make_multiply<int>());
        TEST_REQUIRE(exp.evaluate() == EXPR_SUCCESS);
        TEST_REQUIRE(exp.result() == 50);
    }
    
    {
        expression<int> exp;
        exp.push(make_number(10));
        exp.push(make_number(5));
        exp.push(make_divide<int>());
        TEST_REQUIRE(exp.evaluate() == EXPR_SUCCESS);
        TEST_REQUIRE(exp.result() == 2);
    }


    // test some error cases
    {
        expression<int> exp;
        exp.push(make_number(1234));
        exp.push(make_plus<int>());
        TEST_REQUIRE(exp.evaluate() == EXPR_INVALID);
    }

    {
        expression<int> exp;
        exp.push(make_number(124));
        exp.push(make_number(0));
        TEST_REQUIRE(exp.evaluate() == EXPR_INVALID);
    }

    {
        expression<int> exp;
        exp.push(make_number(123));
        exp.push(make_number(0));
        exp.push(make_divide<int>());
        TEST_REQUIRE(exp.evaluate() == EXPR_DIVISION_BY_ZERO);
    }
}

struct number_test {
    const char* str;
    int expected_value;
};

//
// Synopsis: Test correct alphabetic/numeric number parsing
//
// Expected: Value parsed matches the actual value
//
void test2()
{
    const number_test test_values[] = {
        {"zero", 0},
        {"one", 1},
        {"ten", 10},
        {"fifteen", 15},
        {"twenty", 20},
        {"thirty", 30},
        {"fifty one", 51},
        {"hundred", 100},
        {"one hundred", 100},
        {"one hundred one", 101},
        {"hundred nineteen", 119},
        {"hundred fifty two", 152},
        {"one thousand", 1000},
        {"two thousand three hundred", 2300},
        {"two thousand one", 2001},
        {"two thousand sixty seven", 2067},
        {"three thousand four hundred", 3400},
        {"three thousand four hundred twenty seven", 3427},
        {"million", 1000000},
        {"three million four hundred thousand three hundred fourty two", 3400342},
        {"three hundred fifty two thousand", 352000},
        {"three hundred fifty thousand", 350000},
        {"fifty two thousand", 52000},
        {"three hundred and fifty two", 352},
        {"four million and one", 4000001},
        {"0", 0},
        {"1", 1},
        {"78", 78},
        {"-3888", -3888},
        {"one million and hundred thousand fifty nine", 1100059},
        {"two million and nineteen hundred", 2001900},
        {"five hundred thousand and nineteen hundred", 501900}
    };
    
    for (size_t i = 0; i<sizeof(test_values) / sizeof(number_test); ++i)
    {
        const number_test& a_val = test_values[i];
        std::cout << "testing: " << a_val.str << std::endl;
        node_list nodes;
        parser p;
        p.parse(a_val.str, nodes);
        TEST_REQUIRE(nodes.size() == 1);
        //std::cout << "result: " << nodes[0].value << std::endl;
        TEST_REQUIRE(nodes[0].value == a_val.expected_value);

    }
}

//
// Synopsis: Test incorrect alphabetic/numeric number parsing
// 
// Expected: incorrect input gives parsing failure errors
//
void test3()
{
    const number_test test_values[] = {
        {"asgljasasg", 0},
        {"one one thousand", 0},
        {"one two three", 0},
        {"two fifty", 0},
        {"fifty nineteen", 0},
        {"fifty sixty", 0},
        {"thousand 45", 0},
        {"nineteen nineteen", 0},
        {"nineteen twenty one", 0},
        {"nineteen tree", 0},
        {"thousand one one", 0}
    };
    
    for (size_t i=0; i<sizeof(test_values) / sizeof(number_test); ++i)
    {
        const number_test& a_val = test_values[i];
        std::cout << "testing: " << a_val.str << std::endl;
        node_list nodes;
        parser p;
        TEST_REQUIRE(!p.parse(a_val.str, nodes));
    }
}

struct expression_test {
    const char* expression;
    size_t expected_count;
    parse_node nodes[10];
    
};

//
// Synopsis: Test correct expression parsing
//
// Expected: A correct parse node list with all the expression components extracted
//
void test4()
{
    const expression_test test_values[] = {
        {"", 0},

        {"5 + 10", 3, {{5, NODE_TYPE_OPERAND},
                       {0, NODE_TYPE_OPERATOR_PLUS},
                       {10, NODE_TYPE_OPERAND}}},

        {"five + 20", 3, {{5, NODE_TYPE_OPERAND},
                          {0, NODE_TYPE_OPERATOR_PLUS},
                          {20, NODE_TYPE_OPERAND}}},
        
        {"minus twenty five multiplied by 10", 3, {{-25, NODE_TYPE_OPERAND},
                                                   {0, NODE_TYPE_OPERATOR_MULTIPLY},
                                                   {10, NODE_TYPE_OPERAND}}},
        
        {"28 divided by 7 plus 44 - one hundred", 7, {{28, NODE_TYPE_OPERAND},
                                                      {0, NODE_TYPE_OPERATOR_DIVIDE},
                                                      {7, NODE_TYPE_OPERAND},
                                                      {0, NODE_TYPE_OPERATOR_PLUS},
                                                      {44, NODE_TYPE_OPERAND},
                                                      {0, NODE_TYPE_OPERATOR_MINUS},
                                                      {100, NODE_TYPE_OPERAND}}},
        
        {"five + minus four", 3, {{5, NODE_TYPE_OPERAND},
                                  {0, NODE_TYPE_OPERATOR_PLUS},
                                  {-4, NODE_TYPE_OPERAND}}},
        
        {"minus three million divided by minus two million", 3, {{-3000000, NODE_TYPE_OPERAND},
                                                                 {0, NODE_TYPE_OPERATOR_DIVIDE},
                                                                 {-2000000, NODE_TYPE_OPERAND}}},
        
        {"2345 increased by -10", 3, {{2345, NODE_TYPE_OPERAND},
                                      {0, NODE_TYPE_OPERATOR_PLUS},
                                      {-10, NODE_TYPE_OPERAND}}}

                                                    
          
    };

    for (size_t i=0; i<sizeof(test_values) / sizeof(expression_test); ++i)
    {
        const expression_test& test = test_values[i];
        std::cout << "testing: " << test.expression << std::endl;
        
        node_list nodes;
        parser p;
        TEST_REQUIRE(p.parse(test.expression, nodes));
        TEST_REQUIRE(nodes.size() == test.expected_count);
        for (size_t x=0; x<test.expected_count; ++x)
            TEST_REQUIRE(nodes[x] == test.nodes[x]);
    }
}

//
// Synopsis: Test incorrect expression parsing
//
// Expected: Empty parse node list, parsing failed return value
//
void test5()
{
    const char* expressions[] = {
        "asljagsgkj",
        "one thousand *",
        "* five",
        "minus minus",
        "minus plus",
        "increased by seventeen",
        "thousand one one"
    };
    
    node_list n;
    parser p;
    for (size_t i=0; i< sizeof(expressions) / sizeof(const char*); ++i)
        TEST_REQUIRE(!p.parse(expressions[i], n));
}


int main()  
{
    test0();
    test1();
    test2();
    test3();
    test4();
    test5();

    std::cout << "Hooray! No errors\n";
    return 0;
}
