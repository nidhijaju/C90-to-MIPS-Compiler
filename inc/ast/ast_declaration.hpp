#ifndef AST_DECLARATION_HPP
#define AST_DECLARATION_HPP

#include <string>
#include <iostream>

#include "setup.hpp"
#include "ast_base.hpp"


class Declaration : public BaseNode {
    protected:
        NodePtr left;
        NodePtr right;
        std::string sleft;
        std::string sright;
    public:
        Declaration(NodePtr l, NodePtr r) : left(l), right(r) {}
        Declaration(std::string l, std::string r) : sleft(l), sright(r) {}
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
};

class Init_dec : public BaseNode{
    protected:
        NodePtr dec;
        NodePtr init;
    public:
        Init_dec(NodePtr d, NodePtr i) : dec(d), init(i) {} 
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
    
};

class Function_definition : public BaseNode{
    protected:
        NodePtr decSpec;
        NodePtr dec;
        NodePtr decList;
        NodePtr comp;
    public:
        Function_definition(NodePtr s, NodePtr d, NodePtr dl, NodePtr c) : decSpec(s), dec(d), decList(dl), comp(c) {}
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
};

class Abs_dec : public BaseNode{
    protected:
        NodePtr ptr;
        NodePtr dirDec;
    public:
        Abs_dec(NodePtr p, NodePtr dad): ptr(p), dirDec(dad) {}
    
};

class Direct_Declarator : public BaseNode{
    protected:
        NodePtr left;
        NodePtr right;
        std::string tag;
    public:
        Direct_Declarator(NodePtr l, NodePtr r, std::string t): left(l), right(r), tag(t) {}
        void py_print(std::ostream& out);
        void mips_print(std::ostream& mips,Context* ctxt);
        // void getVariables(std::vector<std::string> s);
};


#endif
