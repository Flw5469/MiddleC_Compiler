#include "parsing.cpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(){
  string user_input = "var peter ; peter = malloc 100 ; ( peter ) = 20 ; if ( ( * peter ) / 5 + 1 ) { function:show ; } ;";
  vector<string> result = parseTokensManual_v2(user_input);
  for (auto ele:result) {
    cout<<ele<<endl;
  }
  cout<<endl;
}