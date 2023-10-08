#include <variant>
#include "classes.cpp"
#include <bits/stdc++.h>
using namespace std;

class Scope
{
public:
    vector<Node *> SymbolTable;     // primeiro da lista encadeada
    vector<Node *> SymbolTableLast; // ultimo da lista encadeada
    int currentLevel = -1;
    variant<Var, Param, Field, Function, Array, Struct, Alias> variant;
    int newBlock()
    {
        SymbolTable.push_back(nullptr);
        SymbolTableLast.push_back(nullptr);
        currentLevel++;
        return currentLevel;
    }

    int endBlock()
    {
        currentLevel--;
        SymbolTable.pop_back();
        SymbolTableLast.pop_back();
        return currentLevel;
    }

    Node *define(int token_secundario)
    {
        Node *newNode = new Node();
        newNode->token_secundario = token_secundario;
        newNode->next = nullptr;
        if (SymbolTable[currentLevel] == nullptr)
        {
            SymbolTable[currentLevel] = newNode;
            SymbolTableLast[currentLevel] = newNode;
        }
        else
        {
            SymbolTableLast[currentLevel]->next = newNode;
            SymbolTableLast[currentLevel] = newNode;
        }
        return newNode;
    }

    Node *search_in_level(int token_sec, int currLevel = -1)
    { // funcao Search na apostila
        if (currentLevel == -1)
            return nullptr;
        if (currLevel == -1)
            currLevel = currentLevel;
        Node *node = SymbolTable[currLevel];
        while (node != nullptr)
        {
            if (node->token_secundario == token_sec)
                break;
            else
                node = node->next;
        }
        return node;
    }

    Node *search_in_all_levels(int token_sec)
    { // funcao Find na apostila
        Node *node = nullptr;
        for (int i = currentLevel; i >= 0; i--)
        {
            node = search_in_level(token_sec, i);
            if (node != nullptr && node->token_secundario == token_sec)
                break;
        }
        return node;
    }

    boolean check_types(Node *t1, Node *t2)
    {
    }
};

int main()
{
    printf("dale");
}
