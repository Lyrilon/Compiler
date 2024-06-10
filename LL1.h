#pragma once
#pragma once
#include <iostream>
#include<vector>
#include<string>
using namespace std;
struct Token {
    string type;
    string value;
    int index;
    int row;
    int column;
};
class Production
{
    //产生式类
public:
    string left;
    vector<string> right;
    Production(const string& l, const vector<string>& r) : left(l), right(r) {}
};

class Grammar
{
public:
    //文法类
    vector<string> nonterminal;
    vector<string> terminal;
    vector<Production> productionList;
    string StartSymbol;
};

//文法
Grammar grammer;
//Token序列


/*
Program       -> StmtList
StmtList      -> Stmt StmtList | ε
Stmt          -> IfStmt | WhileStmt | AssignStmt | Block
IfStmt        -> 'if' '(' Expr ')' Stmt ElseIfList ElsePart
ElseIfList    -> ElseIf ElseIfList | ε
ElseIf        -> 'else if' '(' Expr ')' Stmt
ElsePart      -> 'else' Stmt | ε
WhileStmt     -> 'while' '(' Expr ')' Stmt
AssignStmt    -> IDENTIFIER '=' Expr ';'
Block         -> '{' StmtList '}'
Expr          -> Expr '||' Expr1 | Expr1
Expr1         -> Expr1 '&&' Expr2 | Expr2
Expr2         -> Expr2 '==' Expr3 | Expr2 '!=' Expr3 | Expr2 '<' Expr3 | Expr2 '<=' Expr3 | Expr2 '>' Expr3 | Expr2 '>=' Expr3 | Expr3
Expr3         -> Expr3 '+' Term | Expr3 '-' Term | Term
Term          -> Term '*' Factor | Term '/' Factor | Factor
Factor        -> '(' Expr ')' | IDENTIFIER | NUMBER
*/
vector<string> terminal =
{
    "if","else","while","IDENTIFIER","NUMBER","||","&&","==","!=","<","<=",">",">=","+","-","*","/","=","(",")","{","}"
};

vector<string> nonterminalList =
{
    "Program","StmtList","StmtListPrime","Stmt","IfStmt","ElseIfList","ElseIfListPrime","ElseIf","ElsePart","WhileStmt","AssignStmt","Block",
    "Expr","ExprPrime","Expr1","Expr1Prime","Expr2","Expr2Prime","Expr3","Expr3Prime","Term","TermPrime","Factor"
};

vector<Production> productionList = {
    Production("Program", {"StmtList"}),
    Production("StmtList", {"Stmt", "StmtListPrime"}),
    Production("StmtListPrime", {"Stmt", "StmtListPrime"}),
    Production("StmtListPrime", {}),
    Production("Stmt", {"IfStmt"}),
    Production("Stmt", {"WhileStmt"}),
    Production("Stmt", {"AssignStmt"}),
    Production("Stmt", {"Block"}),
    Production("IfStmt", {"if", "(", "Expr", ")", "Stmt", "ElseIfList", "ElsePart"}),
    Production("ElseIfList", {"ElseIf", "ElseIfListPrime"}),
    Production("ElseIfListPrime", {"ElseIf", "ElseIfListPrime"}),
    Production("ElseIfListPrime", {}),
    Production("ElseIfList", {}),
    Production("ElseIf", {"else if", "(", "Expr", ")", "Stmt"}),
    Production("ElsePart", {"else", "Stmt"}),
    Production("ElsePart", {}),
    Production("WhileStmt", {"while", "(", "Expr", ")", "Stmt"}),
    Production("AssignStmt", {"IDENTIFIER", "=", "Expr", ";"}),
    Production("Block", {"{", "StmtList", "}"}),
    Production("Expr", {"Expr1", "ExprPrime"}),
    Production("ExprPrime", {"||", "Expr1", "ExprPrime"}),
    Production("ExprPrime", {}),
    Production("Expr1", {"Expr2", "Expr1Prime"}),
    Production("Expr1Prime", {"&&", "Expr2", "Expr1Prime"}),
    Production("Expr1Prime", {}),
    Production("Expr2", {"Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {"==", "Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {"!=", "Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {"<", "Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {"<=", "Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {">", "Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {">=", "Expr3", "Expr2Prime"}),
    Production("Expr2Prime", {}),
    Production("Expr3", {"Term", "Expr3Prime"}),
    Production("Expr3Prime", {"+", "Term", "Expr3Prime"}),
    Production("Expr3Prime", {"-", "Term", "Expr3Prime"}),
    Production("Expr3Prime", {}),
    Production("Term", {"Factor", "TermPrime"}),
    Production("TermPrime", {"*", "Factor", "TermPrime"}),
    Production("TermPrime", {"/", "Factor", "TermPrime"}),
    Production("TermPrime", {}),
    Production("Factor", {"(", "Expr", ")"}),
    Production("Factor", {"IDENTIFIER"}),
    Production("Factor", {"NUMBER"})
};

/*
vector<Production> productionList = {
Production("Program", {"StmtList"}),
Production("StmtList", {"Stmt", "StmtList"}),
Production("StmtList", {}),
Production("Stmt", {"IfStmt"}),
Production("Stmt", {"WhileStmt"}),
Production("Stmt", {"AssignStmt"}),
Production("Stmt", {"Block"}),
Production("IfStmt", {"if", "(", "Expr", ")", "Stmt", "ElseIfList", "ElsePart"}),
Production("ElseIfList", {"ElseIf", "ElseIfList"}),
Production("ElseIfList", {}),
Production("ElseIf", {"else if", "(", "Expr", ")", "Stmt"}),
Production("ElsePart", {"else", "Stmt"}),
Production("ElsePart", {}),
Production("WhileStmt", {"while", "(", "Expr", ")", "Stmt"}),
Production("AssignStmt", {"IDENTIFIER", "=", "Expr", ";"}),
Production("Block", {"{", "StmtList", "}"}),
Production("Expr", {"Expr1", "Expr'"}),
Production("Expr'", {"||", "Expr1", "Expr'"}),
Production("Expr'", {}),
Production("Expr1", {"Expr2", "Expr1'"}),
Production("Expr1'", {"&&", "Expr2", "Expr1'"}),
Production("Expr1'", {}),
Production("Expr2", {"Expr3", "Expr2'"}),
Production("Expr2'", {"==", "Expr3", "Expr2'"}),
Production("Expr2'", {"!=", "Expr3", "Expr2'"}),
Production("Expr2'", {"<", "Expr3", "Expr2'"}),
Production("Expr2'", {"<=", "Expr3", "Expr2'"}),
Production("Expr2'", {">", "Expr3", "Expr2'"}),
Production("Expr2'", {">=", "Expr3", "Expr2'"}),
Production("Expr2'", {}),
Production("Expr3", {"Term", "Expr3'"}),
Production("Expr3'", {"+", "Term", "Expr3'"}),
Production("Expr3'", {"-", "Term", "Expr3'"}),
Production("Expr3'", {}),
Production("Term", {"Factor", "Term'"}),
Production("Term'", {"*", "Factor", "Term'"}),
Production("Term'", {"/", "Factor", "Term'"}),
Production("Term'", {}),
Production("Factor", {"(", "Expr", ")"}),
Production("Factor", {"IDENTIFIER"}),
Production("Factor", {"NUMBER"})
};
*/

string StartSymbol = "Program";