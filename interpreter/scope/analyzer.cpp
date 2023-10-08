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

    bool check_types(Node *t1, Node *t2) // CheckTypes
    {
        if (t1 == t2)
            return true;
        if (t1 == universal_node || t2 == universal_node)
            return true;
        if (t1->type == UNIVERSAL_ || t2->type == UNIVERSAL_)
            return true;
        if (t1->type == t2->type)
        {
            if (t1->type == ALIAS_TYPE_)
            {
                Alias t1_Alias = get<Alias>(t1->variant);
                Alias t2_Alias = get<Alias>(t2->variant);
                return check_types(t1_Alias.base_type, t2_Alias.base_type);
            }
            if (t1->type == ARRAY_TYPE_)
            {
                Array t1_array = get<Array>(t1->variant);
                Array t2_array = get<Array>(t2->variant);
                if (t1_array.num_elements == t2_array.num_elements)
                {
                    return check_types(t1_array.element_type, t2_array.element_type);
                }
            }
            else if (t1->type == STRUCT_TYPE_)
            {
                Struct t1_struct = get<Struct>(t1->variant);
                Struct t2_struct = get<Struct>(t2->variant);
                Node *f1 = t1_struct.fields;
                Node *f2 = t2_struct.fields;
                while (f1 != nullptr && f2 != nullptr)
                {
                    Field f1_field = get<Field>(f1->variant);
                    Field f2_field = get<Field>(f2->variant);
                    if (!check_types(f1_field.type, f2_field.type))
                        return false;
                }
                return (f1 == nullptr && f2 == nullptr);
            }
        }
        return false;
    }
};

int main()
{
    printf("dale");
}
