#include <sstream>
#include "ast/ast_basefunc.hpp"
#include "setup.hpp"

void List::push(NodePtr var){
    v.push_back(var);
}

void List::py_print(std::ostream& out){
    for(int i = 0; i<v.size();i++){
        if(type == "idl"){ //init declarator list
            if(v[i]!=NULL){
                out << std::string(scopeLevel,'\t');
                v[i]->py_print(out);
            }
        }
        else if(type == "sl"){ //statement list
            if(v[i]!=NULL){
                out << std::string(scopeLevel,'\t'); 
                v[i]->py_print(out);
                out << "\n";
            }
        }
        else if(type == "pl"){ //parameter list
            if(v[i] != NULL){
                if (i != 0){
                    out << ",";
                }
                v[i]->py_print(out);
            }
        }
        else if (type == "e"){ //expression list
            v[i]->py_print(out);
        }
        else if (type == "ael"){ // argument expression list
            if(i!=0){
                out <<",";
            }
            if(v[i]!=NULL){
                v[i]->py_print(out);
            }
        }
        else{
            if(v[i]!=NULL){
                v[i]->py_print(out);
            } 
        }
    }
}

void List::mips_print(std::ostream& mips,Context* ctxt){
    for (int i = 0; i < v.size(); i++){
        if (type == "ael"){
            if (v[i] != NULL){
                ctxt->updateStack(mips, 1);
                ctxt->doingCalc=true;
                v[i]->mips_print(mips,ctxt);
                ctxt->doingCalc=false;
                if ((i + 4) < 8){
                    mips << "\tlw\t\t$" << i + 4 << ",8($sp)\n";
                }
            }
        }
        else if (type == "pl"){
            if (v[i] != NULL){
                std::ostringstream temp;
                v[i]->py_print(temp);
                ctxt->addLocalVariable(temp.str(), scopeLevel);
                int r = ctxt->getRegister();
                mips << "\tmove\t$" << r << ",$" << i+4 << "\n";
                std::string txt = ctxt->loadWithOffset(temp.str(), mips, r);
                mips << "\tsw\t\t$" << r << "," << txt << "\n";
                ctxt->clearRegister(r);
            }
        }
        else if (v[i] != NULL){
            v[i]->mips_print(mips,ctxt);
            
        }
    }
    if (type == "ael" && (v.size() < 4)){
        int left = 4 - v.size();
        ctxt->updateStack(mips, left);
    }
}

void TopNode::push(NodePtr var){
    v.push_back(var);
}

void TopNode::py_print(std::ostream& out){
    for(int i = 0; i<v.size();i++){
        if (dynamic_cast<Declaration*> (v[i])){
            globalflag = true;
            std::ostringstream temp;
            v[i]->py_print(temp);
            globalflag = false;
        }
    }
    for(int i = 0; i<v.size();i++){
        if(v[i]!=NULL){
            v[i]->py_print(out);
        }
    }
    out << "\nif __name__ == \"__main__\":\n";
    out << "\timport sys\n";
    out << "\tret=main()\n";
    out << "\tsys.exit(ret)\n ";
}

void TopNode::mips_print(std::ostream& mips,Context* ctxt){
    addSetup(mips);
    for(int i = 0; i<v.size();i++){
        if (dynamic_cast<Declaration*> (v[i])){
            globalflag = true;
            std::ostringstream temp;
            v[i]->py_print(temp);
            globalflag = false;
        }
    }
    for (int i = 0; i < v.size(); i++){
        if (dynamic_cast<Declaration*> (v[i])){
            globalflag = true;
        }
        else{
            ctxt->FunctionCount++;
            ctxt->LocalVarCount=0;
        }
        if (v[i] != NULL){
            v[i]->mips_print(mips,ctxt);
        }
        mips << "\n";
        globalflag = false;
    }
 
}

