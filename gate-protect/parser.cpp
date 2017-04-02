
#include <sstream>
#include <algorithm>
#include <iterator>
#include <cassert>

#include "parser.h"

namespace acme
{

token_list tokenize_string(const std::string& str)
{
    token_list list;

    // this tokenizer expects that symbols are separeted by spaces
    // if symbols are given like "5+5" this code should be changed
    // to read character by character and try to match the combined characters
    // against a dictionary
    std::stringstream ss(str);
    std::copy(
        std::istream_iterator<token>(ss),
        std::istream_iterator<token>(),
        std::back_inserter(list)
              );
    return list;
}


parser::parser()
{
    // setup dictionary for keyword-value lookup
    using namespace std;
    dict_["zero"]      = TT_OPERAND_ALPHABETIC_0;
    dict_["one"]       = TT_OPERAND_ALPHABETIC_1;
    dict_["two"]       = TT_OPERAND_ALPHABETIC_2;
    dict_["three"]     = TT_OPERAND_ALPHABETIC_3;
    dict_["four"]      = TT_OPERAND_ALPHABETIC_4;
    dict_["five"]      = TT_OPERAND_ALPHABETIC_5;
    dict_["six"]       = TT_OPERAND_ALPHABETIC_6;
    dict_["seven"]     = TT_OPERAND_ALPHABETIC_7;
    dict_["eight"]     = TT_OPERAND_ALPHABETIC_8;
    dict_["nine"]      = TT_OPERAND_ALPHABETIC_9;
    dict_["ten"]       = TT_OPERAND_ALPHABETIC_10;
    dict_["eleven"]    = TT_OPERAND_ALPHABETIC_11;
    dict_["twelve"]    = TT_OPERAND_ALPHABETIC_12;
    dict_["thirteen"]  = TT_OPERAND_ALPHABETIC_13;
    dict_["fourteen"]  = TT_OPERAND_ALPHABETIC_14;
    dict_["fifteen"]   = TT_OPERAND_ALPHABETIC_15;
    dict_["sixteen"]   = TT_OPERAND_ALPHABETIC_16;
    dict_["seventeen"] = TT_OPERAND_ALPHABETIC_17;
    dict_["eighteen"]  = TT_OPERAND_ALPHABETIC_18;
    dict_["nineteen"]  = TT_OPERAND_ALPHABETIC_19;
    dict_["twenty"]    = TT_OPERAND_ALPHABETIC_20;
    dict_["thirty"]    = TT_OPERAND_ALPHABETIC_30;
    dict_["fourty"]    = TT_OPERAND_ALPHABETIC_40;
    dict_["fifty"]     = TT_OPERAND_ALPHABETIC_50;
    dict_["sixty"]     = TT_OPERAND_ALPHABETIC_60;
    dict_["seventy"]   = TT_OPERAND_ALPHABETIC_70;
    dict_["eighty"]    = TT_OPERAND_ALPHABETIC_80;
    dict_["ninety"]    = TT_OPERAND_ALPHABETIC_90;
    dict_["hundred"]   = TT_OPERAND_ALPHABETIC_100;
    dict_["thousand"]  = TT_OPERAND_ALPHABETIC_1000;
    dict_["million"]   = TT_OPERAND_ALPHABETIC_MILLION;
}

parser::~parser()
{
}

bool parser::parse(const std::string& str, node_list& output)
{
    tokens_ = tokenize_string(str);
    if (tokens_.empty())
        return true;
    // parse first operand
    if (!parse_operand(output))
        return false;

    // parse tokens as much as we can, expect repeating pattern of operator followed by an operand
    while (has_tokens())
    {
        if (!parse_operator(output))
            return false;
        if (!parse_operand(output))
            return false;
    }
    return true;
}

bool parser::parse_operand(node_list& output)
{
    int negate = 1;
    if (match_token("minus") || match_token("-"))
    {
        negate = -1;
        pop_token();
    }
    //  redundant plus
    if (match_token("plus") || match_token("+"))
        pop_token();

    if (!require_next_token())
        return false;

    parse_node operand = {};
    operand.type = NODE_TYPE_OPERAND;

    token_type tt = identify_operand_token();
    bool success  = false;
    if (tt == TT_OPERAND_NUMERIC)
        success = parse_numeric_operand(operand);
    else if (tt == TT_OPERAND_ALPHABETIC)
    {
        if (match_token("zero"))
        {
            pop_token();
            success = true;
        }
        else
        {
            success = parse_alphabetic_operand(operand);
        }
    }
    if (success)
    {
        operand.value *= negate;
        output.push_back(operand);
    }
    return success;
}

bool parser::parse_operator(node_list& output)
{
    if (!require_next_token())
        return false;
    
    output.resize(output.size() + 1);
    parse_node* op = &output[output.size()-1];

    if (match_pop_token("plus") || match_pop_token("+") ||
        match_pop_token("increased", "by"))
    {
        op->type = NODE_TYPE_OPERATOR_PLUS;
        return true;
    }
    else if (match_pop_token("minus") || match_pop_token("-") ||
             match_pop_token("decreased", "by"))
    {
        op->type = NODE_TYPE_OPERATOR_MINUS;
        return true;
    }
    else if (match_pop_token("times") || match_pop_token("*") ||
             match_pop_token("multiplied", "by") ||
             match_pop_token("multiplied", "with"))
    {
        op->type = NODE_TYPE_OPERATOR_MULTIPLY;
        return true;
    }
    else if (match_pop_token("/") ||
             match_pop_token("divided", "by"))
    {
        op->type = NODE_TYPE_OPERATOR_DIVIDE;
        return true;
    }
    // oops failed to parse, 
    output.pop_back();
    return false;
}


bool parser::is_alpha_num(token_type t) const
{
    return (t >= TT_OPERAND_ALPHABETIC_0 && t <= TT_OPERAND_ALPHABETIC_MILLION);
}

bool parser::is_alpha_single_digit(token_type t) const
{
    return (t >= TT_OPERAND_ALPHABETIC_0 && t <= TT_OPERAND_ALPHABETIC_9);
}

bool parser::is_alpha_double_digit(token_type t) const
{
    return (t >= TT_OPERAND_ALPHABETIC_10 && t <= TT_OPERAND_ALPHABETIC_90);
}

bool parser::is_alpha_teen_digit(token_type t) const
{
    return (t >= TT_OPERAND_ALPHABETIC_11 && t <= TT_OPERAND_ALPHABETIC_19);
}

bool parser::is_alpha_multiplier(token_type t) const
{
    // cases of tens are special with irregular patterns so 
    // instead of (x * 10) we have direct value lookup
    return (t == TT_OPERAND_ALPHABETIC_100 ||
            t == TT_OPERAND_ALPHABETIC_1000 ||
            t == TT_OPERAND_ALPHABETIC_MILLION);
}

bool parser::is_smaller_by_factor_of_ten(token_type greater, token_type smaller) const
{
    assert( is_alpha_num(greater) );
    assert( is_alpha_num(smaller) );
    
    // if (greater == TT_OPERAND_ALPHABETIC_MILLION && smaller == TT_OPERAND_ALPHABETIC_1000)
    //     return true;
    // if (greater == TT_OPERAND_ALPHABETIC_1000 && smaller == TT_OPERAND_ALPHABETIC_100)
    //     return true;
    // if (greater == TT_OPERAND_ALPHABETIC_100 && is_alpha_double_digit(smaller))
    //     return true;
    // if (is_alpha_double_digit(greater) && is_alpha_single_digit(smaller))
    //     return true;
    // return false;

    if (smaller >= greater)
        return false;
    // factor out the second coefficient and check the magnitude
    // smaller * 10 * y = greater
    // y = 50 / (smaller * 10)
    // scale up by 10 so no need for fractional numbers
    int ret = (greater * 10) / (smaller * 10);
    return ret < 10;
}

int parser::value(token_type t) const
{
    return static_cast<int>(t);
}


bool parser::parse_alphabetic_operand(parse_node& n)
{
    // number in alphabetical notation is treated as a polynomial.
    // for example "three hundred thousand four hundred fifty two"
    // (3 * 100 * 1000) + (4 * 100) + 50 + 2
    // the algorithm recursively calculates the product for each term and
    // then combines the products to create the final value.

    token_type prev = TT_UNKNOWN;
    while (has_tokens())
    {
        token_type type = map_token(tokens_[0]);
        if (!is_alpha_num(type))
            return true;
        if (is_alpha_single_digit(prev) && 
            (is_alpha_single_digit(type) || is_alpha_double_digit(type)))
            return false;
        if (is_alpha_double_digit(prev) &&  is_alpha_double_digit(type))
            return false;
        if (is_alpha_teen_digit(prev) && is_alpha_single_digit(type))
            return false;
        
        if (is_alpha_multiplier(type))
        {
            // 1 is one implied for example "hundred and fifty" is the same as
            // "one hundred fifty"
            if (n.value == 0)
                n.value = 1;
            // handle a case with multiple sequential multipliers
            // such as "three hundred thousand"
            do
            {
                token_type mul = map_token(tokens_[0]);
                if (!is_alpha_multiplier(mul))
                    break;
                n.value = n.value * value(mul);
                prev = mul;
                pop_token();
            } while(!tokens_.empty());

            if (!tokens_.empty())
            {
                if (match_token("and"))
                    pop_token();

                // calculate the value for the next term in the polynomial
                // and then add the result
                token_type next = map_token(tokens_[0]);
                if (is_alpha_num(next) && !is_smaller_by_factor_of_ten(prev, next))
                {
                    parse_node m = {};
                    if (!parse_alphabetic_operand(m))
                        return false;
                    n.value += m.value;
                }
            }
        }
        else
        {
            n.value += value(type);
            pop_token();
        }
        prev = type;
    }
    return true;
}

bool parser::parse_numeric_operand(parse_node& n)
{
    const token t = tokens_[0];
    std::stringstream ss(t);
    int i = 0;
    ss >> i;
    if (ss.fail() || ss.bad())
        return false;
    
    n. value = i;
    pop_token();
    return true;
}

bool parser::has_tokens() const
{
    return !tokens_.empty();
}

void parser::pop_token() 
{
    assert( !tokens_.empty() );
    tokens_.pop_front();
}

bool parser::require_next_token() const
{
    return !tokens_.empty();
}

bool parser::match_token(const token& t, size_t index) const
{
    if (!(index < tokens_.size()))
        return false;
    return tokens_[index] == t;
}

bool parser::match_pop_token(const token& t) 
{
    if (match_token(t))
    {
        pop_token();
        return true;
    }
    return false;
}

bool parser::match_pop_token(const token& t, const token& m)
{
    if (match_token(t) && match_token(m, 1))
    {
        pop_token();
        pop_token();
        return true;
    }
    return false;
}

parser::token_type parser::identify_operand_token() const
{
    if (tokens_.empty())
        return TT_UNKNOWN;

    const token& tok = tokens_[0];

    dictionary::const_iterator it = dict_.find(tok);
    if (it != dict_.end())
    {
        if (is_alpha_num(it->second))
            return TT_OPERAND_ALPHABETIC;
    }

    // if its convertible to a value it's probably a numeric operand
    if (::isdigit(tok[0]) || tok[0] == '-' || tok[0] == '+')
    {
        std::stringstream ss(tok);
        int val = 0;
        ss >> val;
        if (!ss.fail() && !ss.bad())
            return TT_OPERAND_NUMERIC;
    }
    return TT_UNKNOWN;
}

parser::token_type parser::map_token(const token& t) const
{
    dictionary::const_iterator it = dict_.find(t);
    if (it == dict_.end())
        return TT_UNKNOWN;
    
    return it->second;
}



} // acme

