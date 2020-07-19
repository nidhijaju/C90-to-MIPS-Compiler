#include <iostream>
#include <fstream>
#include <string>
#include "ast.hpp"

int scopeLevel = 0;
std::vector<NodePtr> globalvar;
bool globalflag = false;

int main(int argc, char *argv[]){
        if(std::string(argv[1]) == "--translate" && std::string(argv[3]) == "-o"){
                BaseNode* ast = parseAST(argv[2]);
                std::string oname = argv[4];
                std::ofstream output(oname);               
                ast->py_print(output);
                output.close();

        }
        else if(std::string(argv[1]) == "-S" && std::string(argv[3]) == "-o"){
                BaseNode* ast = parseAST(argv[2]);
                std::string oname = argv[4];
                std::ofstream output(oname);
                Context* ctxt = new Context();               
                ast->mips_print(output,ctxt);
                output.close();
        }
        else{
                
                if(std::string(argv[3]) == "p"){
                        BaseNode* ast = parseAST(argv[1]);
                        std::string oname = argv[2];
                        oname += ".py";
                        std::ofstream output(oname);
                        ast->py_print(output);
                        output.close();
                }
                else{
                        BaseNode* ast = parseAST(argv[1]);
                        std::string oname = argv[2];
                        oname += ".s";
                        std::ofstream output(oname);
                        Context* ctxt = new Context();               
                        ast->mips_print(output,ctxt);
                        output.close();
                }

        }

        return 0;

}