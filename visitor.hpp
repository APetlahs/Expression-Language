#ifndef VISITOR_HPP
#define VISITOR_HPP

#include <iostream>
#include <map>

namespace ast {

class ASTNode;
class ModuleNode;
class BlockNode;
class StmtNode;
class DefNode;
class CallNode;
class AssignNode;
class ParamsNode;
class ArgsNode;
class UniExprNode;
class BinExprNode;
class IdNode;
class NumNode;
class ExprNode;

class Visitor {
public:
    virtual ~Visitor();
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