%{
        #include <stdlib.h>
        #include <stdio.h>
        #include <iostream>
        #include "node.hpp"
        using namespace ast;
        using namespace std;

        ModuleNode *programNode;

        extern int yylex();
        void yyerror(const char *s) {
                fprintf(stderr, "Error: %s\n", s);
        }
%}

%union {
        ast::ASTNode *node;
        ast::ModuleNode *program;
        ast::BlockNode *block;
        ast::IdNode *ident;
        ast::ExprNode *expr;
        ast::CallNode *call;
        ast::ArgsNode *args;
        ast::DefNode *def;
        ast::BinExprNode *binExpr;
        ast::UniExprNode *uniExpr;
        ast::AssignNode *assign;
        ast::StmtNode *stmt;
        std::string *str;
        int ival;
        int token;
}

%token <str> IDENT
%token <ival> INT
%token <token> END_OF_FILE

%type <stmt> statement
%type <expr> expr
%type <uniExpr> uni_expr
%type <binExpr> bin_expr
%type <call> func_call
%type <args> args_list args_loop
%type <def> func_def
%type <assign> assignment
%type <block> code_block

%left '+' '-'
%left '*' '/'
%left '^'

%start program

%%

program:
        code_block end_file     { programNode = new ModuleNode($1); cout << "created a program node" << endl; YYACCEPT;}
        ;

code_block:
        statement               { $$ = new BlockNode(); $$->push($1); }
        | code_block statement  { $1->push($2); }
        ;

statement:
        expr end                { $$ = new StmtNode($1); }
        | assignment end        { $$ = new StmtNode($1); }
        | func_def end          { $$ = new StmtNode($1); }
        ;

assignment:
        IDENT '=' expr          { $$ = new AssignNode(new IdNode($1), $3); }
        ;

expr:
        INT                     { $$ = new IntNode($1); }
        | IDENT                 { $$ = new IdNode($1); }
        | func_call             { $$ = $1; }
        | bin_expr              { $$ = $1; }
        | uni_expr              { $$ = $1; }
        | '(' expr ')'          { $$ = $2; }
        ;

bin_expr:
        expr '+' expr           { $$ = new BinExprNode($1, ast::ADD, $3); }
        | expr '-' expr         { $$ = new BinExprNode($1, ast::SUB, $3); }
        | expr '*' expr         { $$ = new BinExprNode($1, ast::MUL, $3); }
        | expr '/' expr         { $$ = new BinExprNode($1, ast::DIV, $3); }
        | expr '^' expr         { $$ = new BinExprNode($1, ast::POW, $3); }
        ;

uni_expr:
        '-' expr                { $$ = new UniExprNode(ast::SUB, $2); }
        | '|' expr '|'          { $$ = new UniExprNode(ast::ABS, $2); }
        ;

func_def:
        IDENT '(' args_list ')'
        '=' expr                { $$ = new DefNode(new IdNode($1), $3, $6); }

func_call:
        IDENT '(' args_list ')' { $$ = new CallNode(new IdNode($1), $3); }
        ;

args_list:
        args_loop               { $$ = $1; }
        | /* empty args */      { $$ = new ArgsNode(); }
        ;

args_loop:
        IDENT                   { $$ = new ArgsNode(); $$->push(new IdNode($1)); }
        | args_loop ',' IDENT   { $1->push(new IdNode($3)); $$ = $1; }
        ;

end:
        end end_delimeter
        | end_delimeter

end_delimeter:
        '\n'
        | ';'

end_file:
        END_OF_FILE

%%