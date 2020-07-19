#ifndef AST_BASEFUNC_HPP
#define AST_BASEFUNC_HPP

#include "ast/ast_base.hpp"
#include "ast/ast_declaration.hpp"
#include "context.hpp"

class TopNode : public BaseNode{        //Used for g-root. The root node of the tree
    protected:
        std::vector<NodePtr> v;
    public:
        TopNode(){}
        void push(NodePtr var);
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
};

class List : public BaseNode{       //Used for most lists
    protected:
        std::vector<NodePtr> v;
        std::string type;
    public:
        List(NodePtr ptr, std::string t) : type(t) {v.push_back(ptr);}
        void push(NodePtr var);
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
};

class SList : public BaseNode{      //A list used for storing strings
    protected:
        std::string str;
        std::vector<std::string> v;
    public:
        SList(std::string s) {v.push_back(s); }
        void push(std::string var){
            v.push_back(var);
        }
    
};

#endif