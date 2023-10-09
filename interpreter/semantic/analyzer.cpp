#include "../scope/rules.cpp"
#include "../scope/analyzer.cpp"
#include "../syntatic.cpp"
#include <string>
#include <stack>

Object *curFunction;
int functionVarPos;
int nFuncs = 0;

FILE *out;

void open_file()
{
    out = fopen("ssl.ftw", "w");
    if (out == nullptr)
    {
        exit(0);
    }
}

void close_file()
{
    fclose(out);
    out = nullptr;
}

int new_label(void)
{
    static int label_number = 0;
    return label_number++;
}

void semantic_analysis(Lexer lexer, rules rule, Scope scope)
{
    static int name, n, l, l1, l2;
    static Object *p, *t, *f;
    static t_attrib IDD_static, IDU_static, ID_static, T_static, LI_static, LI0_static, LI1_static, TRU_static, FALS_static, STR_static, CHR_static, NUM_static, DC_static, DC0_static, DC1_static, LP_static, LP0_static, LP1_static, E_static, E0_static, E1_static, L_static, L0_static, L1_static, R_static, R0_static, R1_static, Y_static, Y0_static, Y1_static, F_static, F0_static, F1_static, LV_static, LV0_static, LV1_static, MC_static, LE_static, LE0_static, LE1_static, MT_static, ME_static, MW_static, NB_static;
    static stack<t_attrib> semantic_stack;
    int secondary_token = lexer.tokenSecundario;
    switch (rule)
    {
    case RULE_LDE_0: // LDE -> LDE DE
    case RULE_LDE_1: // LDE -> DE
    case RULE_DE_0:  // DE -> DF
    case RULE_DE_1:  // DE -> DT
        break;

    case RULE_T_0: // T -> TOKEN_INTEGER
        T_static.T.type = int_object;
        T_static.type = t_nonterminal::T;
        T_static.nSize = 1;
        semantic_stack.push(T_static);
        break;

    case RULE_T_1: // T -> TOKEN_CHAR
        T_static.T.type = char_object;
        T_static.type = t_nonterminal::T;
        T_static.nSize = 1;
        semantic_stack.push(T_static);
        break;

    case RULE_T_2: // T -> TOKEN_BOOLEAN
        T_static.T.type = bool_object;
        T_static.type = t_nonterminal::T;
        T_static.nSize = 1;
        semantic_stack.push(T_static);
        break;

    case RULE_T_3: // T -> TOKEN_STRING
        T_static.T.type = string_object;
        T_static.type = t_nonterminal::T;
        T_static.nSize = 1;
        semantic_stack.push(T_static);
        break;

    case RULE_T_4: // T -> IDU
        IDU_static = semantic_stack.top();
        p = IDU_static.ID.obj;
        semantic_stack.pop();

        if (IS_TYPE_KIND(p->kind) || p->kind == UNIVERSAL_)
        {
            T_static.T.type = p;
            if (p->kind == ALIAS_TYPE_)
            {
                T_static.nSize = get<Alias>(p->variant).nSize;
            }
            else if (p->kind == ARRAY_TYPE_)
            {
                T_static.nSize = get<Array>(p->variant).nSize;
            }
            else if (p->kind == STRUCT_TYPE_)
            {
                T_static.nSize = get<Struct>(p->variant).nSize;
            }
        }
        else
        {
            T_static.T.type = universal_object;
            T_static.nSize = 0;
            Error(lexer.line, ERR_TYPE_EXPECTED);
        }
        T_static.type = t_nonterminal::T;
        semantic_stack.push(T_static);
        break;

    case RULE_DT_0: // DT -> TOKEN_TYPE IDD TOKEN_EQUAL TOKEN_ARRAY TOKEN_LEFT_SQUARE NUM TOKEN_RIGHT_SQUARE TOKEN_OF T
        T_static = semantic_stack.top();
        semantic_stack.pop();
        NUM_static = semantic_stack.top();
        semantic_stack.pop();
        IDD_static = semantic_stack.top();
        semantic_stack.pop();

        p = IDD_static.ID.obj;
        n = NUM_static.NUM.val;
        t = T_static.T.type;

        p->kind = ARRAY_TYPE_;
        get<Array>(p->variant).num_elements = n;
        get<Array>(p->variant).element_type = t;
        get<Array>(p->variant).nSize = n * T_static.nSize;
        break;

    case RULE_DT_1: // DT -> TOKEN_TYPE IDD TOKEN_EQUAL TOKEN_STRUCT NB TOKEN_LEFT_BRACES DC TOKEN_RIGHT_BRACES
        DC_static = semantic_stack.top();
        semantic_stack.pop();
        IDD_static = semantic_stack.top();
        semantic_stack.pop();

        p = IDD_static.ID.obj;
        p->kind = STRUCT_TYPE_;
        get<Struct>(p->variant).fields = DC_static.DC.list;
        get<Struct>(p->variant).nSize = DC_static.nSize;
        scope.endBlock();
        break;

    case RULE_DT_2: // DT -> TOKEN_TYPE IDD TOKEN_EQUAL T
        T_static = semantic_stack.top();
        semantic_stack.pop();
        IDD_static = semantic_stack.top();
        semantic_stack.pop();

        p = IDD_static.ID.obj;
        t = T_static.T.type;

        p->kind = ALIAS_TYPE_;
        get<Alias>(p->variant).base_type = t;
        get<Alias>(p->variant).nSize = T_static.nSize;
        break;

    case RULE_DC_0: // DC -> DC TOKEN_SEMICOLON LI TOKEN_COLON T
        T_static = semantic_stack.top();
        semantic_stack.pop();
        LI_static = semantic_stack.top();
        semantic_stack.pop();
        DC1_static = semantic_stack.top();
        semantic_stack.pop();

        p = LI_static.LI.list;
        t = T_static.T.type;
        n = DC1_static.nSize;

        while (p != nullptr && p->kind == NO_KIND_DEF_)
        {
            p->kind = FIELD_;
            get<Field>(p->variant).type = t;
            get<Field>(p->variant).nIndex = n;
            get<Field>(p->variant).nSize = T_static.nSize;
            n = n + T_static.nSize;
            p = p->next;
        }

        DC0_static.DC.list = DC1_static.DC.list;
        DC0_static.nSize = n;
        DC0_static.type = t_nonterminal::DC;
        semantic_stack.push(DC0_static);
        break;

    case RULE_DC_1: // DC -> LI TOKEN_COLON T
        T_static = semantic_stack.top();
        semantic_stack.pop();
        LI_static = semantic_stack.top();
        semantic_stack.pop();

        p = LI_static.LI.list;
        t = T_static.T.type;
        n = 0;
        while (p != nullptr && p->kind == NO_KIND_DEF_)
        {
            p->kind = FIELD_;
            get<Field>(p->variant).type = t;
            get<Field>(p->variant).nSize = T_static.nSize;
            get<Field>(p->variant).nIndex = n;
            n = n + T_static.nSize;
            p = p->next;
        }
        DC_static.DC.list = LI_static.LI.list;
        DC_static.nSize = n;
        DC_static.type = t_nonterminal::DC;
        semantic_stack.push(DC_static);
        break;

    case RULE_DF_0: // DF -> TOKEN_FUNCTION IDD NB TOKEN_LEFT_PARENTHESIS LP TOKEN_RIGHT_PARENTHESIS TOKEN_COLON T MF B
        scope.endBlock();
        /*fpos_t currentPos;
          fgetpos(out,&currentPos);
          fsetpos(out,&functionVarPos);
          fprintf(out,"%d",f->_.Function.nVars-f->_.Function.nParams);
          fsetpos(out,&currentPos);
          fprintf(out,"END_FUNC\n");*/
        break;

    case RULE_LP_0: // LP -> LP TOKEN_COMMA IDD TOKEN_COLON T
        T_static = semantic_stack.top();
        semantic_stack.pop();
        IDD_static = semantic_stack.top();
        semantic_stack.pop();
        LP1_static = semantic_stack.top();
        semantic_stack.pop();

        p = IDD_static.ID.obj;
        t = T_static.T.type;
        n = LP1_static.nSize;

        p->kind = PARAM_;
        get<Param>(p->variant).type = t;
        get<Param>(p->variant).nIndex = n;
        get<Param>(p->variant).nSize = T_static.nSize;

        LP0_static.LP.list = LP1_static.LP.list;
        LP0_static.nSize = n + T_static.nSize;
        LP0_static.type = t_nonterminal::LP;
        semantic_stack.push(LP0_static);
        break;

    case RULE_LP_1: // LP -> IDD TOKEN_COLON T
        T_static = semantic_stack.top();
        semantic_stack.pop();
        IDD_static = semantic_stack.top();
        semantic_stack.pop();

        p = IDD_static.ID.obj;
        t = T_static.T.type;
        p->kind = PARAM_;
        get<Param>(p->variant).type = t;
        get<Param>(p->variant).nIndex = 0;
        get<Param>(p->variant).nSize = T_static.nSize;
        LP_static.LP.list = p;
        LP_static.nSize = T_static.nSize;
        LP_static.type = t_nonterminal::LP;

        semantic_stack.push(LP_static);
        break;

    case RULE_B_0: // B -> TOKEN_LEFT_BRACES LDV LS TOKEN_RIGHT_BRACES
        fprintf(out, "END_FUNC\n");

        int currentPos = ftell(out);
        fseek(out, functionVarPos, SEEK_SET);
        fprintf(out, "%02d", get<Function>(f->variant).nVars);
        fseek(out, currentPos, SEEK_SET);
        break;

    case RULE_LDV_0: // LDV -> LDV DV
    case RULE_LDV_1: // LDV -> DV
    case RULE_LS_0:  // LS -> LS S
    case RULE_LS_1:  // LS -> S
        break;

    case RULE_DV_0: // DV -> TOKEN_VAR LI TOKEN_COLON T TOKEN_SEMICOLON
        T_static = semantic_stack.top();
        t = T_static.T.type;
        semantic_stack.pop();
        LI_static = semantic_stack.top();
        semantic_stack.pop();
        p = LI_static.LI.list;
        n = get<Function>(curFunction->variant).nVars;

        while (p != nullptr && p->kind == NO_KIND_DEF_)
        {
            p->kind = VAR_;
            get<Var>(p->variant).type = t;
            get<Var>(p->variant).nSize = T_static.nSize;
            get<Var>(p->variant).nIndex = n;

            n += T_static.nSize;
            p = p->next;
        }

        get<Function>(curFunction->variant).nVars = n;
        break;

    case RULE_LI_0: // LI -> LI TOKEN_COMMA IDD
        IDD_static = semantic_stack.top();
        semantic_stack.pop();
        LI1_static = semantic_stack.top();
        semantic_stack.pop();
        LI0_static.LI.list = LI1_static.LI.list;
        LI0_static.type = t_nonterminal::LI;
        semantic_stack.push(LI0_static);
        break;

    case RULE_LI_1: // LI -> IDD
        IDD_static = semantic_stack.top();
        LI_static.LI.list = IDD_static.ID.obj;
        LI_static.type = t_nonterminal::LI;
        semantic_stack.pop();
        semantic_stack.push(LI_static);
        break;

    case RULE_MT_0: // MT -> ''
        l = new_label();
        MT_static.MT.label = l;
        MT_static.type = t_nonterminal::MT;
        fprintf(out, "\tTJMP_FW L%d\n", l);
        semantic_stack.push(MT_static);
        break;

    case RULE_S_0: // S -> TOKEN_IF TOKEN_LEFT_PARENTHESIS E TOKEN_RIGHT_PARENTHESIS S
        MT_static = semantic_stack.top();
        semantic_stack.pop();
        E_static = semantic_stack.top();
        semantic_stack.pop();

        t = E_static.E.type;
        if (!scope.check_types(t, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        fprintf(out, "L%d\n", MT_static.MT.label);
        break;

    case RULE_ME_0: // ME -> ''
        MT_static = semantic_stack.top();
        l1 = MT_static.MT.label;
        l2 = new_label();
        ME_static.ME.label = l2;
        ME_static.type = t_nonterminal::ME;
        semantic_stack.push(ME_static);

        fprintf(out, "\tTJMP_FW L%d\nL%d\n", l2, l1);
        break;

    case RULE_S_1: // S -> TOKEN_IF TOKEN_LEFT_PARENTHESIS E TOKEN_RIGHT_PARENTHESIS S TOKEN_ELSE S
        ME_static = semantic_stack.top();
        semantic_stack.pop();
        MT_static = semantic_stack.top();
        semantic_stack.pop();
        E_static = semantic_stack.top();
        semantic_stack.pop();

        l = ME_static.ME.label;

        t = E_static.E.type;
        if (!scope.check_types(t, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }
        fprintf(out, "L%d\n", l);
        break;
    case RULE_MW_0: // MW -> ''
        l = new_label();
        MW_static.MW.label = l;
        semantic_stack.push(MW_static);
        fprintf(out, "L%d\n", l);
        break;

    case RULE_S_2: // S -> TOKEN_WHILE TOKEN_LEFT_PARENTHESIS E TOKEN_RIGHT_PARENTHESIS S
        MT_static = semantic_stack.top();
        semantic_stack.pop();
        E_static = semantic_stack.top();
        semantic_stack.pop();
        MW_static = semantic_stack.top();
        semantic_stack.pop();

        l1 = MW_static.MW.label;
        l2 = MT_static.MT.label;

        t = E_static.E.type;
        if (!scope.check_types(t, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        fprintf(out, "\tJMP_BW L%d\nL%d\n", l1, l2);
        break;

    case RULE_S_3: // S -> TOKEN_DO S TOKEN_WHILE TOKEN_LEFT_PARENTHESIS E TOKEN_RIGHT_PARENTHESIS TOKEN_SEMICOLON
        E_static = semantic_stack.top();
        semantic_stack.pop();
        MW_static = semantic_stack.top();
        semantic_stack.pop();

        l = MW_static.MW.label;

        t = E_static.E.type;
        if (!scope.check_types(t, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        fprintf(out, "\tNOT\n\tTJMP_BW L%d\n", l);
        break;

    case RULE_S_4: // S -> NB B
        scope.endBlock();
        break;

    case RULE_S_5: // S -> LV TOKEN_EQUAL E TOKEN_SEMICOLON
        E_static = semantic_stack.top();
        semantic_stack.pop();
        LV_static = semantic_stack.top();
        semantic_stack.pop();
        t = LV_static.LV.type;

        if (!scope.check_types(LV_static.LV.type, E_static.E.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        fprintf(out, "\tSTORE_REF %d\n", get<Type>(t->variant).nSize);
        break;

    case RULE_S_6: // S -> TOKEN_BREAK TOKEN_SEMICOLON
    case RULE_S_7: // S -> TOKEN_CONTINUE TOKEN_SEMICOLON
        break;

    case RULE_E_0: // E -> E TOKEN_AND L
        L_static = semantic_stack.top();
        semantic_stack.pop();
        E1_static = semantic_stack.top();
        semantic_stack.pop();
        if (!scope.check_types(E1_static.E.type, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        if (!scope.check_types(L_static.L.type, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        E0_static.E.type = bool_object;
        E0_static.type = t_nonterminal::E;
        semantic_stack.push(E0_static);
        fprintf(out, "\tAND\n");
        break;

    case RULE_E_1: // E -> E TOKEN_OR L
        L_static = semantic_stack.top();
        semantic_stack.pop();
        E1_static = semantic_stack.top();
        semantic_stack.pop();
        if (!scope.check_types(E1_static.E.type, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        if (!scope.check_types(L_static.L.type, bool_object))
        {
            Error(lexer.line, ERR_BOOL_TYPE_EXPECTED);
        }

        E0_static.E.type = bool_object;
        E0_static.type = t_nonterminal::E;
        semantic_stack.push(E0_static);
        fprintf(out, "\tOR\n");
        break;

    case RULE_E_2: // E -> L
        L_static = semantic_stack.top();
        semantic_stack.pop();
        E_static.E.type = L_static.L.type;
        E_static.type = t_nonterminal::E;
        semantic_stack.push(E_static);
        break;

    case RULE_L_0: // L -> L TOKEN_LESS R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(L1_static.L.type, R_static.R.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        L0_static.L.type = bool_object;
        L0_static.type = t_nonterminal::L;
        semantic_stack.push(L0_static);

        fprintf(out, "\tLT\n");
        break;

    case RULE_L_1: // L -> L TOKEN_GREATER R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(L1_static.L.type, R_static.R.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        L0_static.L.type = bool_object;
        L0_static.type = t_nonterminal::L;
        semantic_stack.push(L0_static);
        fprintf(out, "\tGT\n");
        break;

    case RULE_L_2: // L -> L TOKEN_LESS_OR_EQUAL R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(L1_static.L.type, R_static.R.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        L0_static.L.type = bool_object;
        L0_static.type = t_nonterminal::L;
        semantic_stack.push(L0_static);
        fprintf(out, "\tLE\n");
        break;

    case RULE_L_3: // L -> L TOKEN_GREATER_OR_EQUAL R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(L1_static.L.type, R_static.R.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        L0_static.L.type = bool_object;
        L0_static.type = t_nonterminal::L;
        semantic_stack.push(L0_static);
        fprintf(out, "\tGE\n");
        break;

    case RULE_L_4: // L -> L TOKEN_EQUAL_EQUAL R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(L1_static.L.type, R_static.R.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        L0_static.L.type = bool_object;
        L0_static.type = t_nonterminal::L;
        semantic_stack.push(L0_static);
        fprintf(out, "\tEQ\n");
        break;

    case RULE_L_5: // L -> L TOKEN_NOT_EQUAL R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(L1_static.L.type, R_static.R.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }
        L0_static.L.type = bool_object;
        L0_static.type = t_nonterminal::L;
        semantic_stack.push(L0_static);
        fprintf(out, "\tNE\n");
        break;

    case RULE_L_6: // L -> R
        R_static = semantic_stack.top();
        semantic_stack.pop();
        L_static.L.type = R_static.R.type;
        L_static.type = t_nonterminal::L;
        semantic_stack.push(L_static);
        break;

    case RULE_R_0: // R -> R TOKEN_PLUS Y
        Y_static = semantic_stack.top();
        semantic_stack.pop();
        R1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(R1_static.R.type, Y_static.Y.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }

        if (!scope.check_types(R1_static.R.type, int_object) && !scope.check_types(R1_static.R.type, string_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        R0_static.R.type = R1_static.R.type;
        R0_static.type = t_nonterminal::R;
        semantic_stack.push(R0_static);
        fprintf(out, "\tADD\n");
        break;

    case RULE_R_1: // R -> R TOKEN_MINUS Y
        Y_static = semantic_stack.top();
        semantic_stack.pop();
        R1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(R1_static.R.type, Y_static.Y.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }

        if (!scope.check_types(R1_static.R.type, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        R0_static.R.type = R1_static.R.type;
        R0_static.type = t_nonterminal::R;
        semantic_stack.push(R0_static);
        fprintf(out, "\tSUB\n");
        break;

    case RULE_R_2: // R -> Y
        Y_static = semantic_stack.top();
        semantic_stack.pop();
        R_static.R.type = Y_static.Y.type;
        R_static.type = t_nonterminal::R;
        semantic_stack.push(R_static);
        break;
    case RULE_Y_0: // Y -> Y TOKEN_TIMES F
        F_static = semantic_stack.top();
        semantic_stack.pop();
        Y1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(Y1_static.Y.type, F_static.F.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }

        if (!scope.check_types(Y1_static.Y.type, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        Y0_static.Y.type = Y1_static.Y.type;
        Y0_static.type = t_nonterminal::Y;
        semantic_stack.push(Y0_static);
        fprintf(out, "\tMUL\n");
        break;

    case RULE_Y_1: // Y -> Y TOKEN_DIVIDE F
        F_static = semantic_stack.top();
        semantic_stack.pop();
        Y1_static = semantic_stack.top();
        semantic_stack.pop();

        if (!scope.check_types(Y1_static.Y.type, F_static.F.type))
        {
            Error(lexer.line, ERR_TYPE_MISMATCH);
        }

        if (!scope.check_types(Y1_static.Y.type, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        Y0_static.Y.type = Y1_static.Y.type;
        Y0_static.type = t_nonterminal::Y;
        semantic_stack.push(Y0_static);
        fprintf(out, "\tDIV\n");
        break;
    case RULE_Y_2: // Y -> F
        F_static = semantic_stack.top();
        semantic_stack.pop();
        Y_static.Y.type = F_static.F.type;
        Y_static.type = t_nonterminal::Y;
        semantic_stack.push(Y_static);
        break;
    case RULE_F_0: // F -> LV
        LV_static = semantic_stack.top();
        semantic_stack.pop();
        auto aux = LV_static.LV.type;
        n = get<Type>(aux->variant).nSize;

        F_static.F.type = LV_static.LV.type;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tDE_REF %d\n", n);
        break;
    case RULE_F_1: // F -> TOKEN_PLUS_PLUS LV
        LV_static = semantic_stack.top();
        semantic_stack.pop();
        t = LV_static.LV.type;
        if (!scope.check_types(t, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        F_static.F.type = int_object;
        F_static.type = t_nonterminal::F;
        fprintf(out, "\tDUP\n\tDUP\n\tDE_REF 1\n");
        fprintf(out, "\tINC\n\tSTORE_REF 1\n\tDE_REF 1\n");
        semantic_stack.push(F_static);
        break;
    case RULE_F_2: // F -> TOKEN_MINUS_MINUS LV
        LV_static = semantic_stack.top();
        semantic_stack.pop();
        t = LV_static.LV.type;
        if (!scope.check_types(t, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        F_static.F.type = LV_static.LV.type;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tDUP\n\tDUP\n\tDE_REF 1\n");
        fprintf(out, "\tDEC\n\tSTORE_REF 1\n\tDE_REF 1\n");
        break;
    case RULE_F_3: // F -> LV TOKEN_PLUS_PLUS
        LV_static = semantic_stack.top();
        semantic_stack.pop();
        t = LV_static.LV.type;
        if (!scope.check_types(t, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        F_static.F.type = LV_static.LV.type;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tDUP\n\tDUP\n\tDE_REF 1\n");
        fprintf(out, "\tINC\n\tSTORE_REF 1\n\tDE_REF 1\n");
        fprintf(out, "\tDEC\n");
        break;
    case RULE_F_4: // F -> LV TOKEN_MINUS_MINUS
        LV_static = semantic_stack.top();
        semantic_stack.pop();
        t = LV_static.LV.type;
        if (!scope.check_types(t, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        F_static.F.type = t;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tDUP\n\tDUP\n\tDE_REF 1\n");
        fprintf(out, "\tDEC\n\tSTORE_REF 1\n\tDE_REF 1\n");
        fprintf(out, "\tINC\n");
        break;
    case RULE_F_5: // F -> TOKEN_LEFT_PARENTHESIS E TOKEN_RIGHT_PARENTHESIS
        E_static = semantic_stack.top();
        semantic_stack.pop();

        F_static.F.type = E_static.E.type;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        break;
    case RULE_F_6: // F -> IDU MC TOKEN_LEFT_PARENTHESIS LE TOKEN_RIGHT_PARENTHESIS
        LE_static = semantic_stack.top();
        semantic_stack.pop();
        MC_static = semantic_stack.top();
        semantic_stack.pop();
        IDU_static = semantic_stack.top();
        semantic_stack.pop();
        f = IDU_static.ID.obj;
        F_static.F.type = MC_static.MC.type;
        if (!LE_static.LE.err)
        {
            if (LE_static.LE.n - 1 < get<Function>(f->variant).nParams && LE_static.LE.n != 0)
            {
                Error(lexer.line, ERR_TOO_FEW_ARGS);
            }
            else if (LE_static.LE.n - 1 > get<Function>(f->variant).nParams)
            {
                Error(lexer.line, ERR_TOO_MANY_ARGS);
            }
        }
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tCALL %d\n", get<Function>(f->variant).nIndex);
        break;
    case RULE_F_7: // F -> TOKEN_MINUS F
        F1_static = semantic_stack.top();
        semantic_stack.pop();

        t = F1_static.F.type;
        if (!scope.check_types(t, int_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        F0_static.F.type = t;
        F0_static.type = t_nonterminal::F;
        semantic_stack.push(F0_static);
        fprintf(out, "\tNEG\n");
        break;
    case RULE_F_8: // F -> TOKEN_NOT F
        F1_static = semantic_stack.top();
        semantic_stack.pop();

        t = F1_static.F.type;
        if (!scope.check_types(t, bool_object))
        {
            Error(lexer.line, ERR_INVALID_TYPE);
        }

        F0_static.F.type = t;
        F0_static.type = t_nonterminal::F;
        semantic_stack.push(F0_static);
        fprintf(out, "\tNOT\n");
        break;
    case RULE_F_9: // F -> TRUE
        TRU_static = semantic_stack.top();
        semantic_stack.pop();
        F_static.F.type = bool_object;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tLOAD_CONST %d\n", secondary_token);
        break;
    case RULE_F_10: // F -> FALSE
        FALS_static = semantic_stack.top();
        semantic_stack.pop();
        F_static.F.type = bool_object;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        fprintf(out, "\tLOAD_CONST %d\n", secondary_token);
        break;
    case RULE_F_11: // F -> CHR
        CHR_static = semantic_stack.top();
        semantic_stack.pop();
        F_static.F.type = char_object;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        n = secondary_token;
        fprintf(out, "\tLOAD_CONST %d\n", n);
        break;
    case RULE_F_12: // F -> STR
        STR_static = semantic_stack.top();
        semantic_stack.pop();
        F_static.F.type = string_object;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        n = secondary_token;
        fprintf(out, "\tLOAD_CONST %d\n", n);
        break;
    case RULE_F_13: // F -> NUM
        STR_static = semantic_stack.top();
        semantic_stack.pop();
        F_static.F.type = int_object;
        F_static.type = t_nonterminal::F;
        semantic_stack.push(F_static);
        n = secondary_token;
        fprintf(out, "\tLOAD_CONST %d\n", n);
        break;
    case RULE_LE_1: // LE -> E
        E_static = semantic_stack.top();
        semantic_stack.pop();
        MC_static = semantic_stack.top();

        LE_static.LE.param = nullptr;
        LE_static.LE.err = MC_static.MC.err;
        n = 1;
        if (!MC_static.MC.err)
        {
            p = MC_static.MC.param;
            if (p == nullptr)
            {
                Error(lexer.line, ERR_TOO_MANY_ARGS);
                LE_static.LE.err = true;
            }
            else
            {
                if (!scope.check_types(get<Param>(p->variant).type, E_static.E.type))
                {
                    Error(lexer.line, ERR_PARAM_TYPE);
                }
                LE_static.LE.param = p->next;
                LE_static.LE.n = n + 1;
            }
        }
        LE_static.type = t_nonterminal::LE;
        semantic_stack.push(LE_static);
        break;
    case RULE_LE_0: // LE -> LE TOKEN_COMMA E
        E_static = semantic_stack.top();
        semantic_stack.pop();
        LE1_static = semantic_stack.top();
        semantic_stack.pop();
        LE0_static.LE.param = nullptr;
        LE0_static.LE.err = L1_static.LE.err;

        n = LE1_static.LE.n;
        if (!LE1_static.LE.err)
        {
            p = LE1_static.LE.param;
            if (p == nullptr)
            {
                Error(lexer.line, ERR_TOO_MANY_ARGS);
                LE0_static.LE.err = true;
            }
            else
            {
                if (!scope.check_types(get<Param>(p->variant).type, E_static.E.type))
                {
                    Error(lexer.line, ERR_PARAM_TYPE);
                }
                LE0_static.LE.param = p->next;
                LE0_static.LE.n = n + 1;
            }
        }
        LE0_static.type = t_nonterminal::LE;
        semantic_stack.push(LE0_static);
        break;
    case RULE_LV_0: // LV -> LV TOKEN_DOT IDU
        ID_static = semantic_stack.top();
        semantic_stack.pop();
        LV1_static = semantic_stack.top();
        semantic_stack.pop();

        t = LV1_static.LV.type;
        if (t->kind != STRUCT_TYPE_)
        {
            if (t->kind != UNIVERSAL_)
            {
                Error(lexer.line, ERR_KIND_NOT_STRUCT);
            }
            LV0_static.LV.type = universal_object;
        }
        else
        {
            p = get<Struct>(t->variant).fields;
            while (p != nullptr)
            {
                if (p->token_secundario == ID_static.ID.name)
                {
                    break;
                }
                p = p->next;
            }
            if (p == nullptr)
            {
                Error(lexer.line, ERR_FIELD_NOT_DECL);
                LV0_static.LV.type = universal_object;
            }
            else
            {
                LV0_static.LV.type = get<Field>(p->variant).type;
                auto aux = LV0_static.LV.type;
                get<Type>(aux->variant).nSize = get<Field>(p->variant).nSize;
                fprintf(out, "\tADD %d\n", get<Field>(p->variant).nIndex);
            }
        }

        LV0_static.type = t_nonterminal::LV;
        semantic_stack.push(LV0_static);
        break;
    case RULE_LV_1: // LV -> LV TOKEN_LEFT_SQUARE E TOKEN_RIGHT_SQUARE
        E_static = semantic_stack.top();
        semantic_stack.pop();
        LV1_static = semantic_stack.top();
        semantic_stack.pop();

        t = LV1_static.LV.type;
        if (scope.check_types(t, string_object))
        {
            LV0_static.LV.type = char_object;
        }
        else if (t->kind != ARRAY_TYPE_)
        {
            if (t->kind != UNIVERSAL_)
            {
                Error(lexer.line, ERR_KIND_NOT_ARRAY);
            }
            LV0_static.LV.type = universal_object;
        }
        else
        {
            LV0_static.LV.type = get<Array>(t->variant).element_type;
            n = get<Type>(get<Array>(t->variant).element_type->variant).nSize;
            fprintf(out, "\tMUL %d\n\tADD\n", n);
        }

        if (!scope.check_types(E_static.E.type, int_object))
        {
            Error(lexer.line, ERR_INVALID_INDEX_TYPE);
        }

        LV0_static.type = t_nonterminal::LV;
        semantic_stack.push(LV0_static);
        break;

    case RULE_LV_2: // LV -> IDU
        IDU_static = semantic_stack.top();
        semantic_stack.pop();

        p = IDU_static.ID.obj;
        if (p->kind != VAR_ && p->kind != PARAM_)
        {
            if (p->kind != UNIVERSAL_)
            {
                Error(lexer.line, ERR_KIND_NOT_VAR);
            }
            LV_static.LV.type = universal_object;
        }
        else
        {
            LV_static.LV.type = get<Var>(p->variant).type;
            get<Type>(LV_static.LV.type->variant).nSize = get<Var>(p->variant).nSize;
            fprintf(out, "\tLOAD_REF %d\n", get<Var>(p->variant).nIndex);
        }
        LV_static.type = t_nonterminal::LV;

        t = LV_static.LV.type;

        semantic_stack.push(LV_static);
        break;

    case RULE_NB_0: // NB -> ''
        scope.newBlock();
        break;
    case RULE_NF_0: // NF -> ''
        IDD_static = semantic_stack.top();
        // TODO Não tem pop??
        f = IDD_static.ID.obj;
        f->kind = FUNCTION_;
        get<Function>(f->variant).nParams = 0;
        get<Function>(f->variant).nVars = 0;
        get<Function>(f->variant).nIndex = nFuncs++;
        scope.newBlock();
        break;

    case RULE_MF_0: // MF -> ''

        T_static = semantic_stack.top();
        semantic_stack.pop();
        LP_static = semantic_stack.top();
        semantic_stack.pop();
        IDD_static = semantic_stack.top();
        semantic_stack.pop();

        f = IDD_static.ID.obj;
        f->kind = FUNCTION_;
        get<Function>(f->variant).return_type = T_static.T.type;
        get<Function>(f->variant).params = LP_static.LP.list;
        get<Function>(f->variant).nParams = LP_static.nSize;
        get<Function>(f->variant).nVars = 0;
        curFunction = f;

        fprintf(out, "BEGIN_FUNC %d, %d, %02d\n", get<Function>(f->variant).nIndex,
                get<Function>(f->variant).nParams, 0);
        functionVarPos = ftell(out) - 3;

        /*fgetpos(out,&functionVarPos);
          fprintf(out,"\n");*/
        break;

    case RULE_MC_0: // MC -> ''
        IDU_static = semantic_stack.top();
        f = IDU_static.ID.obj;

        if (f->kind != FUNCTION_)
        {
            // Error(ERR_KIND_NOT_FUNCTION);
            MC_static.MC.type = universal_object;
            MC_static.MC.param = nullptr;
            MC_static.MC.err = true;
        }
        else
        {
            MC_static.MC.type = get<Function>(f->variant).return_type;
            MC_static.MC.param = get<Function>(f->variant).params;
            MC_static.MC.err = false;
        }
        MC_static.type = t_nonterminal::MC;
        semantic_stack.push(MC_static);
        break;

    case RULE_IDD_0: // IDD -> TOKEN_ID
        name = secondary_token;
        IDD_static.ID.name = name;
        if ((p = scope.search_in_level(name)) != nullptr)
        {
            Error(lexer.line, ERR_REDCL);
        }
        else
            p = scope.define(name);
        IDD_static.ID.obj = p;
        semantic_stack.push(IDD_static);
        break;

    case RULE_IDU_0: // IDU -> TOKEN_ID
        name = secondary_token;
        IDU_static.ID.name = name;

        if ((p = scope.search_in_all_levels(name)) == nullptr)
        {
            Error(lexer.line, ERR_NO_DECL);
            exit(0);
            p = scope.define(name);
        }
        IDU_static.ID.obj = p;
        semantic_stack.push(IDU_static);
        break;

    case RULE_ID_0: // ID -> TOKEN_ID
        name = secondary_token;
        ID_static.ID.name = name;
        ID_static.ID.obj = nullptr;
        semantic_stack.push(ID_static);
        break;
    case RULE_TRUE_0: // TRUE -> TOKEN_TRUE
        TRU_static.type = t_nonterminal::TRUE;
        TRU_static.TRU.val = true;
        TRU_static.TRU.type = bool_object;
        semantic_stack.push(TRU_static);
        break;
    case RULE_FALSE_0: // FALSE -> TOKEN_FALSE
        FALS_static.type = t_nonterminal::FALSE;
        FALS_static.FALS.val = false;
        FALS_static.FALS.type = bool_object;
        semantic_stack.push(FALS_static);
        break;
    case RULE_CHR_0: // CHR -> TOKEN_CHARACTER
        CHR_static.type = t_nonterminal::CHR;
        CHR_static.CHR.type = char_object;
        CHR_static.CHR.pos = secondary_token;
        CHR_static.CHR.val = lexer.constants[secondary_token]._.cVal;
        semantic_stack.push(CHR_static);
        break;
    case RULE_STR_0: // STR -> TOKEN_STRINGVAL
        STR_static.type = t_nonterminal::STR;
        STR_static.STR.type = string_object;
        STR_static.STR.pos = secondary_token;
        STR_static.STR.val = lexer.constants[secondary_token]._.sVal;
        semantic_stack.push(STR_static);
        break;
    case RULE_NUM_0: // NUM -> TOKEN_NUMERAL
        NUM_static.type = t_nonterminal::NUM;
        NUM_static.NUM.type = int_object;
        NUM_static.NUM.pos = secondary_token;
        NUM_static.NUM.val = lexer.constants[secondary_token]._.nVal;
        semantic_stack.push(NUM_static);
        break;
    }
}