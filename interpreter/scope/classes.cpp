#include <vector>
#include <variant>

#define IS_TYPE_KIND(k)                                                 \
    ((k) == ARRAY_TYPE_ || (k) == STRUCT_TYPE_ || (k) == ALIAS_TYPE_ || \
     (k) == SCALAR_TYPE_)

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
} token_kind; // t_kind

typedef enum
{
    _B,
    _C,
    _DC,
    _DE,
    _DF,
    _DT,
    _DV,
    _E,
    _F,
    _FALSE,
    _IDD,
    _IDU,
    _L,
    _LDE,
    _LDV,
    _LE,
    _LI,
    _LP,
    _LS,
    _LV,
    _MC,
    _ME,
    _MF,
    _MT,
    _MW,
    _NB,
    _NF,
    _NUM,
    _P,
    _R,
    _S,
    _STR,
    _T,
    _TRUE,
    _Y,
} non_term;

struct Var
{
    Object *type; // pType
    int nSize;
    int nIndex;
};

struct Param
{
    Object *type; // pType
    int nIndex;
    int nSize;
};

struct Field
{
    Object *type; // pType
    int nSize;
    int nIndex;
};
struct Function
{
    Object *return_type; // pRetType
    Object *params;      // pParams
    int nVars;
    int nIndex;
    int nParams;
};

struct Array
{
    Object *element_type; // pElemType
    int num_elements;     // nNumElements
    int nSize;
};

struct Struct
{
    Object *fields; // Pfields
    int nSize;
};

struct Alias
{
    Object *base_type; // pBaseType
    int nSize;
};

struct Type
{
    Object *base_type; // pBaseType
    int nSize;
};

class Object
{
public:
    int token_secundario; // nName
    Object *next;         // pNext
    token_kind kind;      // eKind
    variant<Var, Param, Field, Function, Array, Struct, Alias> variant;
    Object();
    Object(int token_sec, token_kind t_kind)
    {
        token_secundario = token_sec;
        next = nullptr;
        kind = t_kind;
    }
};

typedef enum
{
    DF,
    DE,
    DC,
    DV,
    DT,
    TRUE,
    IDD,
    LE,
    LDE,
    NB,
    NF,
    LI,
    LV,
    NUM,
    LS,
    LP,
    LDV,
    B,
    E,
    F,
    IDU,
    L,
    P,
    S,
    R,
    T,
    STR,
    Y,
    ID,
    ME,
    MF,
    FALSE,
    MC,
    CHR,
    MT,
    MW
} t_nonterminal;
typedef struct
{
    t_nonterminal type;
    int nSize;
    union
    {
        struct
        {
            Object *obj;
            int name;
        } ID;

        struct
        {
            Object *type;
        } T, E, L, R, Y, F, LV;

        struct
        {
            Object *type;
            Object *param;
            int err;
        } MC;

        struct
        {
            int label;
        } MT, ME, MW, MA;

        struct
        {
            Object *type;
            Object *param;
            int err;
            int n;
        } LE;

        struct
        {
            Object *list;
        } LI, DC, LP;

        struct
        {
            Object *type;
            int val;
        } TRU, FALS;

        struct
        {
            Object *type;
            int pos;
            char val;
        } CHR;

        struct
        {
            Object *type;
            char *val;
            int pos;
        } STR;

        struct
        {
            Object *type;
            int val;
            int pos;
        } NUM;
    };
} t_attrib;
Object *int_object = new Object(-1, SCALAR_TYPE_);
Object *char_object = new Object(-1, SCALAR_TYPE_);
Object *bool_object = new Object(-1, SCALAR_TYPE_);
Object *string_object = new Object(-1, SCALAR_TYPE_);
Object *universal_object = new Object(-1, UNIVERSAL_);
