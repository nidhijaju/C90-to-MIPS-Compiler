#include <iostream>
#include <sstream>
#include <string>

#include "context.hpp"
#include "ast/ast_declaration.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_expression.hpp"

void Declaration::py_print(std::ostream& out) {
    //left->py_print(out);
    if(right != NULL){
        right->py_print(out);
    }
}

void Declaration::mips_print(std::ostream& mips,Context* ctxt){
    if (right != NULL){
        right->mips_print(mips,ctxt);
    }
}


void Direct_Declarator::py_print(std::ostream& out) {
    if (tag == "p" || tag == "empty"){
        left->py_print(out);
        out << "(";
        if (right != NULL) {
            right->py_print(out);
        }
        out << ")";
    }
}

void Direct_Declarator::mips_print(std::ostream& mips,Context* ctxt){
    if(ctxt->getName == true){
        left->mips_print(mips,ctxt);
    }
    else{
        if (tag == "empty"){
        //left->mips_print(mips,ctxt);
        }
        if (tag == "p"){
            //left->mips_print(mips,ctxt);
            right->mips_print(mips,ctxt);
        }
    }
    
}

void Init_dec::py_print(std::ostream& out) {
    dec->py_print(out);
    if (scopeLevel == 0 && globalflag == true){
        globalvar.push_back(dec);
    }
    if(init != NULL){
        out << "=";
        init->py_print(out);
        out << "\n";
    }
    else{
            out << "=0\n";
    }

}

void Init_dec::mips_print(std::ostream& mips,Context* ctxt){
    if (globalflag == true){
        std::ostringstream temp;
        dec->py_print(temp);
        std::string var = temp.str();
        mips << "\t.globl\t" << var << "\n";
        mips << "\t.data\n";
        mips << "\t.align\t2\n";
        mips << "\t.type\t" << var << ", @object\n";
        mips << "\t.size\t" << var << ", 4\n";
        // mips << var << ":\n";
    }
    else {
        std::ostringstream temp;
        dec->py_print(temp);
        std::string var = temp.str();
        ctxt->addLocalVariable(var, scopeLevel);
        ctxt->LocalVarCount++;
        //ctxt->updateStack(mips,1);
    }
    if (globalflag == true){
        dec->mips_print(mips,ctxt);
        mips << ": \n";
        //scopeLevel++;
        if (init != NULL){
            //mips << std::string(scopeLevel, '\t');
            mips << "\t.word\t";
            init->mips_print(mips,ctxt);
        }
        else {
            mips << "\t.word\t0\n";
        }
    }
    else{
        if (init != NULL){
            int r = ctxt->getRegister();
            if(dynamic_cast<Prime_Exp*>(init)){
                ctxt->doingCalc = true;
                init->mips_print(mips,ctxt);
                ctxt->doingCalc = false;
                mips << "\tlw\t\t$" << r << ",8($sp)\n";
                //mips << "\tli\t\t$" << r << ",";
                mips << "\n";
            }
            else{
                 ctxt->doingCalc = true;
                 init->mips_print(mips,ctxt);
                 ctxt->doingCalc = false;
                 mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                 ctxt->updateStack(mips,-1);
            }
            std::ostringstream temp;
            dec->py_print(temp);
            std::string var = temp.str();
            int reg = ctxt->getRegister();
            std::string txt = ctxt->loadWithOffset(var, mips, reg);
            mips << "\tsw\t\t$" << r << "," << txt << "\n";
           // ctxt->updateStack(mips,1);
            ctxt->clearRegister(r);
            ctxt->clearRegister(reg);
            }
    }
    //scopeLevel--;
}

void Function_definition::py_print(std::ostream& out) {
        out << "def ";
        if(dec!=NULL){
            dec->py_print(out);
        }
        out << ":" << "\n";
        scopeLevel++;
        for (int i = 0; i < globalvar.size(); i++){
            out << std::string(scopeLevel, '\t');
            out << "global ";
            globalvar[i]->py_print(out);
            out << "\n";
        }
        if (comp != NULL){
            comp->py_print(out);
        }
        scopeLevel--;
        out << "\n";
        
        /*
        if (dec->get_type(type) == "main") {
            out << "if __name__ == ' __main__':" << "\n";
            scopeLevel++;
            out << std::string(scopeLevel, ' ');
            out << "import sys" << "\n";
            scopeLevel++;
            out << std::string(scopeLevel, ' ');
            out << "ret=";
            dec->py_print(out);
            out << "\n";
            scopeLevel++;
            out << std::string(scopeLevel, ' ');
            out << "sys.exit(ret)" << "\n";
        }
    */
    
}

void Function_definition::mips_print(std::ostream& mips,Context* ctxt){
    mips << "\t.text\n";
    mips << "\t.align\t2\n";
    mips << "\t.globl\t";
    ctxt->getName = true;
    dec->mips_print(mips,ctxt);
    mips << "\n";
    mips << "\t.type\t";
    dec->mips_print(mips,ctxt);
    mips << ", @function\n";
    dec->mips_print(mips,ctxt);
    ctxt->getName = false;
    mips << ": \n";
    scopeLevel++;
    addPrologue(mips);
    dec->mips_print(mips,ctxt);
    if (comp != NULL){
        comp->mips_print(mips,ctxt);
    }
    //mips << "\tmove\t$fp,$sp\n";
    addEpilogue(mips, ctxt->LocalVarCount,ctxt);
    scopeLevel--;
}