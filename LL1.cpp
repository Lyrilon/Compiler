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
    vector<Production> newProductions; // �洢�µĲ���ʽ����
    for (const string& nonterminal : nonterminalList) {
        vector<Production> alpha; // �洢��ݹ����ʽ���Ҳ�
        vector<Production> beta;  // �洢����ݹ����ʽ���Ҳ�
        for (auto& prod : productionList) {
            if (prod.left == nonterminal) {
                if (!prod.right.empty() && prod.right[0] == nonterminal) {
                    // ��ȡ��ݹ鲿�֣������µĲ���ʽ�Ҳ�
                    alpha.push_back(Production(nonterminal + "*", vector<string>(prod.right.begin() + 1, prod.right.end())));
                }
                else {
                    // ����ݹ鲿�֣�ֱ�Ӽ���beta
                    beta.push_back(prod);
                }
            }
        }
        if (!alpha.empty()) {
            for (auto& prod : beta) {
                // ����ÿ������ݹ����ʽ�������µķ��ս��
                prod.right.push_back(nonterminal + "*");
                newProductions.push_back(prod);
            }
            for (auto& prod : alpha) {
                // ����ÿ����ݹ����ʽ�������µķ��ս���Ϳյ�vector��ʾ��
                prod.right.push_back(nonterminal + "*");
                newProductions.push_back(prod);
            }
            // ���һ���µĲ���ʽ����ʾ�µķ��ս�������Ƶ�����
            newProductions.push_back(Production(nonterminal + "*", {})); // �յ�vector��ʾ��
            // ���µķ��ս��������ս���б�
            nonterminalList.push_back(nonterminal + "*");
        }
        else {
            // ���û����ݹ����ʽ��ֱ�Ӽ���ԭ���ķ���ݹ����ʽ
            for (auto& prod : beta) {
                newProductions.push_back(prod);
            }
        }
    }
    // ���²���ʽ�б�
    productionList = newProductions;
}



void leftFactoring() {
    vector<Production> newProductions; // �µĲ���ʽ����
    unordered_map<string, vector<vector<string>>> prefixMap; // ���ڴ洢ÿ�����ս���Ŷ�Ӧ�Ĳ���ʽ�Ҳ�

    // ��ÿ������ʽ�����󲿷��ս������
    for (auto& prod : productionList) {
        prefixMap[prod.left].push_back(prod.right);
    }

    // ��ÿ�����ս���ŵĲ���ʽ������������ȡ
    for (auto& entry : prefixMap) {
        //��ǰ��
        string nonterminal = entry.first;
        //��ǰ�󲿵Ĳ���ʽ�Ҳ�
        vector<vector<string>> rights = entry.second;

        while (true) {
            unordered_map<string, vector<vector<string>>> factorMap; // ���ڴ洢������ͬǰ׺�Ĳ���ʽ
            
            //����ǰ���ս���ŵ����в���ʽ������ͬǰ׺�Ĵ�һ��
            //�п��ܱ���ֻ��һ���Ҳ�����ʱ����Ҫ����
            
            for (vector<string>& right : rights) {
                //�����rightΪ����ʽ�Ҳ���������vector<string>
                //�����ǿմ�ҲҪ����
                if (!right.empty()) {
                    factorMap[right[0]].push_back(vector<string>(right.begin() + 1, right.end())); // ����һ�����ŷ���
                } 
                else {
					factorMap[""].push_back(vector<string>()); // �մ�
				}
            }

            // �����ߴ�û�б仯��˵��û�й���ǰ׺
            if (factorMap.size() == rights.size()) {
                // û�й���ǰ׺���²���ʽ�б����ԭ����ʽ
                for (auto& right : rights) {
                    newProductions.push_back(Production(nonterminal, right)); // ���ԭ����ʽ
                }
                break; // ����ѭ��
            }
            //���ڹ���ǰ׺�Ļ�
            else {
                //��Ҫ����һ���м���ս����
                string newNonterminal = nonterminal + "'"; // �µķ��ս����
                //�µķ��ս���ż�����ս�����б�
                nonterminalList.push_back(newNonterminal); // ���·��ս���ż�����ս���б�
                //�µĲ���ʽ�б�����µĲ���ʽ
                vector<vector<string>> newRights; // �洢�µĲ���ʽ�Ҳ�

                // ����������ͬǰ׺�Ĳ���ʽ
                for (auto& entry : factorMap) 
                {
                    if (entry.second.size() > 1) { // ������ڶ������ʽ������ͬǰ׺
                        //����newRights���µĲ���ʽ�Ҳ�����һ������ǰ׺��һ���м���ս���ŵ����
                        newRights.push_back({ entry.first, newNonterminal }); // �����µĲ���ʽ�Ҳ�
                        for (auto& right : entry.second) {
                            newProductions.push_back(Production(newNonterminal, right)); // �����µĲ���ʽ
                        }
                    }
                    //��������ڶ������ʽ������ͬǰ׺
                    else 
                    {
                        newRights.push_back({ entry.first }); // ����ԭ����ʽ
                    }
                }
                // ���²���ʽ�Ҳ�
                rights = newRights; // ���²���ʽ�Ҳ�
            }
        }
    }
    productionList = newProductions; // ���²���ʽ�б�
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
                        newRight.push_back("��");
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

// �ж��Ƿ�Ϊ���ս����
bool isNonTerminal(const string& symbol, const vector<string>& nonterminals) {
    return find(nonterminals.begin(), nonterminals.end(), symbol) != nonterminals.end();
}

// ����FIRST��
void computeFirstSets(const vector<string>& nonterminals, const vector<Production>& productions, map<string, set<string>>& firstSets) {
    //����ÿ�����ս���ŵ�FIRST��
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
        // ��ÿ������ʽ���д���
        for (const auto& production : productions) {
            //�õ�����ʽ���󲿺��Ҳ�
            const string& left = production.left;
            const vector<string>& right = production.right;
            //����Ҳ�Ϊ�գ�˵���ǿ�Ϊ�Ų���ʽ
            if (right.empty()) {
                // �����ǰ���ս����û�Цţ����뵽FIRST���У��жϴ治�����Ƿ�ֹ�ظ�����
                if (firstSets[left].find("��") == firstSets[left].end()) {
                    firstSets[left].insert("��");
                    changed = true;
                }
            }
            else {
                //����Ҳ���һ���������ս���ţ�ֱ�Ӽ��뵽FIRST����
                for (size_t i = 0; i < right.size(); ++i) 
                {
                    const string& symbol = right[i];
                    //����Ƿ��ս����
                    if (isNonTerminal(symbol, nonterminals)) 
                    {
                        //���ұ߷��ս���ŵ�FIRST�����뵽�󲿵�FIRST����
                        set<string> symbolFirstSet = firstSets[symbol];
                        //��ǰ���ս���ŵ�FIRST�����Ƿ��Цţ������ѭ��������û��ֱ���˳�ѭ��
                        bool hasEpsilon = (symbolFirstSet.find("��") != symbolFirstSet.end());
                        //ֻ���Ҳ�ȫ����Ϊ�ţ����ܰѦż��뵽�󲿵�FIRST����
                        symbolFirstSet.erase("��");
                        //���Ҳ����ս���ŵ�FIRST�����뵽�󲿵�FIRST���У�����addFirstSet��������true˵���б仯
                        if (addFirstSet(firstSets[left], symbolFirstSet)) {
                            changed = true;
                        }
                        //����Ҳ����ս���ŵ�FIRST����û�Цţ�ֱ���˳�ѭ��
                        if (!hasEpsilon) {
                            break;
                        }
                    }
                    //������ս���ţ�ֱ�Ӽ��뵽�󲿵�FIRST����
                    else 
                    {
                        //�����ǰ���ս���ŵ�FIRST����û������ս���ţ����뵽FIRST����
                        if (firstSets[left].find(symbol) == firstSets[left].end()) {
                            firstSets[left].insert(symbol);
                            changed = true;
                        }
                        break;
                    }
                    //�鵽���һ�����ţ�����Ҳ����з��Ŷ����Ƴ��ţ���ô�Ѧż��뵽�󲿵�FIRST����
                    if (i == right.size() - 1) 
                    {
                        //����Ҳ����з��Ŷ����Ƴ��ţ���ô�Ѧż��뵽�󲿵�FIRST����
                        if (firstSets[left].find("��") == firstSets[left].end()) 
                        {
                            firstSets[left].insert("��");
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
}
//��ӡFIRST��
void printFirstSets(const map<string, set<string>>& firstSets) {
    for (const auto& pair : firstSets) {
		cout << "FIRST(" << pair.first << ") =\t ";
        for (const auto& symbol : pair.second) {
			cout << symbol << " ";
		}
		cout << endl;
	}
}
//��ӡFollow����
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
FIRST(StmtList) = { if, while, IDENTIFIER, {, �� }

Stmt:
FIRST(Stmt) = { if, while, IDENTIFIER, { }

IfStmt:
FIRST(IfStmt) = { if }

ElseIfList:
FIRST(ElseIfList) = { else if, �� }

ElseIf:
FIRST(ElseIf) = { else if }

ElsePart:
FIRST(ElsePart) = { else, �� }

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
//����FOLLOW��

void computeFollowSets(const vector<string>& nonterminals, const vector<Production>& productions, const map<string, set<string>>& firstSets, map<string, set<string>>& followSets, const string& startSymbol) {
    // ��ʼ��ÿ�����ս���� FOLLOW ����Ϊ�ռ�
    for (const auto& nonterminal : nonterminals) {
        followSets[nonterminal] = set<string>();
    }

    // �� $ ���뿪ʼ���ŵ� FOLLOW ����
    followSets[startSymbol].insert("$");

    bool changed;
    do {
        changed = false;
        // �������в���ʽ
        for (const auto& production : productions) {
            const string& left = production.left; // ��ȡ����ʽ��
            const vector<string>& right = production.right; // ��ȡ����ʽ�Ҳ�

            // ��������ʽ�Ҳ���ÿ������
            for (size_t i = 0; i < right.size(); ++i) {
                const string& symbol = right[i]; // ��ȡ�Ҳ��ĵ� i ������
                // ����Ƿ��ս����
                if (isNonTerminal(symbol, nonterminals)) {
                    size_t j = i + 1;
                    bool epsilonFound = true;
                    // epsilonFound ��ʼ��Ϊ true������Ҳ����з��Ŷ����Ƴ� �ţ�epsilonFound Ϊ true
                    while (j < right.size() && epsilonFound) {
                        epsilonFound = false;
                        const string& nextSymbol = right[j]; // ��ȡ�Ҳ�����һ������

                        if (isNonTerminal(nextSymbol, nonterminals)) 
                        {
                            // �����һ�������Ƿ��ս���ţ����� FIRST �����е�Ԫ����ӵ� symbol �� FOLLOW ������
                            const auto& firstSetNext = firstSets.at(nextSymbol);
                            //����firstSetNext�е�ÿһ��Ԫ��
                            for (const auto& firstSymbol : firstSetNext) {
                                //������Ǧţ��Ͱ�firstSymbol���뵽symbol��FOLLOW������
                                if (firstSymbol != "��") {
                                    // �� nextSymbol �� FIRST �����е�Ԫ����ӵ� symbol �� FOLLOW ������
                                    //�� C++ �ı�׼�� set �У�insert ��������һ�� pair����� pair �ĵ�һ
                                    // ��Ԫ����һ����������ָ�����Ԫ�ص�λ�ã��ڶ���Ԫ����һ������
                                    // ֵ����ʾ��������Ƿ�ɹ�����׳�ԣ�³���ԣ�
                                    if (followSets[symbol].insert(firstSymbol).second) {
                                        changed = true; // ���Ϊ true����ʾ����Ԫ�ر����
                                    }
                                }

                                else 
                                {
                                    epsilonFound = true; // ��� FIRST �������� �ţ����������һ������
                                }
                            }
                        }
                        else 
                        {
                            // �����һ���������ս���ţ�ֱ�ӽ�����ӵ� symbol �� FOLLOW ������
                            if (followSets[symbol].insert(nextSymbol).second) {
                                changed = true; // ���Ϊ true����ʾ����Ԫ�ر����
                            }
                            epsilonFound = false; // �ս���ź���ķ��Ų��ټ��
                            break;
                        }
                        j++;
                    }

           
                    if (epsilonFound) 
                    {
                        const auto& followSetLeft = followSets.at(left);
                        for (const auto& followSymbol : followSetLeft) 
                        {
                            // �� left �� FOLLOW �����е�Ԫ����ӵ� symbol �� FOLLOW ������
                            if (followSets[symbol].insert(followSymbol).second) {
                                changed = true; // ���Ϊ true����ʾ����Ԫ�ر����
                            }
                        }
                    }
                }
            }
        }
    } while (changed); // ������Ԫ�ر����ʱ������ѭ��
}

void computeSelectSets( const map<string, set<string>>& firstSets, const map<string, set<string>>& followSets, map<Production, set<string>>& selectSets,vector<Production>productionList) 
{
    //��������ʽ
    for (const auto& production : productionList) 
    {
        const string& left = production.left;
        const vector<string>& right = production.right;

        set<string> selectSet;
        //���Ϊ�ղ���ʽ��ֱ�Ӱ�FOLLOW�����뵽SELECT����
        if (right.empty()) {
            selectSet.insert(followSets.at(left).begin(), followSets.at(left).end());
        }
        else {
            //���ǿղ���ʽ
            bool containsEpsilon = true;

            for (const auto& symbol : right) 
            {
                //����׷��Ƿ��ս����
                if (isNonTerminal(symbol, nonterminalList)) 
                {
                    //�õ����ս���ŵ�FIRST��
                    const auto& firstSet = firstSets.at(symbol);
          		  //�ѷ��ս���ŵ�FIRST�����뵽SELECT����
                    selectSet.insert(firstSet.begin(), firstSet.end());
                    //������ս���ŵ�FIRST����û�Цţ�ֱ���˳�ѭ��
                    if (firstSet.find("��") == firstSet.end()) 
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
            //����Ҳ����з��Ŷ����Ƴ��ţ���ô����FOLLOW�����뵽SELECT����
            if (containsEpsilon) {
                selectSet.insert(followSets.at(left).begin(), followSets.at(left).end());
            }
        }
        selectSets[production] = selectSet;
    }
    //����ɾ��SELECT���еĦţ�ԭ���ǦŲ����ս���ţ�������������봮�У����ҦŲ���Ӱ��SELECT���ļ���
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
                    if (firstSetSymbol.find("��") != firstSetSymbol.end()) {
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
                firstAlpha.insert("��");
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
                            if (firstSymbol != "��" && followSets[nonterminal].find(firstSymbol) != followSets[nonterminal].end()) {
                                return false;
                            }
                        }
                        if (firstSetSymbol.find("��") != firstSetSymbol.end()) {
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


//��ӡSELECT��
void printSelect() 
{
    for (const auto& pair : selectSets)
    {
		cout << "SELECT(" << pair.first.left << " -> ";
        for (const auto& symbol : pair.first.right)
        {
			cout << symbol << " ";
		}
		cout << ") = ��ʼ ";
        for (const auto& symbol : pair.second)
        {
			cout << symbol << " ";
		}
		cout << "����" << endl;
	}
}

//printProduction�������ڴ�ӡ����ʽ
void printProduction(const Production& production) {
	cout << production.left << " -> ";
    for (const auto& symbol : production.right) {
		cout << symbol << "  ";
	}
	cout << endl;
}

class ASTNode {
    //һ�������﷨���ڵ��࣬�����ڵ����ͣ��ӽڵ㣬ֵ
public:
    string type;
    vector<ASTNode*> children;
    string value; //�ӷ��ű��л�ȡ��ֵ
    //�������͹��캯��
    ASTNode(const string& t) : type(t) {}
    //�������ͺ�ֵ���캯��
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
    //currentToken�������ڷ��ص�ǰ����
    Token& currentToken() {
        //����Ƿ�Խ��
        if (currentTokenIndex < tokens.size()) {
            return tokens[currentTokenIndex];
        }
       
    }
    //advance��������ǰ��һ������
    void advance() {
        if (currentTokenIndex < tokens.size()) {
            currentTokenIndex++;
        }
    }
    //match��������ƥ�䵱ǰ�����Ƿ���type���͵ķ��ţ�����ǣ���ǰ��һ�����ţ�����true�����򷵻�false
    bool match(const string& type) {
        if (currentToken().type == type) {
            return true;
        }
        return false;
    }
    //inSelect���������жϵ�ǰ�����Ƿ��ڷ��ս���ŵ�SELECT����
    bool inSelect(const string& nonterminal) {
        //��ȡ��ǰToken

            const Token& token = currentToken();
        
        
        //�������в���ʽ
        for (const auto& production : productionList) {
            //��ǰ����ʽ�����ǵ�ǰ������ս����
            if (production.left == nonterminal) {
                //�õ���ǰ����ʽ��SELECT��
                const auto& selectSet = selectSets[production];
                //�����ǰ���ŵ�������SELECT���У�����true
                //Token��type�����ս����
                //��ǰ�����Ѿ���ȡ���ˣ�����Ҫ�ж��Ƿ�Խ��
                if (currentTokenIndex < tokens.size()) {
                    if (selectSet.find(token.type) != selectSet.end()) {
                        return true;
                    }
                }
                else {
					//�����ǰ�������ս����,ֻ���������������ʣ��ķ��ս���Ƴ��գ�����û��ʣ��ķ��ս��
                    
                    //�����ǰ�Ҳ��ǿյģ�����true��û��һ���Ҳ�Ϊ�գ�����false
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
        cout << "��������ʽ��";
        printProduction(production);
        cout << endl;
        ASTNode* node = new ASTNode(production.left);
        //�Բ���ʽ���Ҳ�������ѯ����
        if (production.right.empty()) {
            // Handle empty production (�� production)
            return node; // Return the current node for empty production
        }
        for (size_t i = 0; i < production.right.size(); ++i) {
            const auto& symbol = production.right[i];
            if (currentTokenIndex == tokens.size()) {
                //���ں������з��ս����ȫ����ȡ�Ƴ��գ������һ��û�пղ���ʽ�򱨴�
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
                    // ���к����ķ��ս�����пղ���ʽ��ֱ�ӷ��ص�ǰ�ڵ�
                    return node;
                }
                else {
                    // �з��ս��û�пղ���ʽ������ʧ��
                    delete node;
                    return nullptr;
                }
            }

            //����Ƿ��ս���ţ�����parseNonTerminal�Ӻ�������
            if (isNonTerminal(symbol))
            {
                if ((currentTokenIndex < tokens.size()))
                {
                    //�����ǰ������ĳ������ʽ��SELECT����
                    if (inSelect(symbol))
                    {
                        //����parseNonTerminal�����������ս����
                        ASTNode* child = parseNonTerminal(symbol);
                        //��������ɹ������ӽڵ���뵽��ǰ�ڵ���ӽڵ���
                        if (child)
                        {
                            //if child�𵽵��������ж��Ƿ�����ɹ�
                            node->children.push_back(child);
                        }

                        else
                        {
                            //�������ʧ�ܣ�ɾ����ǰ�ڵ㣬���ؿ�ָ��
                            delete node;
                            //����κ�һ�����ս���Ž���ʧ�ܣ����ؿ�ָ��
                            return nullptr;
                        }
                    }
                    else
                    {
                        //�����ǰ���Ų����κβ���ʽ��SELECT���У����ؿ�ָ�룬����ʧ��
                        delete node;
                        return nullptr;
                    }
                }
                //���ս���ţ���������һ�������ж�Inselect��ֱ�Ӵ���
                else
                {
                    //����parseNonTerminal�����������ս����
                    ASTNode* child = parseNonTerminal(symbol);
                    //��������ɹ������ӽڵ���뵽��ǰ�ڵ���ӽڵ���
                    if (child)
                    {
                        //if child�𵽵��������ж��Ƿ�����ɹ�
                        node->children.push_back(child);
                    }

                    else
                    {
                        //�������ʧ�ܣ�ɾ����ǰ�ڵ㣬���ؿ�ָ��
                        delete node;
                        //����κ�һ�����ս���Ž���ʧ�ܣ����ؿ�ָ��
                        return nullptr;
                    }
                }
            }
            //������ս���ţ�����match����ƥ��
            else {
                if (match(symbol)) {
                    cout << "ƥ���ս���ţ�" << symbol << endl << endl;
                    //���ƥ��ɹ������ս���ż��뵽��ǰ�ڵ���ӽڵ���
                    node->children.push_back(new ASTNode(symbol, currentToken().value));
                    advance();
                }
                else {
                    //ƥ��ʧ�ܣ�ɾ����ǰ�ڵ㣬���ؿ�ָ��
                    delete node;
                    return nullptr;
                }
            }
        }
        //û�н���ʧ�ܣ����ص�ǰ�ڵ�
        return node;
    }


    ASTNode* parseNonTerminal(const string& nonterminal) {
        const Token& token = currentToken();
        //�������в���ʽ
        for (const auto& production : productionList) 
        {
            //���������Ҫ�ķ��ս����
            if (production.left == nonterminal) 
            {
                //�õ���ǰ����ʽ��SELECT��
                const auto& selectSet = selectSets.at(production);
                //�����ǰ���ŵ�������SELECT���У�����parseProduction������������ʽ
                if (selectSet.find(token.type) != selectSet.end()) 
                {
                    return parseProduction(production);
                }
            }
        }
       //���û���ҵ����ʵĲ���ʽ�����ؿ�ָ��
		return nullptr;
    }


    //������ս����
    bool isNonTerminal(const string& symbol) {
        return find(nonterminalList.begin(), nonterminalList.end(), symbol) != nonterminalList.end();
    }
    void removeEmptyNodes(ASTNode* node) {
        if (!node) return;

        // �����ӽڵ㣬�ݹ�ɾ���սڵ�
        for (auto it = node->children.begin(); it != node->children.end(); ) {
            removeEmptyNodes(*it);

            // ����ӽڵ��Ƿ�Ϊ�սڵ�
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

    // ��ӡǰ׺
    cout << prefix;

    // ��ӡ�ڵ����ӷ�
    cout << (isLast ? "������" : "������");

    // ��ӡ�ڵ����ͺ�ֵ
    cout << node->type;
    if (!node->value.empty()) {
        cout << ": " << node->value;
    }
    cout << endl;

    // �����µ�ǰ׺
    string newPrefix = prefix + (isLast ? "    " : "��   ");

    // �ݹ��ӡ�ӽڵ�
    for (size_t i = 0; i < node->children.size(); ++i) {
        printAST(node->children[i], newPrefix, i == node->children.size() - 1);
    }
}



//ɾ��select���ϵĿմ�
void deleteEpsilon(map<Production, set<string>>& selectSets)
{
    for (auto& pair : selectSets)
    {
		pair.second.erase("��");
	}
}


//����LL1ת������²���ʽ���·��ս�����б�
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
    cout << "��ʼ���ţ�" << StartSymbol << endl << endl;
//����ս�����Ϻͷ��ս������
    cout << "�ս�����ϣ�" << endl;;
    for (const auto& terminal : terminal) {
		cout << terminal << " ";
	}
	cout << endl<<endl;

	cout << "���ս�����ϣ�"<<endl;
    for (const auto& nonterminal : nonterminalList) {
		cout << nonterminal << " ";
	}
    cout << endl << endl;;

    cout << "����ʽ���ϣ�" << endl;
    for (const auto& production : productionList) {
		printProduction(production);
	}
    cout << endl;
}
//����ʽ���󲿷���ĺ���,���ı���ԭ���Ĵ洢���ݽṹ
void classifyProduction() 
{
    map<string, vector<Production>> productionMap;
    for (const auto& production : productionList) {
		productionMap[production.left].push_back(production);
	}
    //����map�Ż�ԭ���Ĳ���ʽ����
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


   
    //��ʼ��tokens
    Parser parser(tokens, productionList, selectSets, firstSets, followSets, nonterminalList, StartSymbol);
    auto root = parser.parse();
    if (root) 
    {
        cout << "�﷨�����ɹ�" << endl;
		cout << "�����﷨����" << endl;
		printAST(root);
	}
	else 
	{
		cout << "�﷨����ʧ��" << endl;
    }
}

