#include <cmath>
#include "eval_visitor.hpp"
#include "node.hpp"
using namespace std;
using namespace ast;

void EvalVisitor::addSymbol(const string &sym, ASTNode *val) {
    this->symbols[sym] = val;
}

ASTNode *EvalVisitor::getSymbol(const string &sym) {
    if (this->symbols.count(sym) > 0) {
        return this->symbols[sym];
    }
    return NULL;
}

int EvalVisitor::evalExpr(ExprNode *expr) {
    expr->accept(this);
    return this->curVal;
}

void EvalVisitor::visit(ast::ASTNode *node) {
    node->accept(this);
}

void EvalVisitor::visit(ExprNode *node) {
    this->isPrintable = true;
    node->accept(this);
}

void EvalVisitor::visit(BinExprNode *node) {
    this->isPrintable = true;
    int lhs;
    int rhs;
    node->lhs->accept(this);
    lhs = this->curVal;
    node->rhs->accept(this);
    rhs = this->curVal;
    switch (node->op) {
        case ADD:
            this->curVal = lhs + rhs;
            break;
        case SUB:
            this->curVal = lhs - rhs;
            break;
        case MUL:
            this->curVal = lhs * rhs;
            break;
        case DIV:
            this->curVal = lhs / rhs;
            break;
        case POW:
            this->curVal = pow(lhs, rhs);
            break;
        default:
            this->curVal = 9999;
    }
}

void EvalVisitor::visit(UniExprNode *expr) {
    this->isPrintable = true;
    int val;
    expr->expr->accept(this);
    val = this->curVal;
    switch( expr->op ) {
        case ABS:
            this->curVal = (val > 0) ? val : -1 * val;
            break;
        default:
            this->curVal = val;
    }
}

void EvalVisitor::visit(IdNode *id) {
    this->isPrintable = true;
    ASTNode *expr = this->getSymbol(*(id->id));
    if (expr == NULL) {
        std::cout << "symbol '" << *(id->id) << "' has not been defined" << std::endl;
    } else {
        expr->accept(this);
    }
}

void EvalVisitor::visit(IntNode *node) {
    this->isPrintable = true;
    this->curVal = node->val;
}

void EvalVisitor::visit(ModuleNode *node) {
    node->block->accept(this);
}

void EvalVisitor::visit(BlockNode *node) {
    for (std::vector<StmtNode*>::iterator i = node->stmts.begin(); i != node->stmts.end(); ++i)
    {
        (*i)->accept(this);
        if (this->isPrintable) {
            std::cout << this->curVal << std::endl;
        }
    }
}

void EvalVisitor::visit(StmtNode *node) {
    node->stmt->accept(this);
}

void EvalVisitor::visit(DefNode *node) {
    function newFunc;
    newFunc.args = node->args;
    newFunc.body = node->body;
    const std::string sym = *(node->id->id);
    this->funcDefs[sym] = newFunc;
}

void EvalVisitor::visit(ParamsNode *node) {
    cout << "a param node!" << endl;
}

void EvalVisitor::visit(ArgsNode *node) {
    cout << "an args node!" << endl;
}

void EvalVisitor::visit(CallNode *node) {
    std::vector<ASTNode *> tempValues;
    std::map<std::string, ASTNode*> prevScope;

    if (this->funcDefs.find(*(node->id->id)) == this->funcDefs.end()) {
        std::cout << "function '" << *(node->id->id)
                  <<"' has not been defined" << std::endl;
        return;
    }

    function func = this->funcDefs[*(node->id->id)];
    if (func.args->args.size() != node->args->args.size()) {
        std::cout << "function '" << *(node->id->id) <<"' expected "
                  << func.args->args.size() << " arguments, got "
                  << node->args->args.size()
                  << " instead." << std::endl;
        return;
    }

    prevScope = this->symbols;
    for (unsigned int i = 0; i < node->args->args.size(); ++i)
    {
        const std::string sym = *(func.args->args[i]->id);

        // evaluate argument before passing to function
        node->args->args[i]->accept(this);
        IntNode *argValue = new IntNode(this->curVal);
        tempValues.push_back(argValue);
        addSymbol(sym, argValue);
    }

    // delete temporary values
    for (std::vector<ASTNode*>::iterator i = tempValues.begin(); i != tempValues.end(); ++i)
    {
        (*i)->deleteAll();
    }

    func.body->accept(this);
    this->symbols = prevScope;
}

void EvalVisitor::visit(AssignNode *node) {
    const std::string *sym = node->id->id;
    this->addSymbol(*sym, node->expr);
}