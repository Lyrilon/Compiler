#include"LL1.h"
#include <iostream>
#include<map>
#include <algorithm>
#include<unordered_map>
#include <numeric> 
#include<set>
using namespace std;
map<string, set<string>> firstSets;
map<string, set<string>> followSets;
map<Production, set<string>> selectSets;

void printProduction(const Production& production);


void eliminateLeftRecursion() {
    vector<Production> newProductions; // 存储新的产生式集合
    for (const string& nonterminal : nonterminalList) {
        vector<Production> alpha; // 存储左递归产生式的右部
        vector<Production> beta;  // 存储非左递归产生式的右部
        for (auto& prod : productionList) {
            if (prod.left == nonterminal) {
                if (!prod.right.empty() && prod.right[0] == nonterminal) {
                    // 提取左递归部分，创建新的产生式右部
                    alpha.push_back(Production(nonterminal + "*", vector<string>(prod.right.begin() + 1, prod.right.end())));
                }
                else {
                    // 非左递归部分，直接加入beta
                    beta.push_back(prod);
                }
            }
        }
        if (!alpha.empty()) {
            for (auto& prod : beta) {
                // 对于每个非左递归产生式，附加新的非终结符
                prod.right.push_back(nonterminal + "*");
                newProductions.push_back(prod);
            }
            for (auto& prod : alpha) {
                // 对于每个左递归产生式，附加新的非终结符和空的vector表示ε
                prod.right.push_back(nonterminal + "*");
                newProductions.push_back(prod);
            }
            // 添加一条新的产生式，表示新的非终结符可以推导出空
            newProductions.push_back(Production(nonterminal + "*", {})); // 空的vector表示ε
            // 将新的非终结符加入非终结符列表
            nonterminalList.push_back(nonterminal + "*");
        }
        else {
            // 如果没有左递归产生式，直接加入原来的非左递归产生式
            for (auto& prod : beta) {
                newProductions.push_back(prod);
            }
        }
    }
    // 更新产生式列表
    productionList = newProductions;
}



void leftFactoring() {
    vector<Production> newProductions; // 新的产生式集合
    unordered_map<string, vector<vector<string>>> prefixMap; // 用于存储每个非终结符号对应的产生式右部

    // 将每个产生式按其左部非终结符分类
    for (auto& prod : productionList) {
        prefixMap[prod.left].push_back(prod.right);
    }

    // 对每个非终结符号的产生式进行左公因子提取
    for (auto& entry : prefixMap) {
        //当前左部
        string nonterminal = entry.first;
        //当前左部的产生式右部
        vector<vector<string>> rights = entry.second;

        while (true) {
            unordered_map<string, vector<vector<string>>> factorMap; // 用于存储具有相同前缀的产生式
            
            //处理当前非终结符号的所有产生式，把相同前缀的串一串
            //有可能本就只有一个右部，这时候不需要处理
            
            for (vector<string>& right : rights) {
                //这个的right为产生式右部，类型是vector<string>
                //即便是空串也要处理
                if (!right.empty()) {
                    factorMap[right[0]].push_back(vector<string>(right.begin() + 1, right.end())); // 按第一个符号分类
                } 
                else {
					factorMap[""].push_back(vector<string>()); // 空串
				}
            }

            // 如果表尺寸没有变化，说明没有公共前缀
            if (factorMap.size() == rights.size()) {
                // 没有公共前缀，新产生式列表加入原产生式
                for (auto& right : rights) {
                    newProductions.push_back(Production(nonterminal, right)); // 添加原产生式
                }
                break; // 跳出循环
            }
            //存在公共前缀的话
            else {
                //需要增加一个中间非终结符号
                string newNonterminal = nonterminal + "'"; // 新的非终结符号
                //新的非终结符号加入非终结符号列表
                nonterminalList.push_back(newNonterminal); // 将新非终结符号加入非终结符列表
                //新的产生式列表加入新的产生式
                vector<vector<string>> newRights; // 存储新的产生式右部

                // 遍历具有相同前缀的产生式
                for (auto& entry : factorMap) 
                {
                    if (entry.second.size() > 1) { // 如果存在多个产生式具有相同前缀
                        //这里newRights存新的产生式右部，第一个公共前缀和一个中间非终结符号的组合
                        newRights.push_back({ entry.first, newNonterminal }); // 创建新的产生式右部
                        for (auto& right : entry.second) {
                            newProductions.push_back(Production(newNonterminal, right)); // 创建新的产生式
                        }
                    }
                    //如果不存在多个产生式具有相同前缀
                    else 
                    {
                        newRights.push_back({ entry.first }); // 保留原产生式
                    }
                }
                // 更新产生式右部
                rights = newRights; // 更新产生式右部
            }
        }
    }
    productionList = newProductions; // 更新产生式列表
}


/*
vector<Production> eliminateLeftRecursion(const vector<Production>& productions) {
    vector<Production> newProductions;
    map<string, vector<vector<string>>> productionMap;

    // Group productions by their left-hand side
    for (const auto& prod : productions) {
        productionMap[prod.left].push_back(prod.right);
    }

    // Eliminate left recursion for each non-terminal
    for (const auto& entry : productionMap) {
        const string& A = entry.first;
        const auto& rules = entry.second;

        vector<vector<string>> alpha;
        vector<vector<string>> beta;

        // Separate rules into left-recursive and non-left-recursive
        for (const auto& rule : rules) {
            if (!rule.empty() && rule[0] == A) {
                alpha.push_back(vector<string>(rule.begin() + 1, rule.end()));
            }
            else {
                beta.push_back(rule);
            }
        }

        if (!alpha.empty()) {
            string A_prime = A + "'";

            // Create new productions without left recursion
            for (const auto& b : beta) {
                vector<string> newRule(b);
                newRule.push_back(A_prime);
                newProductions.push_back(Production(A, newRule));
            }

            for (const auto& a : alpha) {
                vector<string> newRule(a);
                newRule.push_back(A_prime);
                newProductions.push_back(Production(A_prime, newRule));
            }

            newProductions.push_back(Production(A_prime, vector<string>()));
        }
        else {
            // No left recursion, add original productions
            for (const auto& rule : rules) {
                newProductions.push_back(Production(A, rule));
            }
        }
    }

    return newProductions;
}
*/

/*
vector<Production> extractLeftFactor(const vector<Production>& productions) {
    vector<Production> newProductions;
    unordered_map<string, vector<vector<string>>> productionMap;

    // Group productions by their left-hand side
    for (const auto& prod : productions) {
        productionMap[prod.left].push_back(prod.right);
    }

    // Process each non-terminal
    for (const auto& entry : productionMap) {
        const string& A = entry.first;
        const auto& rules = entry.second;

        unordered_map<string, vector<vector<string>>> prefixMap;
        vector<string> longestCommonPrefix;

        // Find the longest common prefix for each pair of rules
        for (size_t i = 0; i < rules.size(); ++i) {
            for (size_t j = i + 1; j < rules.size(); ++j) {
                vector<string> prefix;
                size_t k = 0;
                while (k < rules[i].size() && k < rules[j].size() && rules[i][k] == rules[j][k]) {
                    prefix.push_back(rules[i][k]);
                    ++k;
                }
                if (!prefix.empty()) {
                    string prefixKey = accumulate(prefix.begin(), prefix.end(), string(""));
                    prefixMap[prefixKey].push_back(rules[i]);
                    prefixMap[prefixKey].push_back(rules[j]);
                    if (prefix.size() > longestCommonPrefix.size()) {
                        longestCommonPrefix = prefix;
                    }
                }
            }
        }

        // Extract left factor
        if (!longestCommonPrefix.empty()) {
            string A_prime = A + "'";

            // Add new production for A
            vector<string> newRule(longestCommonPrefix);
            newRule.push_back(A_prime);
            newProductions.push_back(Production(A, newRule));

            // Add new productions for A'
            for (const auto& rule : rules) {
                if (equal(longestCommonPrefix.begin(), longestCommonPrefix.end(), rule.begin())) {
                    vector<string> newRight(rule.begin() + longestCommonPrefix.size(), rule.end());
                    if (newRight.empty()) {
                        newRight.push_back("ε");
                    }
                    newProductions.push_back(Production(A_prime, newRight));
                }
                else {
                    newProductions.push_back(Production(A, rule));
                }
            }
        }
        else {
            // No common prefix, add original productions
            for (const auto& rule : rules) {
                newProductions.push_back(Production(A, rule));
            }
        }
    }

    return newProductions;
}

*/
void deleteEpsilon(map<Production, set<string>>& selectSets);

// 判断是否为非终结符号
bool isNonTerminal(const string& symbol, const vector<string>& nonterminals) {
    return find(nonterminals.begin(), nonterminals.end(), symbol) != nonterminals.end();
}

// 计算FIRST集
void computeFirstSets(const vector<string>& nonterminals, const vector<Production>& productions, map<string, set<string>>& firstSets) {
    //建立每个非终结符号的FIRST集
    for (const auto& nonterminal : nonterminals) {
        firstSets[nonterminal] = set<string>();
    }

    // Helper function to add FIRST set from source to destination and check for changes
    auto addFirstSet = [](set<string>& dest, const set<string>& src) {
        bool changed = false;
        for (const auto& symbol : src) {
            if (dest.find(symbol) == dest.end()) {
                dest.insert(symbol);
                changed = true;
            }
        }
        return changed;
        };

    bool changed;
    do {
        changed = false;
        // 对每个产生式进行处理
        for (const auto& production : productions) {
            //拿到产生式的左部和右部
            const string& left = production.left;
            const vector<string>& right = production.right;
            //如果右部为空，说明是可为ε产生式
            if (right.empty()) {
                // 如果当前非终结符号没有ε，加入到FIRST集中，判断存不存在是防止重复加入
                if (firstSets[left].find("ε") == firstSets[left].end()) {
                    firstSets[left].insert("ε");
                    changed = true;
                }
            }
            else {
                //如果右部第一个符号是终结符号，直接加入到FIRST集中
                for (size_t i = 0; i < right.size(); ++i) 
                {
                    const string& symbol = right[i];
                    //如果是非终结符号
                    if (isNonTerminal(symbol, nonterminals)) 
                    {
                        //把右边非终结符号的FIRST集加入到左部的FIRST集中
                        set<string> symbolFirstSet = firstSets[symbol];
                        //当前非终结符号的FIRST集中是否有ε，如果有循环继续，没有直接退出循环
                        bool hasEpsilon = (symbolFirstSet.find("ε") != symbolFirstSet.end());
                        //只有右部全都可为ε，才能把ε加入到左部的FIRST集中
                        symbolFirstSet.erase("ε");
                        //把右部非终结符号的FIRST集加入到左部的FIRST集中，这里addFirstSet函数返回true说明有变化
                        if (addFirstSet(firstSets[left], symbolFirstSet)) {
                            changed = true;
                        }
                        //如果右部非终结符号的FIRST集中没有ε，直接退出循环
                        if (!hasEpsilon) {
                            break;
                        }
                    }
                    //如果是终结符号，直接加入到左部的FIRST集中
                    else 
                    {
                        //如果当前非终结符号的FIRST集中没有这个终结符号，插入到FIRST集中
                        if (firstSets[left].find(symbol) == firstSets[left].end()) {
                            firstSets[left].insert(symbol);
                            changed = true;
                        }
                        break;
                    }
                    //查到最后一个符号，如果右部所有符号都能推出ε，那么把ε加入到左部的FIRST集中
                    if (i == right.size() - 1) 
                    {
                        //如果右部所有符号都能推出ε，那么把ε加入到左部的FIRST集中
                        if (firstSets[left].find("ε") == firstSets[left].end()) 
                        {
                            firstSets[left].insert("ε");
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
}
//打印FIRST集
void printFirstSets(const map<string, set<string>>& firstSets) {
    for (const auto& pair : firstSets) {
		cout << "FIRST(" << pair.first << ") =\t ";
        for (const auto& symbol : pair.second) {
			cout << symbol << " ";
		}
		cout << endl;
	}
}
//打印Follow集合
void printFollowSets(const map<string, set<string>>& followSets) {
    for (const auto& pair : followSets) {
		cout << "FOLLOW(" << pair.first << ") =\t ";
        for (const auto& symbol : pair.second) {
			cout << symbol << " ";
		}
		cout << endl;
	}
}



/*
Program:
FIRST(Program) = { if, while, IDENTIFIER, { }

StmtList:
FIRST(StmtList) = { if, while, IDENTIFIER, {, ε }

Stmt:
FIRST(Stmt) = { if, while, IDENTIFIER, { }

IfStmt:
FIRST(IfStmt) = { if }

ElseIfList:
FIRST(ElseIfList) = { else if, ε }

ElseIf:
FIRST(ElseIf) = { else if }

ElsePart:
FIRST(ElsePart) = { else, ε }

WhileStmt:
FIRST(WhileStmt) = { while }

AssignStmt:
FIRST(AssignStmt) = { IDENTIFIER }

Block:
FIRST(Block) = { { }

Expr:
FIRST(Expr) = { (, IDENTIFIER, NUMBER }

Expr1:
FIRST(Expr1) = { (, IDENTIFIER, NUMBER }

Expr2:
FIRST(Expr2) = { (, IDENTIFIER, NUMBER }

Expr3:
FIRST(Expr3) = { (, IDENTIFIER, NUMBER }

Term:
FIRST(Term) = { (, IDENTIFIER, NUMBER }

Factor:
FIRST(Factor) = { (, IDENTIFIER, NUMBER }

*/
//计算FOLLOW集

void computeFollowSets(const vector<string>& nonterminals, const vector<Production>& productions, const map<string, set<string>>& firstSets, map<string, set<string>>& followSets, const string& startSymbol) {
    // 初始化每个非终结符的 FOLLOW 集合为空集
    for (const auto& nonterminal : nonterminals) {
        followSets[nonterminal] = set<string>();
    }

    // 将 $ 加入开始符号的 FOLLOW 集合
    followSets[startSymbol].insert("$");

    bool changed;
    do {
        changed = false;
        // 遍历所有产生式
        for (const auto& production : productions) {
            const string& left = production.left; // 获取产生式左部
            const vector<string>& right = production.right; // 获取产生式右部

            // 遍历产生式右部的每个符号
            for (size_t i = 0; i < right.size(); ++i) {
                const string& symbol = right[i]; // 获取右部的第 i 个符号
                // 如果是非终结符号
                if (isNonTerminal(symbol, nonterminals)) {
                    size_t j = i + 1;
                    bool epsilonFound = true;
                    // epsilonFound 初始化为 true，如果右部所有符号都能推出 ε，epsilonFound 为 true
                    while (j < right.size() && epsilonFound) {
                        epsilonFound = false;
                        const string& nextSymbol = right[j]; // 获取右部的下一个符号

                        if (isNonTerminal(nextSymbol, nonterminals)) 
                        {
                            // 如果下一个符号是非终结符号，将其 FIRST 集合中的元素添加到 symbol 的 FOLLOW 集合中
                            const auto& firstSetNext = firstSets.at(nextSymbol);
                            //遍历firstSetNext中的每一个元素
                            for (const auto& firstSymbol : firstSetNext) {
                                //如果不是ε，就把firstSymbol加入到symbol的FOLLOW集合中
                                if (firstSymbol != "ε") {
                                    // 将 nextSymbol 的 FIRST 集合中的元素添加到 symbol 的 FOLLOW 集合中
                                    //在 C++ 的标准库 set 中，insert 方法返回一个 pair。这个 pair 的第一
                                    // 个元素是一个迭代器，指向插入元素的位置；第二个元素是一个布尔
                                    // 值，表示插入操作是否成功（健壮性，鲁棒性）
                                    if (followSets[symbol].insert(firstSymbol).second) {
                                        changed = true; // 标记为 true，表示有新元素被添加
                                    }
                                }

                                else 
                                {
                                    epsilonFound = true; // 如果 FIRST 集合中有 ε，继续检查下一个符号
                                }
                            }
                        }
                        else 
                        {
                            // 如果下一个符号是终结符号，直接将其添加到 symbol 的 FOLLOW 集合中
                            if (followSets[symbol].insert(nextSymbol).second) {
                                changed = true; // 标记为 true，表示有新元素被添加
                            }
                            epsilonFound = false; // 终结符号后面的符号不再检查
                            break;
                        }
                        j++;
                    }

           
                    if (epsilonFound) 
                    {
                        const auto& followSetLeft = followSets.at(left);
                        for (const auto& followSymbol : followSetLeft) 
                        {
                            // 将 left 的 FOLLOW 集合中的元素添加到 symbol 的 FOLLOW 集合中
                            if (followSets[symbol].insert(followSymbol).second) {
                                changed = true; // 标记为 true，表示有新元素被添加
                            }
                        }
                    }
                }
            }
        }
    } while (changed); // 当有新元素被添加时，继续循环
}

void computeSelectSets( const map<string, set<string>>& firstSets, const map<string, set<string>>& followSets, map<Production, set<string>>& selectSets,vector<Production>productionList) 
{
    //遍历产生式
    for (const auto& production : productionList) 
    {
        const string& left = production.left;
        const vector<string>& right = production.right;

        set<string> selectSet;
        //如果为空产生式，直接把FOLLOW集加入到SELECT集中
        if (right.empty()) {
            selectSet.insert(followSets.at(left).begin(), followSets.at(left).end());
        }
        else {
            //不是空产生式
            bool containsEpsilon = true;

            for (const auto& symbol : right) 
            {
                //如果首符是非终结符号
                if (isNonTerminal(symbol, nonterminalList)) 
                {
                    //拿到非终结符号的FIRST集
                    const auto& firstSet = firstSets.at(symbol);
          		  //把非终结符号的FIRST集加入到SELECT集中
                    selectSet.insert(firstSet.begin(), firstSet.end());
                    //如果非终结符号的FIRST集中没有ε，直接退出循环
                    if (firstSet.find("ε") == firstSet.end()) 
                    {
                        containsEpsilon = false;
                        break;
                    }
                }
                else 
                {
                    selectSet.insert(symbol);
                    containsEpsilon = false;
                    break;
                }
            }
            //如果右部所有符号都能推出ε，那么把左部FOLLOW集加入到SELECT集中
            if (containsEpsilon) {
                selectSet.insert(followSets.at(left).begin(), followSets.at(left).end());
            }
        }
        selectSets[production] = selectSet;
    }
    //这里删除SELECT集中的ε，原因是ε不是终结符号，不会出现在输入串中，并且ε不会影响SELECT集的计算
    deleteEpsilon(selectSets);
}


bool isLL1Grammar(const vector<Production>& productions, const vector<string>& nonterminals, const string& startSymbol) {
    map<string, set<string>> firstSets;
    map<string, set<string>> followSets;

    // Compute FIRST and FOLLOW sets
    computeFirstSets(nonterminals, productions, firstSets);
    computeFollowSets(nonterminals,productionList,firstSets,followSets,startSymbol);

    // Check FIRST set condition
    for (const auto& nonterminal : nonterminals) {
        set<string> firstSet;
        vector<vector<string>> rules;

        for (const auto& prod : productions) {
            if (prod.left == nonterminal) {
                rules.push_back(prod.right);
            }
        }

        for (size_t i = 0; i < rules.size(); ++i) {
            set<string> firstAlpha;
            size_t j = 0;
            bool epsilonFound = true;
            while (j < rules[i].size() && epsilonFound) {
                epsilonFound = false;
                const string& symbol = rules[i][j];
                if (find(nonterminals.begin(), nonterminals.end(), symbol) != nonterminals.end()) {
                    const auto& firstSetSymbol = firstSets[symbol];
                    firstAlpha.insert(firstSetSymbol.begin(), firstSetSymbol.end());
                    if (firstSetSymbol.find("ε") != firstSetSymbol.end()) {
                        epsilonFound = true;
                    }
                }
                else {
                    firstAlpha.insert(symbol);
                    epsilonFound = false;
                }
                ++j;
            }
            if (epsilonFound) {
                firstAlpha.insert("ε");
            }

            for (const auto& symbol : firstAlpha) {
                if (firstSet.find(symbol) != firstSet.end()) {
                    return false;
                }
                firstSet.insert(symbol);
            }
        }
    }

    // Check FOLLOW set condition
    for (const auto& nonterminal : nonterminals) {
        for (const auto& prod : productions) {
            if (prod.left == nonterminal) {
                const auto& right = prod.right;
                bool epsilonFound = true;
                for (const auto& symbol : right) {
                    epsilonFound = false;
                    if (find(nonterminals.begin(), nonterminals.end(), symbol) != nonterminals.end()) {
                        const auto& firstSetSymbol = firstSets[symbol];
                        for (const auto& firstSymbol : firstSetSymbol) {
                            if (firstSymbol != "ε" && followSets[nonterminal].find(firstSymbol) != followSets[nonterminal].end()) {
                                return false;
                            }
                        }
                        if (firstSetSymbol.find("ε") != firstSetSymbol.end()) {
                            epsilonFound = true;
                        }
                    }
                    if (!epsilonFound) {
                        break;
                    }
                }
                if (epsilonFound) {
                    const auto& followSetNonterminal = followSets[nonterminal];
                    for (const auto& followSymbol : followSetNonterminal) {
                        if (followSets[nonterminal].find(followSymbol) != followSets[nonterminal].end()) {
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}


bool operator<(const Production& lhs, const Production& rhs) {
    return tie(lhs.left, lhs.right) < tie(rhs.left, rhs.right);
}


//打印SELECT集
void printSelect() 
{
    for (const auto& pair : selectSets)
    {
		cout << "SELECT(" << pair.first.left << " -> ";
        for (const auto& symbol : pair.first.right)
        {
			cout << symbol << " ";
		}
		cout << ") = 开始 ";
        for (const auto& symbol : pair.second)
        {
			cout << symbol << " ";
		}
		cout << "结束" << endl;
	}
}

//printProduction函数用于打印产生式
void printProduction(const Production& production) {
	cout << production.left << " -> ";
    for (const auto& symbol : production.right) {
		cout << symbol << "  ";
	}
	cout << endl;
}

class ASTNode {
    //一个抽象语法树节点类，包含节点类型，子节点，值
public:
    string type;
    vector<ASTNode*> children;
    string value; //从符号表中获取的值
    //可用类型构造函数
    ASTNode(const string& t) : type(t) {}
    //可用类型和值构造函数
    ASTNode(const string& t, const string& v) : type(t), value(v) {}
};

class Parser {
private:
    vector<Token> tokens;
    int currentTokenIndex;
    map<Production, set<string>> selectSets;
    vector<Production> productionList;
    map<string, set<string>> firstSets;
    map<string, set<string>> followSets;
    vector<string> nonterminalList;
    string startSymbol;
    //currentToken函数用于返回当前符号
    Token& currentToken() {
        //检查是否越界
        if (currentTokenIndex < tokens.size()) {
            return tokens[currentTokenIndex];
        }
       
    }
    //advance函数用于前进一个符号
    void advance() {
        if (currentTokenIndex < tokens.size()) {
            currentTokenIndex++;
        }
    }
    //match函数用于匹配当前符号是否是type类型的符号，如果是，就前进一个符号，返回true，否则返回false
    bool match(const string& type) {
        if (currentToken().type == type) {
            return true;
        }
        return false;
    }
    //inSelect函数用于判断当前符号是否在非终结符号的SELECT集中
    bool inSelect(const string& nonterminal) {
        //调取当前Token

            const Token& token = currentToken();
        
        
        //遍历所有产生式
        for (const auto& production : productionList) {
            //当前产生式的左部是当前这个非终结符号
            if (production.left == nonterminal) {
                //拿到当前产生式的SELECT集
                const auto& selectSet = selectSets[production];
                //如果当前符号的类型在SELECT集中，返回true
                //Token的type就是终结符号
                //当前可能已经读取完了，所以要判断是否越界
                if (currentTokenIndex < tokens.size()) {
                    if (selectSet.find(token.type) != selectSet.end()) {
                        return true;
                    }
                }
                else {
					//如果当前符号是终结符号,只可能有两种情况：剩余的非终结符推出空，或者没有剩余的非终结符
                    
                    //如果当前右部是空的，返回true，没有一个右部为空，返回false
                    if (production.right.empty())
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    ASTNode* parseProduction(const Production& production)
    {
        //ParceProduction
        cout << "解析产生式：";
        printProduction(production);
        cout << endl;
        ASTNode* node = new ASTNode(production.left);
        //对产生式的右部进行轮询处理
        if (production.right.empty()) {
            // Handle empty production (ε production)
            return node; // Return the current node for empty production
        }
        for (size_t i = 0; i < production.right.size(); ++i) {
            const auto& symbol = production.right[i];
            if (currentTokenIndex == tokens.size()) {
                //对于后面所有非终结符号全都采取推出空，如果有一个没有空产生式则报错
                bool allNullable = true;
                for (size_t j = i; j < production.right.size(); ++j) {
                    const auto& remainingSymbol = production.right[j];
                    if (isNonTerminal(remainingSymbol)) {
                        bool hasEmptyProduction = false;
                        for (const auto& prod : productionList) {
                            if (prod.left == remainingSymbol && prod.right.empty()) {
                                hasEmptyProduction = true;
                                break;
                            }
                        }
                        if (!hasEmptyProduction) {
                            allNullable = false;
                            break;
                        }
                    }
                    else {
                        allNullable = false;
                        break;
                    }
                }
                if (allNullable) {
                    // 所有后续的非终结符都有空产生式，直接返回当前节点
                    return node;
                }
                else {
                    // 有非终结符没有空产生式，解析失败
                    delete node;
                    return nullptr;
                }
            }

            //如果是非终结符号，调用parseNonTerminal子函数解析
            if (isNonTerminal(symbol))
            {
                if ((currentTokenIndex < tokens.size()))
                {
                    //如果当前符号在某个产生式的SELECT集中
                    if (inSelect(symbol))
                    {
                        //调用parseNonTerminal函数解析非终结符号
                        ASTNode* child = parseNonTerminal(symbol);
                        //如果解析成功，把子节点加入到当前节点的子节点中
                        if (child)
                        {
                            //if child起到的作用是判断是否解析成功
                            node->children.push_back(child);
                        }

                        else
                        {
                            //如果解析失败，删除当前节点，返回空指针
                            delete node;
                            //如果任何一个非终结符号解析失败，返回空指针
                            return nullptr;
                        }
                    }
                    else
                    {
                        //如果当前符号不在任何产生式的SELECT集中，返回空指针，解析失败
                        delete node;
                        return nullptr;
                    }
                }
                //非终结符号，如果是最后一个，不判断Inselect，直接处理
                else
                {
                    //调用parseNonTerminal函数解析非终结符号
                    ASTNode* child = parseNonTerminal(symbol);
                    //如果解析成功，把子节点加入到当前节点的子节点中
                    if (child)
                    {
                        //if child起到的作用是判断是否解析成功
                        node->children.push_back(child);
                    }

                    else
                    {
                        //如果解析失败，删除当前节点，返回空指针
                        delete node;
                        //如果任何一个非终结符号解析失败，返回空指针
                        return nullptr;
                    }
                }
            }
            //如果是终结符号，调用match函数匹配
            else {
                if (match(symbol)) {
                    cout << "匹配终结符号：" << symbol << endl << endl;
                    //如果匹配成功，把终结符号加入到当前节点的子节点中
                    node->children.push_back(new ASTNode(symbol, currentToken().value));
                    advance();
                }
                else {
                    //匹配失败，删除当前节点，返回空指针
                    delete node;
                    return nullptr;
                }
            }
        }
        //没有解析失败，返回当前节点
        return node;
    }


    ASTNode* parseNonTerminal(const string& nonterminal) {
        const Token& token = currentToken();
        //遍历所有产生式
        for (const auto& production : productionList) 
        {
            //如果左部是需要的非终结符号
            if (production.left == nonterminal) 
            {
                //拿到当前产生式的SELECT集
                const auto& selectSet = selectSets.at(production);
                //如果当前符号的类型在SELECT集中，调用parseProduction函数解析产生式
                if (selectSet.find(token.type) != selectSet.end()) 
                {
                    return parseProduction(production);
                }
            }
        }
       //如果没有找到合适的产生式，返回空指针
		return nullptr;
    }


    //处理非终结符号
    bool isNonTerminal(const string& symbol) {
        return find(nonterminalList.begin(), nonterminalList.end(), symbol) != nonterminalList.end();
    }
    void removeEmptyNodes(ASTNode* node) {
        if (!node) return;

        // 遍历子节点，递归删除空节点
        for (auto it = node->children.begin(); it != node->children.end(); ) {
            removeEmptyNodes(*it);

            // 检查子节点是否为空节点
            if ((*it)->children.empty() && (*it)->value.empty()) {
                delete* it;
                it = node->children.erase(it);
            }
            else {
                ++it;
            }
        }
    }

public:
   

    Parser(const vector<Token>& tokens, const vector<Production>& productions,
        const map<Production, set<string>>& selectSets,
        const map<string, set<string>>& firstSets,
        const map<string, set<string>>& followSets,
        const vector<string>& nonterminals, const string& start)
        : tokens(tokens), currentTokenIndex(0), productionList(productions),
        selectSets(selectSets), firstSets(firstSets), followSets(followSets),
        nonterminalList(nonterminals), startSymbol(start) {}

    ASTNode* parse() {
        auto root= parseNonTerminal(startSymbol);
        removeEmptyNodes(root);
        return root;
    }
};

void printAST(const ASTNode* node, const string& prefix = "", bool isLast = true) {
    if (!node) return;

    // 打印前缀
    cout << prefix;

    // 打印节点连接符
    cout << (isLast ? "└──" : "├──");

    // 打印节点类型和值
    cout << node->type;
    if (!node->value.empty()) {
        cout << ": " << node->value;
    }
    cout << endl;

    // 构建新的前缀
    string newPrefix = prefix + (isLast ? "    " : "│   ");

    // 递归打印子节点
    for (size_t i = 0; i < node->children.size(); ++i) {
        printAST(node->children[i], newPrefix, i == node->children.size() - 1);
    }
}



//删除select集合的空串
void deleteEpsilon(map<Production, set<string>>& selectSets)
{
    for (auto& pair : selectSets)
    {
		pair.second.erase("ε");
	}
}


//根据LL1转化后的新产生式更新非终结符号列表
void updateNonterminalList(const vector<Production>& productions, vector<string>& nonterminals) {
    nonterminals.clear();
    for (const auto& production : productions) {
        if (find(nonterminals.begin(), nonterminals.end(), production.left) == nonterminals.end()) {
            nonterminals.push_back(production.left);
        }
    }
}

void outPutGrammer() 
{
    cout << "开始符号：" << StartSymbol << endl << endl;
//输出终结符集合和非终结符集合
    cout << "终结符集合：" << endl;;
    for (const auto& terminal : terminal) {
		cout << terminal << " ";
	}
	cout << endl<<endl;

	cout << "非终结符集合："<<endl;
    for (const auto& nonterminal : nonterminalList) {
		cout << nonterminal << " ";
	}
    cout << endl << endl;;

    cout << "产生式集合：" << endl;
    for (const auto& production : productionList) {
		printProduction(production);
	}
    cout << endl;
}
//产生式按左部分类的函数,不改变其原来的存储数据结构
void classifyProduction() 
{
    map<string, vector<Production>> productionMap;
    for (const auto& production : productionList) {
		productionMap[production.left].push_back(production);
	}
    //遍历map放回原来的产生式集合
    vector<Production> newProductions;
    for (const auto& entry : productionMap) {
        for (const auto& production : entry.second) {
			newProductions.push_back(production);
		}
	}
    productionList = newProductions;
}




int main()
{
    computeFirstSets(nonterminalList, productionList, firstSets);
    computeFollowSets(nonterminalList, productionList, firstSets, followSets, StartSymbol);
    computeSelectSets(firstSets, followSets, selectSets, productionList);
    /*
    vector<Token> tokens = {
    {"if", "if", 0, 1, 1},
    {"(", "(", 1, 1, 4},
    {"IDENTIFIER", "x", 2, 1, 5},
    {")", ")", 3, 1, 6},
    {"{", "{", 4, 1, 8},
    {"IDENTIFIER", "y", 5, 2, 1},
    {"=", "=", 6, 2, 3},
    {"NUMBER", "10", 7, 2, 5},
    {";", ";", 8, 2, 7},
    {"}", "}", 9, 3, 1},
    {"else", "else", 10, 4, 1},
    {"{", "{", 11, 4, 6},
    {"IDENTIFIER", "y", 12, 5, 1},
    {"=", "=", 13, 5, 3},
    {"NUMBER", "20", 14, 5, 5},
    {";", ";", 15, 5, 7},
    {"}", "}", 16, 6, 1}
    };
    */
    vector<Token> tokens = {
        {"IDENTIFIER", "x", 0, 1, 1},
        {"=", "=", 1, 1, 2},
        {"IDENTIFIER", "x", 2, 1, 4},
        {"+", "+", 3, 1, 6},
        {"NUMBER", "3", 4, 1, 8},
        {";", ";", 5, 1, 9}
    };


   
    //初始化tokens
    Parser parser(tokens, productionList, selectSets, firstSets, followSets, nonterminalList, StartSymbol);
    auto root = parser.parse();
    if (root) 
    {
        cout << "语法分析成功" << endl;
		cout << "抽象语法树：" << endl;
		printAST(root);
	}
	else 
	{
		cout << "语法分析失败" << endl;
    }
}

