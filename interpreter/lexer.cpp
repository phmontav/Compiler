#include <iostream>
#include <fstream>
#include <vector>
#include <boost/preprocessor.hpp>
#include <algorithm>
#include <string.h>
#include <unordered_map>
#include <climits>
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
                                    (CHARACTER)(NUMERAL)(STRINGVAL)(ID)
                                    //token desconhecido
                                    (UNKNOWN))

typedef struct {
    byte type; // 0-char, 1-int, 2- string
    union _{
        char cVal;
        int nVal;
        char * sVal;
    } _;
} t_const;

class Lexer{
public:
    // Defina a tabela de identificadores como um mapa de hash que associa cada nome a um número inteiro
    unordered_map<string, int> names;

    // Defina a tabela de constantes como um vetor que armazena as estruturas t_const
    vector<t_const> constants;

    // Definindo as constantes globais
    const static int MAX_LENGTH = 256; // tamanho máximo de um lexema
    const static int KEYWORDS_SIZE = 18; // número de palavras reservadas
    const char* KEYWORDS[KEYWORDS_SIZE] = { // vetor de palavras reservadas ordenado alfabeticamente
        "ARRAY", "BOOLEAN", "BREAK", "CHAR", "CONTINUE", "DO", "ELSE", "FALSE", 
        "FUNCTION", "IF", "INTEGER", "OF", "STRING", "STRUCT", "TRUE", "TYPE", 
        "VAR", "WHILE"
    };

    // Definindo as variáveis globais
    ifstream input; // arquivo de entrada
    ofstream output; // arquivo de saída
    char nextChar = '\x20';
    TokenType token;
    int tokenSecundario;

    Lexer() = default;
    // Função para buscar uma palavra na tabela de palavras reservadas
    TokenType searchKeyWord(char* name) {
        // Busca binária no vetor de palavras reservadas
        int low = 0;
        int high = KEYWORDS_SIZE - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            int cmp = strcmp(name, KEYWORDS[mid]);
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
    int searchName(char* name) {
        // Busca na tabela de nomes usando um algoritmo de hash
        string key(name);
        auto it = names.find(key);
        if (it != names.end()) {
            // Identificador encontrado, retorna o seu nome (token secundário)
            return it->second;
        }
        
        // Identificador não encontrado, insere na tabela com um novo nome
        int newName = static_cast<int>(names.size());
        names[key] = newName;
        return newName;
    }

    // Define uma função para inserir uma constante char na tabela de constantes e retornar seu índice no vetor
    int addCharConst( char c ) {
        //Cria constante com tipo especificado
        t_const constant;
        constant.type = static_cast<byte>(0);
        constant._.cVal = c;
        // Insere a constante no final do vetor de constantes
        constants.push_back(constant);
        // Retorna o índice da constante inserida (igual ao tamanho atual do vetor menos um)
        return static_cast<int>(constants.size()) - 1;
    }
    
    // Define uma função para inserir uma constante char na tabela de constantes e retornar seu índice no vetor
    int addIntConst( int n ) {
        //Cria constante com tipo especificado
        t_const constant;
        constant.type = static_cast<byte>(1);
        constant._.nVal = n;
        // Insere a constante no final do vetor de constantes
        constants.push_back(constant);
        // Retorna o índice da constante inserida (igual ao tamanho atual do vetor menos um)
        return static_cast<int>(constants.size()) - 1;
    }

    // Define uma função para inserir uma constante char na tabela de constantes e retornar seu índice no vetor
    int addStringConst( char* s ) {
        //Cria constante com tipo especificado
        t_const constant;
        constant.type = static_cast<byte>(2);
        constant._.sVal = s;
        // Insere a constante no final do vetor de constantes
        constants.push_back(constant);
        // Retorna o índice da constante inserida (igual ao tamanho atual do vetor menos um)
        return static_cast<int>(constants.size()) - 1;
    }

    //Recuperação do valor de uma constante a partir da sua posição
    t_const getConst( int n ) {
        // Retorna a constante através do índice fornecido
        return constants[n];
    }

    // Defina uma função para ler o próximo caractere do arquivo de entrada e retorná-lo
    char readChar() {
        // Declara uma variável para armazenar o caractere lido
        char c;
        // Lê o próximo caractere do arquivo de entrada
        if(input.get(c))
            return c;
        // Retorna o caractere lido
        return EOF;
    }

    // Função para ler o próximo token do arquivo de entrada
    TokenType nextToken() {
        // loop do estado inicial para pular os separadores
        while (isspace(nextChar)) {
            nextChar = readChar();
        }
        if(nextChar == EOF){
            token = UNKNOWN;
            tokenSecundario = INT_MAX;
        }
        else if (isalpha(nextChar)) { // se for uma letra
            char text[MAX_LENGTH + 1]; // buffer para armazenar o lexema
            int i = 0; // índice do buffer
            do {
                text[i++] = nextChar; // copia o caractere para o buffer
                nextChar = readChar(); // lê o próximo caractere
            } while (isalnum(nextChar) || nextChar == '_'); // enquanto for letra, dígito ou _
            text[i] = '\0'; // termina a string com o caractere nulo
            token = searchKeyWord(text); // busca na tabela de palavras reservadas
            if (token == ID) { // se for um identificador
                tokenSecundario = searchName(text); // busca ou insere na tabela de nomes
            }
        }
        else if (isdigit(nextChar)) { // se for um dígito
            char numeral[MAX_LENGTH + 1]; // buffer para armazenar o lexema
            int i = 0; // índice do buffer
            do {
                numeral[i++] = nextChar; // copia o caractere para o buffer
                nextChar = readChar(); // lê o próximo caractere
            } while (isdigit(nextChar)); // enquanto for dígito
            numeral[i] = '\0'; // termina a string com o caractere nulo
            token = NUMERAL; // define o tipo do token como numeral
            tokenSecundario = addIntConst(atoi(numeral)); // converte a string em inteiro e insere na tabela de constantes
        }
        else if (nextChar == '"') { // se for uma aspa dupla
            char string[MAX_LENGTH + 1]; // buffer para armazenar o lexema
            int i = 0; // índice do buffer
            string[i++] = nextChar; // copia a aspa dupla inicial para o buffer
            nextChar = readChar(); // lê o próximo caractere
            while (nextChar != '"') { // enquanto não for outra aspa dupla
                string[i++] = nextChar; // copia o caractere para o buffer
                nextChar = readChar(); // lê o próximo caractere
            }
            string[i++] = nextChar; // copia a aspa dupla final para o buffer
            string[i] = '\0'; // termina a string com o caractere nulo
            token = STRINGVAL; // define o tipo do token como stringval
            tokenSecundario = addStringConst(string); // insere a string na tabela de constantes
        }
        else switch (nextChar) { // se for um símbolo ou outro caractere inválido
            case '\'':
                nextChar = readChar();
                token = CHARACTER;
                tokenSecundario = addCharConst(nextChar);
                nextChar = readChar(); // pular o '\''
                nextChar = readChar();
                break;
            case ':':
                nextChar = readChar();
                token = COLON;
                break;
            case '+':
                nextChar = readChar();
                if (nextChar == '+') {
                    token = PLUS_PLUS;
                    nextChar = readChar();
                }
                else {
                    token = PLUS;
                }
                break;
            case ';':
                nextChar = readChar();
                token = SEMI_COLON;
                break;
            case ',':
                nextChar = readChar();
                token = COMMA;
                break;
            case '=':
                nextChar = readChar();
                if (nextChar == '=') {
                    token = EQUAL_EQUAL;
                    nextChar = readChar();
                }
                else {
                    token = EQUALS;
                }
                break;
            case '[':
                nextChar = readChar();
                token = LEFT_SQUARE;
                break;
            case ']':
                nextChar = readChar();
                token = RIGHT_SQUARE;
                break;
            case '{':
                nextChar = readChar();
                token = LEFT_BRACES;
                break;
            case '}':
                nextChar = readChar();
                token = RIGHT_BRACES;
                break;
            case '(':
                nextChar = readChar();
                token = LEFT_PARENTHESIS;
                break;
            case ')':
                nextChar = readChar();
                token = RIGHT_PARENTHESIS;
                break;
            case '&':
                nextChar = readChar();
                if (nextChar == '&') {
                    token = AND;
                    nextChar = readChar();
                }
                break;
            case '|':
                nextChar = readChar();
                if (nextChar == '|') {
                    token = OR;
                    nextChar = readChar();
                }
                break;
            case '<':
                nextChar = readChar();
                if (nextChar == '=') {
                    token = LESS_OR_EQUAL;
                    nextChar = readChar();
                }
                else if (nextChar == '>') {
                    token = NOT_EQUAL;
                    nextChar = readChar();
                }
                else {
                    token = LESS_THAN;
                }
                break;
            case '>':
                nextChar = readChar();
                if (nextChar == '=') {
                    token = GREATER_OR_EQUAL;
                    nextChar = readChar();
                }
                else {
                    token = GREATER_THAN;
                }
                break;
            case '-':
                nextChar = readChar();
                if (nextChar == '-') {
                    token = MINUS_MINUS;
                    nextChar = readChar();
                }
                else {
                    token = MINUS;
                }
                break;
            case '*':
                nextChar = readChar();
                token = TIMES;
                break;
            case '/':
                nextChar = readChar();
                token = DIVIDE;
                break;
            case '.':
                nextChar = readChar();
                token = DOT;
                break;
            case '!':
                nextChar = readChar();
                if (nextChar == '=') {
                    token = NOT_EQUAL;
                    nextChar = readChar();
                }
                else {
                    token = NOT;
                }
                break;
            // Aqui vão os casos para os tokens regulares e o token desconhecido...
            default:
                token = UNKNOWN;
                break;
        }
        return token;
    }
};

int main() {
    Lexer lexer;
    lexer.input.open("input.txt", ifstream::in);
    string s;
    while(true){
        lexer.nextToken();
        if(lexer.tokenSecundario == INT_MAX) break;
        cout << "Token: " << ToString(lexer.token);
        if(lexer.token == ID){
            cout << " , Attribute: " << lexer.tokenSecundario;
        }
        cout << endl;
    }
    return 0;
}
