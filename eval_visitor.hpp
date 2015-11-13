#ifndef _EVAL_VISITOR_HPP_
#define _EVAL_VISITOR_HPP_

#include <map>
#include "visitor.hpp"

namespace ast {

class EvalVisitor: public ast::Visitor {
private:
    std::map<std::string , ASTNode*> symbols;
    int curVal;
    bool isPrintable;

public:

    EvalVisitor(): symbols(), curVal(0), isPrintable(false) {}
    void addSymbol(const std::string &sym, ASTNode *val);
    ast::ASTNode *getSymbol(const std::string &sym);

    virtual void visit(ast::ASTNode *node);
    virtual void visit(ast::ModuleNode *node);
    virtual void visit(ast::BlockNode *node);
    virtual void visit(ast::StmtNode *node);
    virtual void visit(ast::DefNode *node);
    virtual void visit(ast::CallNode *node);
    virtual void visit(ast::AssignNode *node);
    virtual void visit(ast::ArgsNode *node);
    virtual void visit(ast::UniExprNode *node);
    virtual void visit(ast::BinExprNode *node);
    virtual void visit(ast::IdNode *node);
    virtual void visit(ast::IntNode *node);
    virtual void visit(ast::ExprNode *node);
};

}; // namespace ast

#endif