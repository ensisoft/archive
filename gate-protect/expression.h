
#ifndef GATEPROTECT_EXPRESSION_H
#define GATEPROTECT_EXPRESSION_H

#include <stack>
#include <vector>
#include <functional>
#include <cassert>
#include <memory>

namespace acme
{
    enum expression_result {
        EXPR_SUCCESS,
        EXPR_DIVISION_BY_ZERO,
        EXPR_INVALID
    };

    template<typename NumberType>
    class expression_component
    {
    public:
        virtual ~expression_component() {}
        virtual expression_result evaluate(std::stack<NumberType>& stack) const = 0;
    };

    template<typename NumberType>
    class operand : public expression_component<NumberType>
    {
    public:
        operand(NumberType n) : n_(n)
        {
        }
        expression_result evaluate(std::stack<NumberType>& stack) const
        {
            stack.push(n_);
            return EXPR_SUCCESS;
        }
    private:
        const NumberType n_;
    };

    template<typename T>
    struct no_validation {
        inline expression_result check(const T& m) { return EXPR_SUCCESS; }
    };

    template<typename T>
    struct zero_check {
        inline expression_result check(const T& m) {
            const T zero(0);
            if (m == zero)
                return EXPR_DIVISION_BY_ZERO;
            return EXPR_SUCCESS;
        }
    };

    template<typename NumberType,
             template <typename> class Functor,
             template <typename> class RightValidator = no_validation>
    class binary_operator : public expression_component<NumberType>
    {
    public:
        expression_result evaluate(std::stack<NumberType>& stack) const
        {
            if (stack.size() < 2)
                return EXPR_INVALID;
            NumberType m = stack.top();
            expression_result ret = RightValidator<NumberType>().check(m);
            if (ret != EXPR_SUCCESS)
                return ret;
            stack.pop();
            NumberType n = stack.top();
            stack.pop();
            stack.push(Functor<NumberType>()(n, m));
            return EXPR_SUCCESS;
        }
    };

    template<typename T>
    std::auto_ptr<expression_component<T> > make_number(T num)
    {
        return std::auto_ptr<expression_component<T> >(new operand<T>(num));
    }

    template<typename T>
    std::auto_ptr<expression_component<T> > make_plus()
    {
        return std::auto_ptr<expression_component<T> >(new binary_operator<T, std::plus>());
    }

    template<typename T>
    std::auto_ptr<expression_component<T> > make_minus()
    {
        return std::auto_ptr<expression_component<T> >(new binary_operator<T, std::minus>());
    }
    
    template<typename T>
    std::auto_ptr<expression_component<T> > make_multiply()
    {
        return std::auto_ptr<expression_component<T> >(new binary_operator<T, std::multiplies>());
    }
    
    template<typename T>
    std::auto_ptr<expression_component<T> > make_divide()
    {
        return std::auto_ptr<expression_component<T> >(new binary_operator<T, std::divides, zero_check>());
    }

    // this template presents an expression in post-fix form.
    // the expression can be evaluated and the result then retrieved
    template<typename NumberType>
    class expression 
    {
    public:
        typedef expression_component<NumberType> component_type;
        typedef std::vector<component_type*> components;
        // this will take ownership of the expression components
        // and delete them appropriately
        expression(const components& comp) : comp_(comp), result_(NumberType())
        {
        }
        expression() : result_(NumberType())
        {
        }
       ~expression()
        {
            for (typename components::iterator it = comp_.begin();
                 it != comp_.end(); ++it)
                delete *it;
        }
        // transfer new component into the expression
        void push(component_type* comp)
        {
            comp_.push_back(comp);
        }
        
        void push(std::auto_ptr<component_type> comp)
        {
            comp_.push_back(comp.release());
        }

        expression_result evaluate()
        {
            std::stack<NumberType> stack;
            for (typename components::const_iterator it = comp_.begin();
                 it != comp_.end(); ++it)
            {
                component_type* pc = *it;
                expression_result res = pc->evaluate(stack);
                if (res != EXPR_SUCCESS)
                    return res;
            }
            // if we only pushed operands onto the stack nothing was actually
            // evaluated since there were no operands. in this case 
            // stack size is larger than 1 while there were no errors
            if (stack.size() != 1)
                return EXPR_INVALID;

            result_ = stack.top();
            return EXPR_SUCCESS;
        }
        NumberType result() const
        {
            return result_;
        }
    private:
        expression(const expression&);
        expression& operator=(const expression&);

        NumberType result_;
        components comp_;
    };
    
} // acme

#endif // 

