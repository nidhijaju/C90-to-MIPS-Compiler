#ifndef AST_BASE_HPP
#define AST_BASE_HPP

#include <string>
#include <iostream>
#include <vector>

#include "context.hpp"

class BaseNode;

typedef BaseNode *NodePtr;
extern int scopeLevel;
extern std::vector<NodePtr> globalvar;
extern bool globalflag;


class BaseNode {
    protected:
        std::string type;
        std::string value;
    public:
        virtual std::string get_type(std::string typ) {}
        virtual ~BaseNode() {}
        virtual void py_print(std::ostream& out) {}
        virtual void mips_print(std::ostream& mips,Context* ctxt) {}
        virtual void push(NodePtr var){}
        virtual void push(std::string var) {}

};

#endif

