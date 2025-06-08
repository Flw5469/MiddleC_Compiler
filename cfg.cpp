#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include <queue>
#include <map>

// directly implemeted from https://en.wikipedia.org/wiki/CYK_algorithm
// make sure to convert grammar into chomsky norm form

using namespace std;
template <typename T>
struct triplet {
    T first;
    T second;
    T third;
};

int main(){
  const int N = 10;
  const int R = 4;
  bool P[N][N][R] = {};
  queue<triplet<int>> back[N][N][R];
  pair<string, vector<vector<string>>> rules[R] = {
      {"A", {{"B", "C"}, {"A","A"}, {"n"}}},
      {"B", {{"C", "D"}, {"B", "C"}, {"a"}}},
      {"C", {{"n"},{"v"}}},
      {"D", {{"i"}}},
  };
  
  map<string, int> m;
  for (int i=0;i<R;i++){
    if (m.find(rules[i].first)==m.end()){
      m.insert({rules[i].first, i});
    }
  }

  string input[N] = {"n", "n", "n","n", "n", "n","n", "n", "a","v"};

// Terminal productions
for (int s = 0; s < N; s++) {
    for (int v = 0; v < R; v++) {
        for (const auto& rhs : rules[v].second) {
            if (rhs.size() == 1 && rhs[0] == input[s]) {
                P[0][s][v] = true;  // Length 1, starting at position s
                cout << "Terminal match: " << input[s] << " as " << rules[v].first << endl;
            }
        }
    }
}

// Binary productions
for (int l = 1; l < N; l++) {          // Length-1 of span (0-indexed)
    for (int s = 0; s < N-l; s++) {    // Start of span (0-indexed)
        for (int p = 0; p < l; p++) {  // Partition point (0-indexed)
            for (int a = 0; a < R; a++) {
                for (const auto& rhs : rules[a].second) {
                    if (rhs.size() == 2) {
                        int b = m[rhs[0]];  // Non-terminal index for first part 
                        int c = m[rhs[1]];  // Non-terminal index for second part
                        
                        if (P[p][s][b] && P[l-p-1][s+p+1][c]) {
                            P[l][s][a] = true;
                            triplet<int> path = {p, b, c};
                            back[l][s][a].push(path);
                            cout << "Binary match at [" << l << "," << s << "," << a << "] " 
                                 << "using partition " << p << " with " 
                                 << rules[b].first << " and " << rules[c].first << endl;
                        }
                    }
                }
            }
        }
    }
}

  cout<<"answer: "<<P[N-1][0][0]<<endl;
  if (P[N-1][0][0]) cout<<"YES\n"; else cout<<"NO\n"<<endl;
  // while (!back[N-1][0][0].empty()){
  //   struct triplet<int> path = back[N-1][0][0].front();
  //   cout<<"path: "<<path.first<<" "<<path.second<<" "<<path.third<<endl;
  //   back[N-1][0][0].pop();
  // }

  for (int i=0;i<N;i++){
    for (int j=0;j<10;j++){
      for (int k=0;k<4;k++){
        if (!back[i][j][k].empty())
        cout<<"\n\n for "<<i<<" "<<j<<" "<<k<<endl;
        while (!back[i][j][k].empty()){
    struct triplet<int> path = back[i][j][k].front();
    cout<<"path: "<<path.first<<" "<<path.second<<" "<<path.third<<endl;
    back[i][j][k].pop();
  }
      }
    }
  }

}