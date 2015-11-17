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
        ast::ParamsNode *params;
        ast::DefNode *def;
        ast::BinExprNode *binExpr;
        ast::UniExprNode *uniExpr;
        ast::AssignNode *assign;
        ast::StmtNode *stmt;
        std::string *str;
        double num;
        int token;
}

%token <str> IDENT
%token <num> INT FLOAT
%token <token> END_OF_FILE
%token LET

%type <stmt> statement
%type <expr> expr terminal
%type <uniExpr> uni_expr
%type <binExpr> bin_expr
%type <call> func_call
%type <params> parameter_list parameter_loop
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
        | end_file              { programNode = new ModuleNode( new BlockNode() ); YYACCEPT; }
        ;

code_block:
        statement               { $$ = new BlockNode(); $$->push($1); }
        | end /* empty stmt*/   { $$ = new BlockNode(); }
        | code_block statement  { $1->push($2); }
        | code_block end
        ;

statement:
        expr end                { $$ = new StmtNode($1); }
        | assignment end        { $$ = new StmtNode($1); }
        | func_def end          { $$ = new StmtNode($1); }
        ;

assignment:
        LET IDENT '=' expr      { $$ = new AssignNode(new IdNode($2), $4); }
        ;

terminal:
        INT                     { $$ = new NumNode($1); }
        | FLOAT                 { $$ = new NumNode($1); }
        | IDENT                 { $$ = new IdNode($1); }
        ;

expr:
        terminal                { $$ = $1; }
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
        LET IDENT '(' parameter_list ')'
        '=' expr                { $$ = new DefNode(new IdNode($2), $4, $7); }
        ;

func_call:
        IDENT '(' args_list ')' { $$ = new CallNode(new IdNode($1), $3); }
        ;

parameter_list:
        parameter_loop          { $$ = $1; }
        | /* empty args */      { $$ = new ParamsNode(); }
        ;

parameter_loop:
        IDENT                           { $$ = new ParamsNode(); $$->push(new IdNode($1)); }
        | parameter_loop ',' IDENT      { $1->push(new IdNode($3)); $$ = $1; }
        ;

args_list:
        args_loop               { $$ = $1; }
        | /* empty args */      { $$ = new ArgsNode(); }
        ;

args_loop:
        expr                    { $$ = new ArgsNode(); $$->push($1); }
        | args_loop ',' expr    { $1->push($3); $$ = $1; }
        ;


end:
        end_delimeter
        ;

end_delimeter:
        '\n'
        | ';'
        ;

end_file:
        END_OF_FILE
        ;

%%