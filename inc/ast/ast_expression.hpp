#ifndef AST_EXPRESSION_HPP
#define AST_EXPRESSION_HPP

#include <string>
#include <iostream>
#include <map>

#include <memory>

#include "ast_base.hpp"

class Prime_Exp : public BaseNode{
        protected:
                std::string typ;
                std::string val;
                NodePtr exp;
        public:
                Prime_Exp(std::string t, std::string v) : typ(t), val(v) {}
                void py_print(std::ostream& out);
                void mips_print(std::ostream& mips,Context* ctxt);
};

class Postfix_Exp : public BaseNode{
        protected:
                NodePtr post_exp;
                NodePtr exp;
                std::string val;
                std::string tag;
       public:
                Postfix_Exp(NodePtr e, std::string v) : post_exp(e), val(v) {}
                Postfix_Exp(NodePtr pe, NodePtr e, std::string s) : post_exp(pe), exp(e), tag(s) {}
                void py_print(std::ostream& out);
                void mips_print(std::ostream& mips,Context* ctxt);
       
};

#endif
