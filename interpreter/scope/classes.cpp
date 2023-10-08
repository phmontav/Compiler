#include <vector>
#include <variant>
using namespace std;
typedef enum
{
    NO_KIND_DEF_ = -1,
    VAR_,
    PARAM_,
    FUNCTION_,
    FIELD_,
    ARRAY_TYPE_,
    STRUCT_TYPE_,
    ALIAS_TYPE_,
    SCALAR_TYPE_,
    UNIVERSAL_
} token_type; // t_kind

struct Var
{
    Node *type; // pType
};

struct Param
{
    Node *type; // pType
};

struct Field
{
    Node *type; // pType
};
struct Function
{
    Node *return_type; // pRetType
    Node *params;      // pParams
};

struct Array
{
    Node *element_type; // pElemType
    int num_elements;   // nNumElements
};

struct Struct
{
    Node *fields; // Pfields
};

struct Alias
{
    Node *base_type; // pBaseType
};

class Node
{
public:
    int token_secundario; // nName
    Node *next;           // pNext
    token_type type;      // eKind
    variant<Var, Param, Field, Function, Array, Struct, Alias> variant;
    Node(int token_sec, token_type t_type)
    {
        token_secundario = token_sec;
        next = nullptr;
        type = t_type;
    }
};

Node *int_node = new Node(-1, SCALAR_TYPE_);
Node *char_node = new Node(-1, SCALAR_TYPE_);
Node *bool_node = new Node(-1, SCALAR_TYPE_);
Node *string_node = new Node(-1, SCALAR_TYPE_);
Node *universal_node = new Node(-1, UNIVERSAL_);
