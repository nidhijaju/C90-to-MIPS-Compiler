#include <vector>
#include "setup.hpp"

void addSetup(std::ostream& mips){
    mips << ".section .mdebug.abi32\n";
    mips << ".previous\n";
    mips << ".nan legacy\n";
    mips << ".module fp=32\n";
    mips << ".module nooddspreg\n";
    mips << ".abicalls\n";
}

void addPrologue(std::ostream& mips){
    mips << "\t#start of prologue \n";  
    mips << "\taddiu\t$sp,$sp," << -8 <<"\n";
    mips << "\tsw\t\t$31,8($sp)\t#save return address in stack\n";
    mips << "\tsw\t\t$fp,4($sp)\t#save old frame pointer in stack\n";
    mips << "\tmove\t$fp,$sp\n";
    mips << "\taddiu\t$sp,$sp," << -72 <<"\t#making space for declaring variables\n";

    mips << "\t#end of prologue \n \n";
}

void addEpilogue(std::ostream& mips, int LocalVarCount, Context* ctxt){
    //Load any variables from stack
    mips << "\n\t#start of epilogue \n";
    std::string endLabel = ctxt->functionLabel("endLabel");
    mips << endLabel << ":\n";
    mips << "\tmove\t$sp,$fp\n";
    mips << "\tlw\t\t$31," << "8($sp)\t#restore return address from stack \n\tnop\n";
    mips << "\tlw\t\t$fp," <<  "4($sp)\t#restore old frame pointer from stack \n\tnop\n";
    mips << "\taddiu\t$sp,$sp," << "8\t#reset stack pointer " <<(8+LocalVarCount*8) << "\n"; 
    mips << "\t#end of epilogue \n";
    mips << "\tj\t\t$31\t#return \n";
    mips << "\tnop \n";
}