#include "node.hpp"
#include "visitor.hpp"
#include <iostream>
using namespace std;
using namespace ast;

void BlockNode::deleteAll() {
    for (vector<StmtNode*>::iterator i = stmts.begin(); i != stmts.end(); ++i) {
        (*i)->deleteAll();
    }
}

void ArgsNode::deleteAll() {
    for (vector<IdNode*>::iterator i = args.begin(); i != args.end(); ++i) {
        (*i)->deleteAll();
    }
}