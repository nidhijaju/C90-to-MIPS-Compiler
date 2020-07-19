#ifndef AST_OPERATORS_HPP
#define AST_OPERATORS_HPP

#include <string>
#include <iostream>
#include <cmath>

#include "ast_base.hpp"


class BinaryOperator : public BaseNode{
    protected:
        NodePtr left;
        NodePtr right;
        std::string OP;
    public: 
        BinaryOperator(NodePtr l,std::string s, NodePtr r) : left(l), OP(s), right(r) {}
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
};

class Pointer : public BaseNode{
    protected:
        std::vector<NodePtr> v;
    public:
        Pointer(NodePtr temp) { v.push_back(temp); }
        void push(const NodePtr var){
            v.push_back(var);
        }
    
};

#endif
