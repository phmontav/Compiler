//#include "lexer.cpp"
#include "lexer.cpp"
#include <string>

using namespace std;

vector<int> RIGHT = {1,		2,		1,		1,		1,		1,		1,		1,		1,		1,		9,		8,		4,		5,		3,		10,		5,		3,		4,		2,		1,		2,		1,		5,		3,		1,		1,		1,		6,		9,		9,		7,		8,		2,		4,		2,		2,		3,		3,		1,		3,		3,		3,		3,		3,		3,		1,		3,		3,		1,		3,		3,		1,		1,		2,		2,		2,		2,		3,		5,		2,		2,		1,		1,		1,		1,		1,		3,		1,		3,		4,		1,		1,		1,		1,		1,			1,			1,		1,		1,		0,		0,		0,		0,		0,		0,		0};
vector<TokenType> LEFT =  {P,     LDE,	LDE,	DE,	    DE,	    T,	    T,	    T,      T,  	T,  	DT, 	DT, 	DT, 	DC, 	DC, 	DF, 	LP, 	LP, 	B,  	LDV,	LDV,	LS, 	LS, 	DV, 	LI, 	LI, 	S,  	S,  	U,  	U,  	M,	    M,  	M,  	M,  	M,  	M,  	M,  	E,  	E,  	E,  	L,  	L,  	L,  	L,	    L,  	L,  	L,  	R,  	R,	    R,  	Y,  	Y,  	Y,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,  	F,	    LE, 	LE, 	LV,	    LV, 	LV,	    IDD,	IDU,	ID, 	TRUE,   	FALSE,  	CHR,    STR,	NUM,     NB,   	MF,	    MC,	    NF,	    MT, 	ME,	    MW};

class Syntatic{
public:
    vector<vector<string>> action;
    int rows = 0, columns = 0;

    Syntatic() : rows(0), columns(0) {};

    void parse_csv(){
        freopen("action_table.csv", "r", stdin);
        //remove, just for test parsing  
        freopen("out", "w", stdout);
        string token;
        while(getline(cin, token)){
            string cur;
            columns = 0;
            action.push_back({});
            for(int i = 0; i < static_cast<int>(token.size()); i++){
                if(token[i] == 9){
                    action[rows].push_back(cur);
                    cur.clear();
                    columns++;
                    continue;
                }
                cur.push_back(token[i]);
            }
            action[rows].push_back(cur);
            rows++, columns++;
        }
    }
};

// int main(){
//     Syntatic syntatic;
//     syntatic.parse_csv();
//     for(int i = 0; i < 182; i++){
//         for(int j = 0; j < 89; j++){
//             cout << syntatic.action[i][j] << " ";
//         }
//         cout << "\n";
//     }
//     return 0;
// }