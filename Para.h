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
    //����ʽ��
public:
    string left;
    vector<string> right;
    Production(const string& l, const vector<string>& r) : left(l), right(r) {}
};

class Grammar
{
public:
    //�ķ���
    vector<string> nonterminal;
    vector<string> terminal;
    vector<Production> productionList;
    string StartSymbol;
};

//�ķ�
Grammar grammer;
//Token����


/*
Program       -> StmtList
StmtList      -> Stmt StmtList | ��
Stmt          -> IfStmt | WhileStmt | AssignStmt | Block
IfStmt        -> 'if' '(' Expr ')' Stmt ElseIfList ElsePart
ElseIfList    -> ElseIf ElseIfList | ��
ElseIf        -> 'else if' '(' Expr ')' Stmt
ElsePart      -> 'else' Stmt | ��
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
    "Program","StmtList","Stmt","IfStmt","ElseIfList","ElseIf","ElsePart","WhileStmt","AssignStmt","Block",
    "Expr","Expr1","Expr2","Expr3","Term","Factor"
};
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
    Production("Expr", {"Expr", "||", "Expr1"}),
    Production("Expr", {"Expr1"}),
    Production("Expr1", {"Expr1", "&&", "Expr2"}),
    Production("Expr1", {"Expr2"}),

    Production("Expr2", {"Expr2", "==", "Expr3"}),
    Production("Expr2", {"Expr2", "!=", "Expr3"}),
    Production("Expr2", {"Expr2", "<", "Expr3"}),
    Production("Expr2", {"Expr2", "<=", "Expr3"}),
    Production("Expr2", {"Expr2", ">", "Expr3"}),
    Production("Expr2", {"Expr2", ">=", "Expr3"}),
    
    Production("Expr2", {"Expr3"}),

    Production("Expr3", {"Expr3", "+", "Term"}),
    Production("Expr3", {"Expr3", "-", "Term"}),
    
    Production("Expr3", {"Term"}),
    Production("Term", {"Term", "*", "Factor"}),
    Production("Term", {"Term", "/", "Factor"}),
    Production("Term", {"Factor"}),
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