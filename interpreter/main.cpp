#include "syntatic.cpp"

int main() {
    Syntatic syntatic;
    syntatic.parse_csv();
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