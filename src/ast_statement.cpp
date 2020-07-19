#include "ast/ast_statement.hpp"

void Compound::py_print(std::ostream& out){
        if(dec==NULL && stat==NULL){
                out << std::string(scopeLevel,'\t');
                out << "pass";
        }
        if(dec != NULL){
                dec->py_print(out);
        }
        if(stat != NULL){
               stat->py_print(out);
        }
}

void Compound::mips_print(std::ostream& mips,Context* ctxt){
        if(dec != NULL){
                dec->mips_print(mips, ctxt);
        }
        if (stat != NULL){
                stat->mips_print(mips, ctxt);
        }
}

void Jump::py_print(std::ostream& out){
        if(typ == "return"){
                out << typ;
                if(expr != NULL){
                        out << " ";
                        expr->py_print(out);
                }
        }
}
void Jump::mips_print(std::ostream& mips,Context* ctxt){
        if(typ == "return"){
                std::string endLabel = ctxt->functionLabel("endLabel");
                if(expr!=NULL){
                        ctxt->doingCalc = true;
                        expr->mips_print(mips,ctxt);
                        ctxt->doingCalc = false;
                        mips << "\tlw\t\t$2,8($sp)\n\tnop\n";
                        mips << "\tj\t\t" << endLabel << "\n\tnop\n";                        
                }
                else{
                        mips << "\taddu\t$2,$0,$0\n";
                        mips << "\tj\t\t" << endLabel << "\n\tnop\n";
                }
        }
        else if (typ == "break"){
                mips << "\tbeq\t\t$0,$0," << ctxt->GetEndLabel() << "\n\tnop\n";

        }
        else if (typ == "continue"){
                mips << "\tbeq\t\t$0,$0," << ctxt->GetBeginLabel() << "\n\tnop\n";
        }
}

void Selection::py_print(std::ostream& out){
        out << typ << "(";                      //If statement
        Expr->py_print(out);
        out << "):\n";
        scopeLevel++;
        if(!dynamic_cast<Compound*> (Stat)){
                out << std::string(scopeLevel,'\t');
                Stat->py_print(out);
                out << "\n";
        }
        else{
                Stat->py_print(out);
                out << "\n";         
        }
         scopeLevel--;

        if(EStat != NULL){
                out << std::string(scopeLevel, '\t');
                out << "else:\n";
                scopeLevel++;
                if(!dynamic_cast<Compound*> (EStat)){
                         out << std::string(scopeLevel,'\t');
                         EStat->py_print(out);
                         scopeLevel--;
                         out << "\n";
                }
                else{
                        EStat->py_print(out);
                        scopeLevel--;
                }

        }
        out << "\n";
}


void Selection::mips_print(std::ostream& mips,Context* ctxt){ 
        if(typ=="if"){
                std::string label2 = ctxt->uniqueLabel("IF_END");
                int r1 = ctxt->getRegister();
                if(EStat!=NULL){
                        std::string label1 = ctxt->uniqueLabel("IF_FALSE");
                        ctxt->doingCalc = true;
                        Expr->mips_print(mips,ctxt);
                        ctxt->doingCalc = false;
                        mips << "\tlw\t\t$" << r1 << ",8($sp)\n\tnop\n";
                        ctxt->updateStack(mips,-1);
                        mips << "\tbeq\t\t$" << r1 << ",$0," << label1 << "\n\tnop\n";
                        Stat->mips_print(mips,ctxt);
                        mips << "\tb\t\t" << label2 << "\n\tnop\n";
                        mips << label1 << ":\n";
                        EStat->mips_print(mips,ctxt);                  
                }
                else{
                        ctxt->doingCalc = true;
                        Expr->mips_print(mips,ctxt);
                        ctxt->doingCalc = false;
                        mips << "\tlw\t\t$" << r1 << ",8($sp)\n\tnop\n";
                        ctxt->updateStack(mips,-1);
                        mips << "\tbeq\t\t$" << r1 << ",$0," << label2 << "\n\tnop\n";
                        Stat->mips_print(mips,ctxt);
                }
                mips << label2 << ":\n";  
                ctxt->clearRegister(r1);
        }
        if (typ == "?"){    //Selection statement
                int r = ctxt->getRegister();
                std::string label1 = ctxt->uniqueLabel("TERNARY_FALSE");
                std::string label2 = ctxt->uniqueLabel("TERNARY_END");
                ctxt->doingCalc = true;
                Expr->mips_print(mips,ctxt);
                mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                mips << "\tbeq\t\t$" << r << ",$0," << label1 << "\n\tnop\n";
                ctxt->doingCalc = true;
                Stat->mips_print(mips,ctxt);
                mips << "\tb\t" << label2 << "\n\tnop\n";
                mips << label1 << ":\n";
                EStat->mips_print(mips,ctxt);
                ctxt->doingCalc = false;
                mips << label2 << ":\n";
                ctxt->clearRegister(r);
        }
        if (typ == "switch"){
                std::string label = ctxt->uniqueLabel("SWITCH");
                std::string label2 = ctxt->uniqueLabel("START");
                std::string label3 = ctxt->uniqueLabel("END");
                ctxt->CaseReturn = label3;
                mips << label2 << ":\n";
                mips << "\tb\t\t" << label << "\n\tnop\n";
                ctxt->doingCalc = true;
                Stat->mips_print(mips, ctxt);
                mips << "\tb\t\t" << label3 << "\n\tnop\n";
                mips << label << ":\n";
                Expr->mips_print(mips, ctxt);
                ctxt->doingCalc = false;
                mips << "\tb\t\t" << label2 << "\n\tnop\n";
                mips << label3 << ":\n";
        }

}

void Label::mips_print(std::ostream& mips,Context* ctxt){
        if (typ == "case"){
                std::string label = ctxt->uniqueLabel("CASE");
                std::cout << label << std::endl;
                const_ex->mips_print(mips, ctxt);
                int reg = ctxt->getRegister();
                int reg2 = ctxt->getRegister();
                mips << "\tlw\t\t$" << reg << ",8($sp)\n";
                mips << "\tlw\t\t$" << reg2 << ",16($sp)\n";
                ctxt->updateStack(mips,-1);
                mips << "\tbne\t\t$" << reg << ",$" << reg2 << "," << label << "\n\tnop\n";
                stat->mips_print(mips,ctxt);
                mips << "\tb\t\t" << ctxt->CaseReturn << "\n\tnop\n";
                mips << label << ":\n";
        }
}