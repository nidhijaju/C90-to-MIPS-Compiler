#ifndef AST_SPECIFIERS_HPP
#define AST_SPECIFIERS_HPP

#include <string>
#include <iostream>

#include "ast_base.hpp"

    class Declaration_Specifier : public BaseNode{
        protected:
            std::string str;
            NodePtr left;
            NodePtr right;
        public:
            Declaration_Specifier(NodePtr l, NodePtr r) : str(""), left(l), right(r) {}
            Declaration_Specifier(std::string s, NodePtr r) : str(s), left(NULL), right(r) {}
            void pyprint(std::ostream& out) const;

        
    };

    class Enumerator: public BaseNode{
        protected:
            NodePtr ptr;
            std::string str;
        public:
            Enumerator(std::string s, NodePtr p) : str(s), ptr(p) {}
        
    };
    
    class Struct_Union : public BaseNode {
         protected:
            std::string typ;
            std::string identifier;
            NodePtr struct_list;

        public:
            Struct_Union(std::string t, std::string i, NodePtr s) : typ(t), identifier(i), struct_list(s) {}
       
    };

#endif
