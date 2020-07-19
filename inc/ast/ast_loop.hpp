#ifndef AST_LOOP_HPP
#define AST_LOOP_HPP

#include "ast_base.hpp"

        class While_Loop : public BaseNode{
                protected:
                        NodePtr Stat;
                        NodePtr Expr;
                public: 
                        While_Loop(NodePtr St,NodePtr Ex) : Stat(St), Expr(Ex) {}
                        void py_print(std::ostream& out);
                        void mips_print(std::ostream& mips,Context* ctxt);
                
        };

        class For_Loop : public BaseNode{
                protected:
                        NodePtr Expr_stat1, Expr_stat2;
                        NodePtr Expr;
                        NodePtr Stat;
                public: 
                        For_Loop(NodePtr ExSt1,NodePtr ExSt2, NodePtr Ex, NodePtr St) : Expr_stat1(ExSt1), Expr_stat2(ExSt2), Expr(Ex), Stat(St) {}
                        void mips_print(std::ostream& mips,Context* ctxt);
                
        };

#endif
