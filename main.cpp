#include <iostream>
#include "node.hpp"
#include "visitor.hpp"
#include "eval_visitor.hpp"
using namespace std;
using namespace ast;

extern ast::ModuleNode* programNode;
extern int yyparse();

int main(int argc, char **argv) {
    yyparse();
    std::cout << programNode << std::endl;
    if (!programNode) {
        return 0;
    }
    EvalVisitor v;
    programNode->accept(&v);
    programNode->deleteAll();
    delete programNode;
    return 0;
}