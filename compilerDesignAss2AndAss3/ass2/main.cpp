#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    char val;
    int pos;
    Node *c1, *c2;
    set<int> lpos, fpos;
    bool isNullable;

    Node(char val, int pos)
    {
        this->val = val;
        this->pos = pos;
        c1 = c2 = NULL;
        isNullable = false;
    }
};

class DState
{
public:
    set<int> positions;
    unordered_map<char, int> transitions;
    bool isFinal;

    DState(set<int> positions)
    {
        this->positions = positions;
        isFinal = false;
    }
};

bool isOperator(char c)
{
    return c == '*' || c == '.' || c == '|';
}

Node *createTree(string regex, int &followPosSize, map<int, Node *> &pos2node)
{
    stack<Node *> st;
    int pos = 1;

    for (char c : regex)
    {
        if (!isOperator(c))
        {

            Node *newNode = new Node(c, pos);
            pos2node[pos++] = newNode;
            newNode->lpos.insert(newNode->pos);
            newNode->fpos.insert(newNode->pos);
            st.push(newNode);
        }
        else
        {
            Node *newNode = new Node(c, 0);
            if (c == '*')
            {
                if (!st.empty())
                {
                    newNode->c1 = st.top();
                    st.pop();
                }
            }
            else
            {
                if (!st.empty())
                {
                    newNode->c2 = st.top();
                    st.pop();
                }
                if (!st.empty())
                {
                    newNode->c1 = st.top();
                    st.pop();
                }
            }
            st.push(newNode);
        }
    }
    followPosSize = pos;
    return st.top();
}

void updateNullable(Node *root)
{
    if (!root)
        return;
    updateNullable(root->c1);
    updateNullable(root->c2);

    if (!isOperator(root->val))
        root->isNullable = false;
    else if (root->val == '*')
        root->isNullable = true;
    else if (root->val == '|')
        root->isNullable = (root->c1->isNullable || root->c2->isNullable);
    else if (root->val == '.')
        root->isNullable = (root->c1->isNullable && root->c2->isNullable);
}

void updateLposFpos(Node *root)
{
    if (!root)
        return;
    updateLposFpos(root->c1);
    updateLposFpos(root->c2);

    if (!isOperator(root->val))
    {
        root->fpos.insert(root->pos);
        root->lpos.insert(root->pos);
    }

    else if (root->val == '*')
    {
        root->fpos = root->c1->fpos;
        root->lpos = root->c1->lpos;
    }

    else if (root->val == '|')
    {
        root->fpos.insert(root->c1->fpos.begin(), root->c1->fpos.end());
        root->fpos.insert(root->c2->fpos.begin(), root->c2->fpos.end());

        root->lpos.insert(root->c1->lpos.begin(), root->c1->lpos.end());
        root->lpos.insert(root->c2->lpos.begin(), root->c2->lpos.end());
    }

    else if (root->val == '.')
    {
        root->fpos = root->c1->fpos;
        if (root->c1->isNullable)
        {
            root->fpos.insert(root->c2->fpos.begin(), root->c2->fpos.end());
        }

        root->lpos = root->c2->lpos;
        if (root->c2->isNullable)
        {
            root->lpos.insert(root->c1->lpos.begin(), root->c1->lpos.end());
        }
    }
}

void printLposFpos(Node *root)
{
    if (!root)
        return;

    vector<Node *> nodes;
    queue<Node *> q;
    q.push(root);

    while (!q.empty())
    {
        Node *curr = q.front();
        q.pop();
        nodes.push_back(curr);

        if (curr->c1)
            q.push(curr->c1);
        if (curr->c2)
            q.push(curr->c2);
    }

    cout << "\nSyntax Tree Table:\n";
    cout << string(60, '-') << endl;
    cout << left << setw(10) << "Node" << setw(20) << "Last Pos"
         << setw(20) << "First Pos" << setw(10) << "Nullable" << endl;
    cout << string(60, '-') << endl;

    for (Node *node : nodes)
    {
        cout << left << setw(10) << node->val;

        stringstream lposStream, fposStream;
        lposStream << "{ ";
        for (int pos : node->lpos)
            lposStream << pos << " ";
        lposStream << "}";

        fposStream << "{ ";
        for (int pos : node->fpos)
            fposStream << pos << " ";
        fposStream << "}";

        cout << setw(20) << lposStream.str()
             << setw(20) << fposStream.str()
             << setw(10) << (node->isNullable ? "Yes" : "No")
             << endl;
    }

    cout << string(60, '-') << endl;
}

void computeFollowPos(Node *root, vector<set<int>> &followPos)
{
    if (!root)
        return;

    computeFollowPos(root->c1, followPos);
    computeFollowPos(root->c2, followPos);

    if (root->val == '.')
    {
        for (int i : root->c1->lpos)
        {
            followPos[i].insert(root->c2->fpos.begin(), root->c2->fpos.end());
        }
    }
    else if (root->val == '*')
    {
        for (int i : root->lpos)
        {
            followPos[i].insert(root->fpos.begin(), root->fpos.end());
        }
    }
}

void printFollowPos(vector<set<int>> &followPos, map<int, Node *> &pos2node)
{
    cout << "\nFollow position Table:\n"
         << endl;
    cout << string(40, '-') << endl;
    cout << left << setw(10) << "Symbol" << setw(10) << "Pos" << "FollowPos" << endl;
    cout << string(40, '-') << endl;

    for (int i = 1; i < followPos.size(); i++)
    {
        cout << left << setw(10) << pos2node[i]->val << setw(10) << i << "[ ";
        for (auto it : followPos[i])
        {
            cout << it << " ";
        }
        cout << "]" << endl;
    }
}

vector<DState *> createDFA(Node *root, vector<set<int>> &followPos, map<int, Node *> &pos2node)
{
    vector<DState *> dStates;
    map<set<int>, int> dStateMap;
    set<char> symbols;
    queue<int> unmarkedStates;

    for (auto it : pos2node)
    {
        if (!isOperator(it.second->val) && it.second->val != '#')
        {
            symbols.insert(it.second->val);
        }
    }

    DState *startState = new DState(root->fpos);
    dStates.push_back(startState);
    dStateMap[root->fpos] = 0;
    unmarkedStates.push(0);

    while (!unmarkedStates.empty())
    {
        int stateIndex = unmarkedStates.front();
        unmarkedStates.pop();
        DState *currentState = dStates[stateIndex];

        for (char symbol : symbols)
        {
            set<int> nStatePos;

            for (int pos : currentState->positions)
            {
                if (pos2node[pos]->val == symbol)
                {
                    nStatePos.insert(followPos[pos].begin(), followPos[pos].end());
                }
            }

            if (!nStatePos.empty() && dStateMap.find(nStatePos) == dStateMap.end())
            {
                DState *newState = new DState(nStatePos);
                int newIndex = dStates.size();
                dStates.push_back(newState);
                dStateMap[nStatePos] = newIndex;
                unmarkedStates.push(newIndex);
            }

            if (!nStatePos.empty())
            {
                currentState->transitions[symbol] = dStateMap[nStatePos];
            }
        }
    }

    for (DState *state : dStates)
    {
        for (int pos : state->positions)
        {
            if (pos2node[pos]->val == '#')
            {
                state->isFinal = true;
                break;
            }
        }
    }
    return dStates;
}

string setToString(const set<int> &s)
{
    stringstream ss;
    ss << "{ ";
    for (int pos : s)
    {
        ss << pos << " ";
    }
    ss << "}";
    return ss.str();
}

void printDFA(const vector<DState *> &dStates)
{
    cout << "\n\nState Table:";

    set<char> symbols;
    for (const auto &state : dStates)
    {
        for (const auto &[symbol, _] : state->transitions)
        {
            symbols.insert(symbol);
        }
    }

    const int stateColWidth = 35;
    const int transColWidth = 30;

    cout << "\n"
         << string(stateColWidth + transColWidth * symbols.size(), '-') << "\n";
    cout << left << setw(stateColWidth) << "States";
    for (char symbol : symbols)
    {
        cout << setw(transColWidth) << ("on '" + string(1, symbol) + "'");
    }
    cout << "\n"
         << string(stateColWidth + transColWidth * symbols.size(), '-') << "\n";

    for (size_t i = 0; i < dStates.size(); i++)
    {
        string stateInfo = "State " + to_string(i) + " " + setToString(dStates[i]->positions);
        if (dStates[i]->isFinal)
        {
            stateInfo += " [Final]";
        }
        cout << left << setw(stateColWidth) << stateInfo;

        for (char symbol : symbols)
        {
            if (dStates[i]->transitions.count(symbol))
            {
                cout << setw(transColWidth) << dStates[i]->transitions.at(symbol);
            }
            else
            {
                cout << setw(transColWidth) << "-";
            }
        }
        cout << "\n";
    }
}

int main()
{
    string input;
    cout << "Enter the input regular expression in postfix form: " << endl;
    cin >> input;

    int followPosSize;
    map<int, Node *> pos2node;
    Node *synTreeRoot = createTree(input, followPosSize, pos2node);
    updateNullable(synTreeRoot);
    updateLposFpos(synTreeRoot);

    vector<set<int>> followPos(followPosSize);

    computeFollowPos(synTreeRoot, followPos);

    printLposFpos(synTreeRoot);

    printFollowPos(followPos, pos2node);

    vector<DState *> states = createDFA(synTreeRoot, followPos, pos2node);
    printDFA(states);

    return 0;
}
