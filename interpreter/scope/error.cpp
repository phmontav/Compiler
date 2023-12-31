typedef enum
{
    ERR_REDCL,
    ERR_NO_DECL,
    ERR_TYPE_EXPECTED,
    ERR_BOOL_TYPE_EXPECTED,
    ERR_TYPE_MISMATCH,
    ERR_INVALID_TYPE,
    ERR_KIND_NOT_STRUCT,
    ERR_FIELD_NOT_DECL,
    ERR_KIND_NOT_ARRAY,
    ERR_INVALID_INDEX_TYPE,
    ERR_KIND_NOT_VAR,
    ERR_KIND_NOT_FUNCTION,
    ERR_TOO_MANY_ARGS,
    ERR_PARAM_TYPE,
    ERR_TOO_FEW_ARGS,
    ERR_RETURN_TYPE_MISMATCH
} error_type;