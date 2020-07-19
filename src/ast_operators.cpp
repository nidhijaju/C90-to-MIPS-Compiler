#include <sstream>
#include "ast/ast_operators.hpp"

void BinaryOperator::py_print(std::ostream& out){
        if(left!=NULL){
                out << "(";
                left->py_print(out);
                out << ")";
        }
        else if(OP == "-"){
                out << "(0)";
        }
        if (OP == "||"){
                OP = " or ";
        }
        if (OP == "&&"){
                OP = " and ";
                
        }
        out << OP;
        if(right!=NULL){
                out << "(";
                right->py_print(out);
                out << ")";
        }
}

void BinaryOperator::mips_print(std::ostream& mips,Context* ctxt){
        if(left==NULL){ //unary operators
                if(OP == "-"){
                        ctxt->doingCalc = true;
                        right->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        mips << "\tsubu\t$" << r <<",$0,$" << r << "\n" ;
                        mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        ctxt->clearRegister(r);
                }
                if (OP == "+"){
                        ctxt->doingCalc = true;
                        right->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        mips << "\taddu\t$" << r <<",$0,$" << r << "\n" ;
                        mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        ctxt->clearRegister(r);
                }
                if (OP == "~"){
                        ctxt->doingCalc = true;
                        right->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        mips << "\tnor\t$" << r <<",$0,$" << r << "\n" ;
                        mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        ctxt->clearRegister(r);
                }
                if (OP == "!"){
                        ctxt->doingCalc = true;
                        right->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        mips << "\tsltu\t$" << r << ",$" << r << ",1\n" ;
                        mips << "\tandi\t$" << r << ",$" << r << ",0x00ff\n";
                        mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        ctxt->clearRegister(r);
                }
                if (OP == "++"){
                        bool calc = ctxt->doingCalc;
                        ctxt->doingCalc = true;
                        right->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        mips << "\taddiu\t$" << r <<",$" << r << ",1\n" ;
                        std::ostringstream temp;
                        right->py_print(temp);
                        int reg = ctxt->getRegister();
                        std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                        mips << "\tsw\t\t$" << r <<","<< txt << "\n";
                        if (calc == true){
                                mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        }
                        else {
                                ctxt->updateStack(mips,-1);
                        }
                        ctxt->clearRegister(r);
                        ctxt->clearRegister(reg);
                }
                if (OP == "--"){
                        bool calc = ctxt->doingCalc;
                        ctxt->doingCalc = true;
                        right->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        mips << "\taddiu\t$" << r <<",$" << r << ",-1\n" ;
                        std::ostringstream temp;
                        right->py_print(temp);
                        int reg = ctxt->getRegister();
                        std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                        mips << "\tsw\t\t$" << r <<","<< txt << "\n";
                        if (calc == true){
                                mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        }
                        else {
                                ctxt->updateStack(mips,-1);
                        }
                        ctxt->clearRegister(r);
                        ctxt->clearRegister(reg);
                }
                ctxt->doingCalc = false;
        }
        else if (right == NULL){
                if (OP == "++"){
                        bool calc = ctxt->doingCalc;
                        ctxt->doingCalc = true;
                        left->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        if (calc == true){
                                mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        }
                        else {
                                ctxt->updateStack(mips,-1);
                        }
                        mips << "\taddiu\t$" << r <<",$" << r << ",1\n" ;
                        std::ostringstream temp;
                        left->py_print(temp);
                        int reg = ctxt->getRegister();
                        std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                        mips << "\tsw\t\t$" << r <<","<< txt << "\n";
                        ctxt->clearRegister(r);
                        ctxt->clearRegister(reg);
                }
                if (OP == "--"){
                        bool calc = ctxt->doingCalc;
                        ctxt->doingCalc = true;
                        left->mips_print(mips,ctxt);
                        int r = ctxt->getRegister();
                        mips << "\tlw\t\t$" << r << ",8($sp)\n\tnop\n";
                        if (calc == true){
                                mips << "\tsw\t\t$" << r << ",8($sp)\n";
                        }
                        else {
                                ctxt->updateStack(mips,-1);
                        }
                        mips << "\taddiu\t$" << r <<",$" << r << ",-1\n" ;
                        std::ostringstream temp;
                        left->py_print(temp);
                        int reg = ctxt->getRegister();
                        std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                        mips << "\tsw\t\t$" << r <<","<< txt << "\n";
                        ctxt->clearRegister(r);
                        ctxt->clearRegister(reg);
                }
                ctxt->doingCalc = false;
        }
        else{
        ctxt->doingCalc = true;
        if(left!=NULL){
                left->mips_print(mips, ctxt);
        }
        else{
                
        }
        if(right!=NULL){
                right->mips_print(mips, ctxt);
        }
        int r1 = ctxt->getRegister();
        int r2 = ctxt->getRegister();

        mips << "\tlw\t\t$"<< r1 <<",16($sp)\n\tnop\n";
        mips << "\tlw\t\t$"<< r2 <<",8($sp)\n\tnop\n";
        ctxt->updateStack(mips,-2);
        //arithmetic operations
        if (OP == "+"){
                ctxt->updateStack(mips,1);
                mips << "\taddu\t$"<< r1 <<",$" << r1 << ",$"<< r2 <<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if (OP == "-"){
                ctxt->updateStack(mips,1);
                mips << "\tsubu\t$"<< r1 <<",$" << r1 << ",$"<< r2 <<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if(OP == "*"){
                ctxt->updateStack(mips,1);
                mips << "\tmultu\t$"<< r1 << ",$"<< r2 <<"\n";
                mips << "\tmflo\t$"<< r1 << "\n";
                mips << "\tmfhi\t$"<< r2 << "\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
                mips << "\tsw\t\t$" << r2 <<",4($sp)\n";
        }
        if(OP == "/"){
                ctxt->updateStack(mips,1);
                mips << "\tdivu\t$"<< r1 << ",$"<< r2 <<"\n";
                mips << "\tmflo\t$"<< r1 << "\n";
                mips << "\tmfhi\t$"<< r2 << "\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
                mips << "\tsw\t\t$" << r2 <<",4($sp)\n";
        }
        //logical operators
        if(OP == "%"){
                ctxt->updateStack(mips,1);
                mips << "\tdivu\t$"<< r1 << ",$"<< r2 <<"\n";
                mips << "\tmfhi\t$"<< r1 << "\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if(OP == "^"){
                ctxt->updateStack(mips,1);
                mips << "\txor\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if(OP == "|"){
                ctxt->updateStack(mips,1);
                mips << "\tor\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if(OP == "&"){
                ctxt->updateStack(mips,1);
                mips << "\tand\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if(OP == "<<"){
                ctxt->updateStack(mips,1);
                mips << "\tsll\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }
        if(OP == ">>"){
                ctxt->updateStack(mips,1);
                mips << "\tsra\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                mips << "\tsw\t\t$" << r1 <<",8($sp)\n";
        }

        if(OP == "&&"){
                ctxt->updateStack(mips,1);
                std::string label1 = ctxt->uniqueLabel("LOGAND");
                std::string label2 = ctxt->uniqueLabel("LOGAND");
                mips << "\tbeq\t\t$" << r1 << "$0," << label1 << "\n\tnop\n";
                mips << "\tbeq\t\t$" << r2 << "$0," << label1 << "\n\tnop\n";
                mips << "\tli\t\t$" << r1 << ",1\n";
                mips << "\tb\t\t"<< label2 << "\n\tnop\n";
                mips << label1 << ":\n";
                mips << "\tmove\t$" << r1 << ",$0\n";
                mips << label2 << ":\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        if(OP == "||"){
                ctxt->updateStack(mips,1);
                std::string label1 = ctxt->uniqueLabel("LOGOR");
                std::string label2 = ctxt->uniqueLabel("LOGOR");
                std::string label3 = ctxt->uniqueLabel("LOGOR");
                mips << "\tbne\t\t$" << r1 << "$0," << label1 << "\n\tnop\n";
                mips << "\tbeq\t\t$" << r2 << "$0," << label2 << "\n\tnop\n";
                mips << label1 << ":\n";
                mips << "\tli\t\t$" << r1 << ",1\n";
                mips << "\tb\t\t"<< label3 << "\n\tnop\n";
                mips << label2 << ":\n";
                mips << "\tmove\t$" << r1 << ",$0\n";
                mips << label3 << ":\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        //assign operators
        if(OP == "="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r2 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if(OP == "+="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\taddu\t$"<< r1 <<",$" << r1 << ",$"<< r2 <<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "-="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tsubu\t$"<< r1 <<",$" << r1 << ",$"<< r2 <<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "*="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tmultu\t$"<< r1 << ",$"<< r2 <<"\n";
                mips << "\tmflo\t$"<< r1 << "\n";
                mips << "\tmfhi\t$"<< r2 << "\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$" << r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "/="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tdivu\t$"<< r1 << ",$"<< r2 <<"\n";
                mips << "\tmflo\t$"<< r1 << "\n";
                mips << "\tmfhi\t$"<< r2 << "\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "&="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tand\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "%="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tdivu\t$"<< r1 << ",$"<< r2 <<"\n";
                mips << "\tmfhi\t$"<< r1 << "\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "|="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tor\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "^="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\txor\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == "<<="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tsll\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        if (OP == ">>="){
                std::ostringstream temp;
                left->py_print(temp);
                int reg = ctxt->getRegister();
                ctxt->updateStack(mips, 1);
                mips << "\tsra\t$"<< r1 << ",$"<< r1 << ",$"<< r2<<"\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, reg);
                mips << "\tsw\t\t$"<< r1 <<","<< txt << "\n"; 
                ctxt->doingCalc = false;
                ctxt->clearRegister(reg);
        }
        //bitwise operators
        if(OP == "<"){
                ctxt->updateStack(mips,1);
                mips << "\tslt\t\t$" << r1 << ",$" << r1 << ",$" << r2 << "\n";
                mips << "\tandi\t$" << r1 << ",$" << r1 << ",0x00ff\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        if(OP == ">"){
                ctxt->updateStack(mips,1);
                mips << "\tslt\t\t$" << r1 << ",$" << r2 << ",$" << r1 << "\n";
                mips << "\tandi\t$" << r1 << ",$" << r1 << ",0x00ff\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        if(OP == "<="){
                ctxt->updateStack(mips,1);
                mips << "\taddi\t$" << r2 << ",$" << r2 << ",1\n"; 
                mips << "\tslt\t\t$" << r1 << ",$" << r1 << ",$" << r2 << "\n";
                mips << "\tandi\t$" << r1 << ",$" << r1 << ",0x00ff\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        if(OP == ">="){
                ctxt->updateStack(mips,1);
                mips << "\taddi\t$" << r1 << ",$" << r1 << ",1\n";
                mips << "\tslt\t\t$" << r1 << ",$" << r2 << ",$" << r1 << "\n";
                mips << "\tandi\t$" << r1 << ",$" << r1 << ",0x00ff\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        if(OP == "!="){
                ctxt->updateStack(mips,1);
                mips << "\txor\t$" << r1 << ",$" << r2 << ",$" << r1 << "\n";
                mips << "\tsltu\t$" << r1 << ",$0,$" << r1 << "\n";
                mips << "\tandi\t$" << r1 << ",$" << r1 << ",0x00ff\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        if(OP == "=="){
                ctxt->updateStack(mips,1);
                mips << "\txor\t$" << r1 << ",$" << r2 << ",$" << r1 << "\n";
                mips << "\tsltu\t$" << r1 << ",$" << r1 << ",1\n";
                mips << "\tandi\t$" << r1 << ",$" << r1 << ",0x00ff\n";
                mips << "\tsw\t\t$" << r1 << ",8($sp)\n";
        }
        
        ctxt->clearRegister(r1);
        ctxt->clearRegister(r2);
        }
}