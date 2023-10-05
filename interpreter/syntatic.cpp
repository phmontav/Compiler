//#include "lexer.cpp"
#include <bits/stdc++.h>

using namespace std;

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
            for(int i = 0; i < token.size(); i++){
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

int main(){
    Syntatic syntatic;
    syntatic.parse_csv();
    for(int i = 0; i < 182; i++){
        for(int j = 0; j < 89; j++){
            cout << syntatic.action[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}