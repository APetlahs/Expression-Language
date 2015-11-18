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
    if (!programNode) {
        return 1;
    }
    EvalVisitor v;
    programNode->accept(&v);
    bool error = v.error;
    programNode->deleteAll();
    delete programNode;

    if (error) return 2;
    return 0;
}