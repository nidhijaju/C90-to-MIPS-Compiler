#include <string>
#include <sstream>
#include "ast/ast_expression.hpp"


void Prime_Exp::py_print(std::ostream& out){
        out << val;
}

void Prime_Exp::mips_print(std::ostream& mips,Context* ctxt){
        if (globalflag == true){
                mips << val;
        }
        else{
                if(ctxt->doingCalc){
                        if(typ=="ident"){
                                int r = ctxt->getRegister();
                                int reg = ctxt->getRegister();
                                std::string txt = ctxt->loadWithOffset(val, mips, reg);
                                mips << "\tlw\t\t$" << r <<","<< txt << " \n\tnop\n";
                                ctxt->updateStack(mips,1);
                                mips << "\tsw\t\t$" << r <<","<<"8($sp)\n";
                                ctxt->clearRegister(r);
                                ctxt->clearRegister(reg);
                        }
                        else if(typ =="int"){
                                int r = ctxt->getRegister();
                                mips << "\taddiu\t$" << r <<",$0," << val << "  #reloading variable to stack\n" ;
                                ctxt->updateStack(mips,1);
                                mips << "\tsw\t\t$" << r <<","<<"8($sp)\n";
                                ctxt->clearRegister(r);
                        }
                }
                else{
                        mips << val;
                }
                
                
                
        }

}

void Postfix_Exp::py_print(std::ostream& out){
        post_exp->py_print(out);
        if (tag == "ael" || tag == "empty"){
                out << "(";
                if (exp != NULL){
                        exp->py_print(out);
                }
                out << ")";
        }
}

void Postfix_Exp::mips_print(std::ostream& mips,Context* ctxt){
        if (tag == "ael" || tag == "empty"){
                if (exp != NULL){
                        exp->mips_print(mips,ctxt);
                }
                std::ostringstream temp;
                post_exp->py_print(temp);
                mips << "\tjal\t\t" << temp.str() << "\n\tnop\n";
                ctxt->updateStack(mips,1);
                mips << "\tsw\t\t$2,8($sp)\n"; 
        }
}