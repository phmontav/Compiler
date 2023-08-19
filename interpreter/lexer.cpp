#include <iostream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <algorithm>
#define X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE(r, data, elem) \
    case elem:                                                             \
        return BOOST_PP_STRINGIZE(elem);

#define DEFINE_ENUM_WITH_STRING_CONVERSIONS(name, enumerators)       \
    enum name                                                        \
    {                                                                \
        BOOST_PP_SEQ_ENUM(enumerators)                               \
    };                                                               \
    inline const char *ToString(name v)                              \
    {                                                                \
        switch (v)                                                   \
        {                                                            \
            BOOST_PP_SEQ_FOR_EACH(                                   \
                X_DEFINE_ENUM_WITH_STRING_CONVERSIONS_TOSTRING_CASE, \
                name,                                                \
                enumerators)                                         \
        default:                                                     \
            return "[Unknown " BOOST_PP_STRINGIZE(name) "]";         \
        }                                                            \
    }
using namespace std;

DEFINE_ENUM_WITH_STRING_CONVERSIONS(TokenType,
                                    (ARRAY)(BOOLEAN)(BREAK)(CHAR)(CONTINUE)(DO)(ELSE)(FALSE)(FUNCTION)(IF)(INTEGER)(OF)(STRING)(STRUCT)(TRUE)(TYPE)(VAR)(WHILE)(COLON)(SEMI_COLON)(COMMA)(EQUALS)(LEFT_SQUARE)(RIGHT_SQUARE)(LEFT_BRACES)(RIGHT_BRACES)(LEFT_PARENTHESIS)(RIGHT_PARENTHESIS)(AND)(OR)(LESS_THAN)(GREATER_THAN)(LESS_OR_EQUAL)(GREATER_OR_EQUAL)(NOT_EQUAL)(EQUAL_EQUAL)(PLUS)(PLUS_PLUS)(MINUS)(MINUS_MINUS)(TIMES)(DIVIDE)(DOT)(NOT)(CHARATER)(NUMERAL)(STRINGVAL)(ID)(UNKNOWN))

class Token
{
public:
    TokenType tokenType;
    int value;
};

class Lexer
{
public:
    string text;
    vector<string> TokenTypeVector;
    Lexer(string &text)
    {
        this->text = text;
        Lexer();
    }
    Lexer()
    {
        for (int i = ARRAY; i <= UNKNOWN; i++)
        {
            TokenType e = static_cast<TokenType>(i);
            TokenTypeVector.push_back(ToString(e));
        }
        sort(TokenTypeVector.begin(), TokenTypeVector.end());
    }
};

int main()
{
    Lexer();
}
