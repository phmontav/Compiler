#include <variant>
#include "classes.cpp"
#include "error.cpp"
#include <bits/stdc++.h>
using namespace std;

class Scope
{
public:
    vector<Object *> SymbolTable;     // primeiro da lista encadeada
    vector<Object *> SymbolTableLast; // ultimo da lista encadeada
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

    Object *define(int token_secundario)
    {
        Object *newObject = new Object();
        newObject->token_secundario = token_secundario;
        newObject->next = nullptr;
        if (SymbolTable[currentLevel] == nullptr)
        {
            SymbolTable[currentLevel] = newObject;
            SymbolTableLast[currentLevel] = newObject;
        }
        else
        {
            SymbolTableLast[currentLevel]->next = newObject;
            SymbolTableLast[currentLevel] = newObject;
        }
        return newObject;
    }

    Object *search_in_level(int token_sec, int currLevel = -1)
    { // funcao Search na apostila
        if (currentLevel == -1)
            return nullptr;
        if (currLevel == -1)
            currLevel = currentLevel;
        Object *Object = SymbolTable[currLevel];
        while (Object != nullptr)
        {
            if (Object->token_secundario == token_sec)
                break;
            else
                Object = Object->next;
        }
        return Object;
    }

    Object *search_in_all_levels(int token_sec)
    { // funcao Find na apostila
        Object *Object = nullptr;
        for (int i = currentLevel; i >= 0; i--)
        {
            Object = search_in_level(token_sec, i);
            if (Object != nullptr && Object->token_secundario == token_sec)
                break;
        }
        return Object;
    }

    bool check_types(Object *t1, Object *t2) // CheckTypes
    {
        if (t1 == t2)
            return true;
        if (t1 == universal_object || t2 == universal_object)
            return true;
        if (t1->kind == UNIVERSAL_ || t2->kind == UNIVERSAL_)
            return true;
        if (t1->kind == t2->kind)
        {
            if (t1->kind == ALIAS_TYPE_)
            {
                Alias t1_Alias = get<Alias>(t1->variant);
                Alias t2_Alias = get<Alias>(t2->variant);
                return check_types(t1_Alias.base_type, t2_Alias.base_type);
            }
            if (t1->kind == ARRAY_TYPE_)
            {
                Array t1_array = get<Array>(t1->variant);
                Array t2_array = get<Array>(t2->variant);
                if (t1_array.num_elements == t2_array.num_elements)
                {
                    return check_types(t1_array.element_type, t2_array.element_type);
                }
            }
            else if (t1->kind == STRUCT_TYPE_)
            {
                Struct t1_struct = get<Struct>(t1->variant);
                Struct t2_struct = get<Struct>(t2->variant);
                Object *f1 = t1_struct.fields;
                Object *f2 = t2_struct.fields;
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

void Error(int line, error_type error)
{
    bool has_err = true;
    cout << "Line: " << line << " - ";
    switch (error)
    {
    case ERR_NO_DECL:
        printf("Variable not declared");
        break;
    case ERR_REDCL:
        printf("Variable already declared");
        break;
    case ERR_TYPE_EXPECTED:
        printf("Type not declared");
        break;
    case ERR_BOOL_TYPE_EXPECTED:
        printf("Expected Type boolean");
        break;
    case ERR_INVALID_TYPE:
        printf("Invalid Type for this operation");
        break;
    case ERR_TYPE_MISMATCH:
        printf("Invalid Type for this operation");
        break;
    case ERR_KIND_NOT_STRUCT:
        printf("Only Struct Types are allowed for this operation");
        break;
    case ERR_FIELD_NOT_DECL:
        printf("Field not declared");
        break;
    case ERR_KIND_NOT_ARRAY:
        printf("Only Array Types are allowed for this operation");
        break;
    case ERR_INVALID_INDEX_TYPE:
        printf("Invalid Index for Array");
        break;
    case ERR_KIND_NOT_VAR:
        printf("Only Var Types are allowed for this operation");
        break;
    case ERR_KIND_NOT_FUNCTION:
        printf("Only Function Types are allowed for this operation");
        break;
    case ERR_TOO_FEW_ARGS:
        printf("Number of parameters less than the specified value");
        break;
    case ERR_TOO_MANY_ARGS:
        printf("Number of parameters greater than the specified value");
        break;
    case ERR_PARAM_TYPE:
        printf("Invalid Specified Type");
        break;
    case ERR_RETURN_TYPE_MISMATCH:
        printf("Return Type not compatible with the specified function return Type");
        break;
    }
    printf("\n");
}

int main()
{
    printf("dale");
}
