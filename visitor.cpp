#include <sstream>
#include <iostream>

#include "visitor.hpp"
#include "node.hpp"

using namespace std;
using namespace ast;

Visitor::~Visitor() {}

void Visitor::visit(ASTNode *node) {
    cout << "visiting an ASTNode: " << node << endl;
}

void Visitor::visit(ModuleNode *node) {
    cout << "visiting a Module Node: " << node << endl;
    node->block->accept(this);
}

void Visitor::visit(BlockNode *node) {
    cout << "visiting a BlockNode: " << node << endl;
    for (std::vector<StmtNode*>::iterator i = node->stmts.begin();
            i != node->stmts.end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Visitor::visit(StmtNode *node) {
    cout << "visiting a StmtNode: " << node << endl;
    node->stmt->accept(this);
}

void Visitor::visit(DefNode *node) {
    cout << "visiting a DefNode: " << node << endl;
    node->id->accept(this);
    node->args->accept(this);
    node->body->accept(this);
}

void Visitor::visit(CallNode *node) {
    cout << "visiting a CallNode: " << node << endl;
    node->id->accept(this);
    node->args->accept(this);
}

void Visitor::visit(AssignNode *node) {
    cout << "visiting a AssignNode: " << node << endl;
    node->id->accept(this);
    node->expr->accept(this);
}

void Visitor::visit(ParamsNode *node) {
    cout << "visiting a ParamsNode: " << node << endl;
    for (vector<IdNode*>::iterator i = node->args.begin();
        i != node->args.end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Visitor::visit(ArgsNode *node) {
    cout << "visiting a ArgsNode: " << node << endl;
    for (vector<ExprNode*>::iterator i = node->args.begin();
        i != node->args.end(); ++i)
    {
        (*i)->accept(this);
    }
}

void Visitor::visit(UniExprNode *node) {
    cout << "visiting a UniExprNode: " << node << endl;
    cout << "operator: " << node->op << endl;
    node->expr->accept(this);
}

void Visitor::visit(BinExprNode *node) {
    cout << "visiting a BinExprNode: " << node << endl;
    node->lhs->accept(this);
    cout << "operator: " << node->op << endl;
    node->rhs->accept(this);
}

void Visitor::visit(IdNode *node) {
    cout << "visiting a IdNode: " << node << endl;
    cout << "label: " << *(node->id) << endl;
}

void Visitor::visit(IntNode *node) {
    cout << "visiting a IntNode: " << node << endl;
    cout << "value: " << node->val << endl;
}

void Visitor::visit(ExprNode *node) {
    cout << "visiting a ExprNode: " << node << endl;
}