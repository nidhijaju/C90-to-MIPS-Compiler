#ifndef AST_STATEMENT_HPP
#define AST_STATEMENT_HPP

#include "ast_base.hpp"
        


        class Selection : public BaseNode{
                protected:
                        std::string typ;
                        NodePtr Expr;
                        NodePtr Stat;
                        NodePtr EStat;
                public:
                        Selection(std::string t, NodePtr Ex, NodePtr St, NodePtr ESt) : typ(t), Expr(Ex), Stat(St), EStat(ESt) {}
                        void py_print(std::ostream& out);
                        void mips_print(std::ostream& mips,Context* ctxt);
        };

        class Label : public BaseNode{
                protected:
                        std::string typ;
                        NodePtr const_ex;
                        NodePtr stat;
                public:
                        Label(std::string t, NodePtr l, NodePtr r) :  typ(t), const_ex(l), stat(r) {}
                        Label(std::string t, NodePtr r) : typ(t), stat(r) {}     
                        void mips_print(std::ostream& mips,Context* ctxt);

        };

        class Compound: public BaseNode{
                protected:
                        NodePtr stat;
                        NodePtr dec;
                public:
                        Compound(NodePtr s, NodePtr d) : stat(s), dec(d) {}
                        void py_print(std::ostream& out);
                        void mips_print(std::ostream& mips,Context* ctxt);
        };

        class Jump: public BaseNode{
                protected:
                        std::string typ;
                        std::string identifier;
                        NodePtr expr;
                public:
                        Jump(std::string t) : typ(t) {}
                        Jump(std::string t, std::string i) : typ(t), identifier(i) {}
                        Jump(std::string t, NodePtr e) : typ(t), expr(e) {}
                        void py_print(std::ostream& out);
                        void mips_print(std::ostream& mips,Context* ctxt);


        };

#endif
