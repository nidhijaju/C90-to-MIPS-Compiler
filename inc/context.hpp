#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>


        class Context{
                private:
                        int uniqueLabelNumber=0;
                        
                        //Available registers
                        bool regs[32] = {0};

                        //mapping for variables
                        std::unordered_map<std::string, unsigned int> LocalVarBindings;
                        std::unordered_map<std::string, unsigned int> GlobalVarBindings;
                        
                        //vector for labels
                        std::vector<std::string> BeginLabels;
                        std::vector<std::string> EndLabels;

                public:
                        Context();

                        int LocalVarCount = 0;
                        int GlobalVarCount = 0;
                        int FunctionCount = 0;
                        int scopeCounter = 0;
                        int stackOffset = 0;

                        bool doingCalc = false;
                        bool getName =  false;
                        std::string uniqueLabel(std::string s);
                        std::string functionLabel(std::string s);
                        
                        std::string loadWithOffset(std::string name, std::ostream& mips, int registerNo);

                        //Global Variables
                        void addGlobalVariable(std::string name);
                        void deleteGlobalVariable(std::string name);
                        unsigned int FindGlobalVariable(std::string name);

                        //Local Variables
                        void addLocalVariable(std::string name, int scopeLevel);
                        void deleteLocalVariable(std::string name);
                        int FindLocalVariable(std::string name, int scopeLevel);

                        // int GetOffset();
                        int getRegister();
                        void clearRegister(int r);

                        //Updates the stack pointer
                        void updateStack(std::ostream& mips, int c);

                        //Labeles
                        void AddBeginLabel(std::string name);
                        void AddEndLabel(std::string name);
                        std::string GetBeginLabel();
                        std::string GetEndLabel();

                        std::string CaseReturn;
        };

#endif