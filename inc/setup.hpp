#ifndef SETUP_HPP
#define SETUP_HPP

#include <string>
#include <sstream>

#include "context.hpp"


void addSetup(std::ostream& mips);
void addPrologue(std::ostream& mips);
void addEpilogue(std::ostream& mips, int LocalVarCount, Context* ctxt);

#endif