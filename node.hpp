#ifndef AST_NODE_HPP
#define AST_NODE_HPP

#include <string>
#include <vector>
#include "visitor.hpp"

namespace ast {

enum Operator {ADD,SUB,MUL,DIV,POW,ABS};

// accept method for visitor
#define ACCEPT() virtual void accept(ast::Visitor *v) { v->visit(this); }

class ASTNode {
public:
        virtual ~ASTNode() {}
        // this method deletes all child nodes as well.
        virtual void deleteAll() = 0;
        ACCEPT();
};

class ExprNode: public ASTNode {
public:
        virtual ~ExprNode() {}
        ACCEPT();
};

class NumNode: public ExprNode {
public:
        double val;
        NumNode(const double val): val(val) {}
        virtual void deleteAll() {}
        ACCEPT();
};

class IdNode: public ExprNode {
public:
        std::string *id;
        IdNode(std::string *id): id(id) {}
        virtual void deleteAll() { delete id; }
        ACCEPT();
};

class BinExprNode: public ExprNode {
public:
        ASTNode *lhs;
        Operator op;
        ASTNode *rhs;
        BinExprNode( ASTNode *lhs, Operator op, ASTNode *rhs ):
                lhs(lhs), op(op), rhs(rhs) {}
        virtual void deleteAll() { lhs->deleteAll(); rhs->deleteAll(); }
        ACCEPT();
};

class UniExprNode: public ExprNode {
public:
        ExprNode *expr;
        Operator op;
        UniExprNode( Operator op, ExprNode *expr ):
                expr(expr), op(op) {}
        virtual void deleteAll() { expr->deleteAll(); }
        ACCEPT();
};

class StmtNode: public ASTNode {
public:
        ASTNode *stmt;
        StmtNode(ASTNode *stmt): stmt(stmt) {}
        virtual void deleteAll() { stmt->deleteAll(); }
        ACCEPT();
};

class BlockNode: public ASTNode {
public:
        std::vector<StmtNode*> stmts;
        BlockNode(): stmts() {}
        void push(StmtNode *stmt) { stmts.push_back(stmt); }
        virtual void deleteAll();
        ACCEPT();
};

class ParamsNode: public ASTNode {
public:
        std::vector<IdNode*> args;
        ParamsNode(): args() {}
        void push(IdNode *node) { args.push_back(node); }
        virtual void deleteAll();
        ACCEPT();
};

class ArgsNode: public ASTNode {
public:
        std::vector<ExprNode*> args;
        ArgsNode(): args() {}
        void push(ExprNode *node) { args.push_back(node); }
        virtual void deleteAll();
        ACCEPT();
};

class CallNode: public ExprNode {
public:
        IdNode *id;
        ArgsNode *args;
        CallNode(IdNode *id, ArgsNode *args): id(id), args(args) {};
        virtual void deleteAll() { id->deleteAll(); args->deleteAll(); }
        ACCEPT();
};

class DefNode: public ASTNode {
public:
        IdNode *id;
        ParamsNode *args;
        ExprNode *body;

        DefNode(IdNode *id, ParamsNode *args, ExprNode *body):
                id(id), args(args), body(body) {}
        virtual void deleteAll() {
            id->deleteAll(); args->deleteAll(); body->deleteAll();
        }
        ACCEPT();
};

class AssignNode: public ASTNode {
public:
        IdNode *id;
        ExprNode *expr;

        AssignNode(IdNode *id, ExprNode *expr): id(id), expr(expr) {}
        virtual void deleteAll() {
            id->deleteAll(); expr->deleteAll();
        }
        ACCEPT();
};

// enclosing class
class ModuleNode: public ASTNode {
public:
        BlockNode *block;
        ModuleNode(BlockNode *block): block(block) {}
        virtual void deleteAll() { block->deleteAll(); }
        ACCEPT();
};

}; // namespace ast

#endif