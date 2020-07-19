#include <string>

#include "context.hpp"

std::string Context::uniqueLabel(std::string s){
         return "_"+s+"_"+std::to_string(uniqueLabelNumber++);
}
std::string Context::functionLabel(std::string s){
         return "_"+s+"_"+std::to_string(FunctionCount);
}

Context::Context(){
	regs[0] = true; 
        regs[1] = true; 
	//Return Registers
	regs[2] = true; 
	regs[3] = true;

        //Argument Registers
        regs[4] = true;
        regs[5] = true;
        regs[6] = true;
        regs[7] = true;

        //Can't be used - Kernel Reserved Register
	regs[26] = true;
	regs[27] = true;
        //Global Pointer
	regs[28] = true;
        //Stack Pointer
	regs[29] = true;
        //Frame Pointer
	regs[30] = true;
        //Return Address in a subroutine
	regs[31] = true;
}

//Global Variables
void Context::addGlobalVariable(std::string name){
        int location = GlobalVarBindings.size();
        GlobalVarBindings.emplace(name, location);
}

void Context::deleteGlobalVariable(std::string name){
        GlobalVarBindings.erase(name);
}

unsigned Context::FindGlobalVariable(std::string name){
        if(GlobalVarBindings.find(name) == GlobalVarBindings.end()){
                return 0;
        }
        else{
                return 1;
        }
}

//Local Variables
void Context::addLocalVariable(std::string name, int scopeLevel){ // add FunctionLabel
        unsigned int location = LocalVarBindings.size();
        std::string temp = name + std::to_string(scopeLevel);
        LocalVarBindings.emplace(temp, location);
}

void Context::deleteLocalVariable(std::string name){
        LocalVarBindings.erase(name);
}

int Context::FindLocalVariable(std::string name, int scopeLevel){
        for(int i = 0; i < scopeLevel; i++){
               std::string tmp = name + std::to_string(scopeLevel); // add FunctionLabel
               if (LocalVarBindings.find(tmp) != LocalVarBindings.end()){
                       return ((LocalVarBindings.find(tmp)->second))*-8;
               }
        }
}

int Context::getRegister(){
        for(int i=0; i<32;i++){
                if(regs[i]==false){
                        regs[i]=true;
                        return i; 
                }
        }
        return -1;
}

void Context::clearRegister(int r){
        regs[r] = false;
}

void Context::updateStack(std::ostream& mips, int c){
        stackOffset+=2*c;
        mips << "\taddiu\t$sp,$sp,"<< -8*c <<"\n";
}

std::string Context::loadWithOffset(std::string name, std::ostream& mips, int registerNo){
        std::string tmp = name + std::to_string(1);
        if(LocalVarBindings.find(tmp) != LocalVarBindings.end()){
                int offset = FindLocalVariable(name,1);
                return (std::to_string(offset)+"($fp)");
        }
        else{
                mips << "\tlw\t\t$" << registerNo << ",%got(" << name << ")($28)\n";
                return ("0($" + std::to_string(registerNo) + ")");
        }
}

void Context::AddBeginLabel(std::string name){
        BeginLabels.push_back(name);
}

void Context::AddEndLabel(std::string name){
        EndLabels.push_back(name);
}

std::string Context::GetBeginLabel(){
        std::string temp = BeginLabels.back();
        BeginLabels.pop_back();
        return temp;
}

std::string Context::GetEndLabel(){
        std::string temp = EndLabels.back();
        EndLabels.pop_back();
        return temp;
}

