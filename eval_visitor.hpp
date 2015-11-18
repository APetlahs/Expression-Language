#ifndef _EVAL_VISITOR_HPP_
#define _EVAL_VISITOR_HPP_

#include <map>
#include "visitor.hpp"

namespace ast {

// struct to store function definitions
typedef struct {
    ast::ParamsNode* args;
    ast::ExprNode* body;
} function;

class EvalVisitor: public ast::Visitor {
private:
    std::map<std::string , ASTNode*> symbols;
    std::map<std::string , function> funcDefs;
    double curVal;
    bool isPrintable;

public:
    bool error;

    EvalVisitor(): symbols(), funcDefs(), curVal(0), isPrintable(false) {}
    void addSymbol(const std::string &sym, ASTNode *val);
    ast::ASTNode *getSymbol(const std::string &sym);
    double evalExpr(ExprNode *expr);

    virtual void visit(ast::ASTNode *node);
    virtual void visit(ast::ModuleNode *node);
    virtual void visit(ast::BlockNode *node);
    virtual void visit(ast::StmtNode *node);
    virtual void visit(ast::DefNode *node);
    virtual void visit(ast::CallNode *node);
    virtual void visit(ast::AssignNode *node);
    virtual void visit(ast::ParamsNode *node);
    virtual void visit(ast::ArgsNode *node);
    virtual void visit(ast::UniExprNode *node);
    virtual void visit(ast::BinExprNode *node);
    virtual void visit(ast::IdNode *node);
    virtual void visit(ast::NumNode *node);
    virtual void visit(ast::ExprNode *node);
};

}; // namespace ast

#endif