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

  for (int i=0;i<N;i++){
    for (int j=0;j<R;j++){
      for (int k=0;k<rules[j].second.size();k++){
        if (rules[j].second[k][0]==input[i]){
          P[1][i][j]=true;
          cout<<"matching: "<< input[i]<<endl;
        }
      }
    }
  }

  for (int l = 1; l<N;l++){
      for (int s = 0; s<= N-l-1;s++){
        for (int p = 0; p<=l-1;p++){
          // cout<< "checking" << p<<" "<<s<<"    and    ";
          // cout<< "checking" << l-p<<" "<<s+p<<endl;

      for (int i=0;i<R;i++){
      for (int k=0;k<rules[i].second.size();k++){
        if (rules[i].second[k].size()!=2) {
          continue;
        }
        else {
          string first = rules[i].second[k][0];
          string second = rules[i].second[k][1];

          // cout<< "checking" << p<<" "<<s<<" "<<m[first]<<"    and    ";
          // cout<< "checking" << l-p<<" "<<s+p<<" "<<m[second]<<endl;

          if (P[p][s][m[first]] && P[l-p-1][s+p+1][m[second]]){
            P[l][s][i] = true;
            // cout<<"YES!"<<endl;
            //cout<<"true for "<<l<<" "<<s<<" "<<i<<endl;
            struct triplet<int> path; path.first = l; path.second = m[first]; path.third = m[second];
            back[l][s][i].push(path);
            cout<<"setting answer for "<<l<<" "<<s<<" "<<i<<" "<<path.first<<" "<<path.second<<" "<<path.third<<endl;
          } else {
            //cout<<"false for"<<l<<" "<<s<<" "<<i<<endl;
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