#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <cstdlib>
#include <fstream>

#include "ast/ast_base.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_expression.hpp"
#include "ast/ast_loop.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_specifiers.hpp"
#include "ast/ast_statement.hpp"
#include "ast/ast_basefunc.hpp"

extern BaseNode* parseAST(std::string file);

#endif
