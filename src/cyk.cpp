#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <iterator>
#include <unordered_set>
#include <cstring>  // Add this at the top with your other includes

#include "helper_functions.cpp"
#include "structures.cpp"
using namespace std;

void terminal_production(int N, int R, std::vector<std::string> input, vector<pair<string, vector<vector<string>>>> rules, unordered_set<string> &terminators, bool*** P){
  for (int s = 0; s < N; s++) {
    for (int v = 0; v < R; v++) {
        for (const auto& rhs : rules[v].second) {
            if (rhs.size() == 1 && s < N && handleRules(rhs[0],input[s], terminators)) {
                P[0][s][v] = true;  // Length 1, starting at position s
                cout << "Terminal match: " << input[s] << " as " << rules[v].first << " rule used: "<<rhs[0]<< endl;
            }
        }
    }
  }
}

void binary_production(int N, int R, std::vector<std::string> input, vector<pair<string, vector<vector<string>>>> rules, unordered_set<string> &terminators, bool*** P, map<string, int> m, queue<triplet<int>>***& back){
  for (int l = 1; l < N; l++) {          // Length of span (0-indexed)
      for (int s = 0; s < N-l; s++) {    // Start of span (0-indexed)


              for (int a = 0; a < R; a++) { //which rule is used
                  for (const auto& rhs : rules[a].second) {
                      if (rhs.size() == 2) {
                        for (int p = 0; p < l; p++) {  // Partition point (0-indexed)
                        //cout<<"length: "<<l<<" start:"<<s<<" partition at "<<p<<",  checking"<<" "<<p<<" "<<s<<" "<<l-p-1<<" "<<s+p+1<<endl;
                          // Verify the rule components exist in our map
                          auto firstIt = m.find(rhs[0]);
                          auto secondIt = m.find(rhs[1]);
                          if (firstIt == m.end() || secondIt == m.end()) {
                              continue;  // Skip if symbols not found
                          }
                          
                          int b = firstIt->second;  // Non-terminal index for first part 
                          int c = secondIt->second;  // Non-terminal index for second part
                          
                          // Bounds check to prevent access errors
                          if (p >= N || s >= N || b >= R || 
                              (l-p-1) >= N || (s+p+1) >= N || c >= R) {
                              continue;
                          }
                          
                          //cout<<"left: "<<P[p][s][b]<<"right: "<<  P[l-p-1][s+p+1][c]<<endl;
                          //cout<<"p s b: "<<p<<" "<<s<<" "<<b<<"   "<<"l-p-1 s+p+1 c"<<l-p-1<<" "<<s+p+1<<" "<<c<<endl;

                          if (P[p][s][b] && P[l-p-1][s+p+1][c]) {
                              P[l][s][a] = true;
                              triplet<int> path = {p, b, c, rhs, a};
                              back[l][s][a].push(path);
                              cout << "Binary match at [" << l << "," << s << "," << a << "] " 
                                  << "using partition " << p << " with " 
                                  << rules[b].first << " and " << rules[c].first << endl;
                              cout<<"aka: start at "<< s<<" with length "<<l<<" parition at: "<<p<<endl;
                              for (int start = s; start<=l+s;start++){
                                cout<<" "<<input[start];
                                if (start==p+s) cout<<" ||| ";
                              }
                              cout<<endl;
                            }
                          //  else {
                          //     for (int start = s; start<=l+s;start++){
                          //       cout<<" "<<input[start];
                          //     }
                          //     cout<<endl;
                          // }
                      }
                  }
              }
          }
      }
  }
}


int cyk(std::vector<std::string> input, vector<pair<string, vector<vector<string>>>> rules, unordered_set<string> terminators, queue<triplet<int>>***& back){
  int N = input.size();
  int R = rules.size();

  // initalize memories
  bool*** P = (bool***)malloc(N * sizeof(bool**));
  for (int i = 0; i < N; i++) {
      P[i] = (bool**)malloc(N * sizeof(bool*));
      for (int j = 0; j < N; j++) {
          P[i][j] = (bool*)malloc(R * sizeof(bool));
          // Initialize to false (equivalent to = {})
          memset(P[i][j], false, R * sizeof(bool));
      }
  }

  // use queue for rules appliable in each slice (slices are 3 dimension)
  back = (queue<triplet<int>>***)malloc(N * sizeof(queue<triplet<int>>**));
  for (int i = 0; i < N; i++) {
      back[i] = (queue<triplet<int>>**)malloc(N * sizeof(queue<triplet<int>>*));
      for (int j = 0; j < N; j++) {
          back[i][j] = (queue<triplet<int>>*)malloc(R * sizeof(queue<triplet<int>>));
          // Initialize each queue (call constructor)
          for (int k = 0; k < R; k++) {
              new(&back[i][j][k]) queue<triplet<int>>();
          }
      }
  }


  map<string, int> m;
  for (int i=0;i<R;i++){
    if (m.find(rules[i].first)==m.end()){
      m.insert({rules[i].first, i});
    }
  }

  cout<<"populate terminors"<<endl;
  populateTerminators(terminators, rules);

  // Terminal productions
  terminal_production(N, R, input, rules, terminators, P);

  // Binary productions
  binary_production(N,R, input, rules, terminators, P, m, back);


  cout<<"answer: "<<P[N-1][0][0]<<endl;
  return P[N-1][0][0] ? 0 : -1;
}