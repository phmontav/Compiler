#include <iostream>
#include <fstream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <algorithm>
#include <string>
#include <unordered_map>
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
                                    //palavras reservadas
                                    (ARRAY)(BOOLEAN)(BREAK)(CHAR)(CONTINUE)(DO)(ELSE)(FALSE)(FUNCTION)(IF)(INTEGER)(OF)(STRING)(STRUCT)(TRUE)(TYPE)(VAR)(WHILE)
                                    //simbolos
                                    (COLON)(SEMI_COLON)(COMMA)(EQUALS)(LEFT_SQUARE)(RIGHT_SQUARE)(LEFT_BRACES)(RIGHT_BRACES)(LEFT_PARENTHESIS)(RIGHT_PARENTHESIS)(AND)(OR)(LESS_THAN)(GREATER_THAN)(LESS_OR_EQUAL)(GREATER_OR_EQUAL)(NOT_EQUAL)(EQUAL_EQUAL)(PLUS)(PLUS_PLUS)(MINUS)(MINUS_MINUS)(TIMES)(DIVIDE)(DOT)(NOT)
                                    //tokens regulares
                                    (CHARATER)(NUMERAL)(STRINGVAL)(ID)
                                    //token desconhecido
                                    (UNKNOWN))


typedef unsigned char u_byte;
typedef struct {
    u_byte type; // 0-char, 1-int, 2- string
    union {
        char cVal;
        int nVal;
        char * sVal;
    } _;
} t_const;

class Token{
    public:
        TokenType tokenType;
        int value;
};

class Lexer{
    public:

        // Defina a tabela de identificadores como um mapa de hash que associa cada nome a um número inteiro
        unordered_map<string, int> names;

        // Defina a tabela de constantes como um vetor que armazena as estruturas t_const
        vector<t_const> constants;

        // Definindo as constantes globais
        const static int MAX_LENGTH = 256; // tamanho máximo de um lexema
        const static int KEYWORDS_SIZE = 18; // número de palavras reservadas
        const string KEYWORDS[KEYWORDS_SIZE] = { // vetor de palavras reservadas ordenado alfabeticamente
            "ARRAY", "BOOLEAN", "BREAK", "CHAR", "CONTINUE", "DO", "ELSE", "FALSE", 
            "FUNCTION", "IF", "INTEGER", "OF", "STRING", "STRUCT", "TRUE", "TYPE", 
            "VAR", "WHILE"
        };

        // Definindo as variáveis globais
        ifstream input; // arquivo de entrada
        ofstream output; // arquivo de saída
        char lexeme[MAX_LENGTH]; // buffer para armazenar o lexema atual
        int line = 1; // contador de linhas
        int column = 0; // contador de colunas

        string text;
        vector<string> TokenTypeVector;
        Lexer(string &_text) {
            this->text = _text;
            Lexer();
        }
        Lexer() {
            for (int i = ARRAY; i <= UNKNOWN; i++)
            {
                TokenType e = static_cast<TokenType>(i);
                TokenTypeVector.push_back(ToString(e));
            }
            sort(TokenTypeVector.begin(), TokenTypeVector.end());
        }
        // Função para buscar uma palavra na tabela de palavras reservadas
        TokenType searchKeyWord(string name) {
            // Busca binária no vetor de palavras reservadas
            int low = 0;
            int high = KEYWORDS_SIZE - 1;
            while (low <= high) {
                int mid = (low + high) / 2;
                int cmp = name.compare(KEYWORDS[mid]);
                if (cmp == 0) {
                    // Palavra encontrada, retorna o token correspondente
                    return (TokenType) mid;
                }
                else if (cmp < 0) {
                    // Palavra é menor que o elemento do meio, busca na metade inferior
                    high = mid - 1;
                }
                else {
                    // Palavra é maior que o elemento do meio, busca na metade superior
                    low = mid + 1;
                }
            }
            // Palavra não encontrada, retorna o token de identificador
            
            return ID;
        }

        // Função para buscar ou inserir um identificador na tabela de nomes
        int searchName(string name) {
            // Busca na tabela de nomes usando um algoritmo de hash
            std::string key(name);
            auto it = names.find(key);
            if (it != names.end()) {
                // Identificador encontrado, retorna o seu nome (token secundário)
                return it->second;
            }
            else {
                // Identificador não encontrado, insere na tabela com um novo nome
                int newName = static_cast<int>(names.size());
                names[key] = newName;
                return newName;
            }
        }

        // Defina uma função para inserir uma constante na tabela de constantes e retornar seu índice no vetor
        int insertConst( t_const c ) {
            // Insere a constante no final do vetor de constantes
            constants.push_back(c);
            // Retorna o índice da constante inserida (igual ao tamanho atual do vetor menos um)
            return static_cast<int>(constants.size()) - 1;
        }

        // Defina uma função para ler o próximo caractere do arquivo de entrada e retorná-lo
        char nextChar() {
            // Declara uma variável para armazenar o caractere lido
            char c;
            // Lê o próximo caractere do arquivo de entrada
            input.get(c);
            // Retorna o caractere lido
            return c;
        }

        // Defina uma função para retornar o último caractere lido para o arquivo de entrada
        void backChar() {
            // Volta uma posição no arquivo de entrada
            input.unget();
        }
};

int main()
{
    Lexer();
}
