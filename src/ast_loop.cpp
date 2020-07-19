#include "ast/ast_loop.hpp"
#include "ast/ast_statement.hpp"

void While_Loop::py_print(std::ostream& out){
    out << "while ( ";
    Expr->py_print(out);            //Get the expression
    out << "):\n";
    scopeLevel++;
    if(!dynamic_cast<Compound*> (Stat)){ //Check if its not pointing to a compound statement
        out << std::string(scopeLevel,'\t');
        Stat->py_print(out);
        scopeLevel--;
        out << "\n";
    }
    else{
        Stat->py_print(out);
        scopeLevel--;
    }
}

void While_Loop::mips_print(std::ostream& mips,Context* ctxt){
    std::string label1 = ctxt->uniqueLabel("WHILE_CONDITION");
    std::string label2 = ctxt->uniqueLabel("WHILE_CONTENT");
    std::string label3 = ctxt->uniqueLabel("WHILE_END");
    ctxt->AddBeginLabel(label1);
    ctxt->AddEndLabel(label3);
    mips << "\tb\t\t" << label1 << "\n\tnop\n";
    mips << label2 << ":\n";
    Stat->mips_print(mips,ctxt);
    mips << label1 << ":\n";
    ctxt->doingCalc = true;
    Expr->mips_print(mips,ctxt);
    ctxt->doingCalc = false;
    int r1 = ctxt->getRegister();
    mips << "\tlw\t\t$" << r1 << ",8($sp)\n\tnop\n";
    ctxt->updateStack(mips,-1);
    mips << "\tbne\t\t$" << r1 << ",$0," << label2 << "\n\tnop\n";
    mips << label3 << ":\n";
    ctxt->clearRegister(r1);
}

void For_Loop::mips_print(std::ostream& mips,Context* ctxt){
    std::string label1 = ctxt->uniqueLabel("FOR_CONDITION");
    std::string label2 = ctxt->uniqueLabel("FOR_CONTENT");
    std::string label3 = ctxt->uniqueLabel("FOR_END");
    ctxt->AddBeginLabel(label1);
    ctxt->AddEndLabel(label3);
    Expr_stat1->mips_print(mips,ctxt);
    mips << "\tb\t\t" << label1 << "\n\tnop\n";
    mips << label2 << ":\n";
    Stat->mips_print(mips,ctxt);
    if (Expr != NULL){
        Expr->mips_print(mips,ctxt);
    }
    mips << label1 << ":\n";
    Expr_stat2->mips_print(mips,ctxt);
    int r1 = ctxt->getRegister();
    mips << "\tlw\t\t$" << r1 << ",8($sp)\n\tnop\n";
    ctxt->updateStack(mips,-1);
    mips << "\tbne\t\t$" << r1 << ",$0," << label2 << "\n\tnop\n";
    mips << label3 << ":\n";
    ctxt->clearRegister(r1);
}