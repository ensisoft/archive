
// parser code

#ifndef GATEPROTECT_PARSER_H
#define GATEPROTECT_PARSER_H

#include <string>
#include <deque>
#include <map>
#include <vector>
#include <cassert>

namespace acme
{
    typedef std::string token;
    typedef std::deque<token> token_list;

    // split the input into a list of tokens
    token_list tokenize_string(const std::string& str);

    enum parse_node_type {
        NODE_TYPE_OPERAND,
        NODE_TYPE_OPERATOR_PLUS,
        NODE_TYPE_OPERATOR_MINUS,
        NODE_TYPE_OPERATOR_MULTIPLY,
        NODE_TYPE_OPERATOR_DIVIDE
    };
    struct parse_node {
        int value;
        parse_node_type type;
    };

    inline 
    bool is_operator(const parse_node& pn)
    {
        assert( pn.type >= 0 && pn.type <= NODE_TYPE_OPERATOR_DIVIDE );
        return pn.type > NODE_TYPE_OPERAND;
    }
    
    inline
    bool operator==(const parse_node& lhs, const parse_node& rhs) 
    {
        return (lhs.value == rhs.value &&
                lhs.type == rhs.type);
    }
    
    
    typedef std::vector<parse_node> node_list;
  
    // parser class parses simple expressions such as "3 + 5" into node lists
    // where each node represents a component of the expression. Each parsed component
    // is either an operator or an operand. 
    class parser 
    {
    public:
        parser();
       ~parser();
        
        // perform parsing of the given string expression into components.
        // if parsing fails function returns false. otherwise true.
        bool parse(const std::string& str, node_list& output);
        
    private:
        enum token_type {
            TT_OPERAND_ALPHABETIC_0       = 0,
            TT_OPERAND_ALPHABETIC_1       = 1,
            TT_OPERAND_ALPHABETIC_2       = 2,
            TT_OPERAND_ALPHABETIC_3       = 3,
            TT_OPERAND_ALPHABETIC_4       = 4,
            TT_OPERAND_ALPHABETIC_5       = 5,
            TT_OPERAND_ALPHABETIC_6       = 6,
            TT_OPERAND_ALPHABETIC_7       = 7,
            TT_OPERAND_ALPHABETIC_8       = 8,
            TT_OPERAND_ALPHABETIC_9       = 9,
            TT_OPERAND_ALPHABETIC_10      = 10,
            TT_OPERAND_ALPHABETIC_11      = 11,
            TT_OPERAND_ALPHABETIC_12      = 12,
            TT_OPERAND_ALPHABETIC_13      = 13,
            TT_OPERAND_ALPHABETIC_14      = 14,
            TT_OPERAND_ALPHABETIC_15      = 15,
            TT_OPERAND_ALPHABETIC_16      = 16,
            TT_OPERAND_ALPHABETIC_17      = 17,
            TT_OPERAND_ALPHABETIC_18      = 18,
            TT_OPERAND_ALPHABETIC_19      = 19,
            TT_OPERAND_ALPHABETIC_20      = 20,
            TT_OPERAND_ALPHABETIC_30      = 30,
            TT_OPERAND_ALPHABETIC_40      = 40,
            TT_OPERAND_ALPHABETIC_50      = 50,
            TT_OPERAND_ALPHABETIC_60      = 60,
            TT_OPERAND_ALPHABETIC_70      = 70,
            TT_OPERAND_ALPHABETIC_80      = 80,
            TT_OPERAND_ALPHABETIC_90      = 90,
            TT_OPERAND_ALPHABETIC_100     = 100,
            TT_OPERAND_ALPHABETIC_1000    = 1000,
            TT_OPERAND_ALPHABETIC_MILLION = 1000000,
            TT_OPERAND_NUMERIC,
            TT_OPERAND_ALPHABETIC,
            TT_UNKNOWN,

        };
        bool is_alpha_num(token_type t) const;
        bool is_alpha_single_digit(token_type t) const;
        bool is_alpha_double_digit(token_type t) const;
        bool is_alpha_teen_digit(token_type t) const;
        bool is_alpha_multiplier(token_type t) const;
        bool is_smaller_by_factor_of_ten(token_type greater, token_type smaller) const;
        int value(token_type t) const;

        typedef std::map<std::string, token_type> dictionary;

        bool parse_operand(node_list& output);
        bool parse_operator(node_list& output);
        bool parse_alphabetic_operand(parse_node& n);
        bool parse_numeric_operand(parse_node& n);

        bool has_tokens() const;
        void pop_token();
        bool require_next_token() const;
        bool match_token(const token& t, size_t index=0) const;
        bool match_pop_token(const token& t);
        bool match_pop_token(const token& t, const token& n);
        
        token_type identify_operand_token() const;
        token_type map_token(const token& t) const;
        
        dictionary dict_;
        token_list tokens_;
    };
    
} // acme

#endif // GATEPROTECT_PARSER_H

