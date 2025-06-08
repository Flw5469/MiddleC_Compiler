#include <iostream>
#include <set>
#include <stack>
#include <vector>

using namespace std;

int main(){
  string a = "101*3-25/5+17*21321"; //315

  int i=0;
  vector<string> store;
  string last = "";
  while (i<a.size()){
    if ((a[i]>='0') && (a[i]<='9') && last!="~"){
      last+=a[i];
    }
    if ((a[i]>='0') && (a[i]<='9') && last=="~"){
      last=a[i];
    }
    if (!((a[i]>='0') && (a[i]<='9'))) {
      store.push_back(last);
      last = "~";
      store.push_back(a.substr(i,1));   
    }
    i++;
  }
  store.push_back(last);

  for (auto ele:store) {
    cout<<ele<<" ";
  }

  vector<string> intermediate;
  for (int i=0;i<store.size();i++){
    if ((store[i][0]>='0') && (store[i][0]<='9') && ((intermediate.back() == "*") || (intermediate.back() == "/"))){
      intermediate.push_back(intermediate[intermediate.size()-2]);
    }
  }

}