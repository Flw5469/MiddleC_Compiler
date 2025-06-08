#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include <string>

using namespace std;

int main(){
  string a = "101*3-25/5+17*21321"; //315
  vector<string> store;

  int start=0, end=0;
  while (end<a.size()){
    cout<<"checking "<<end<<endl;
    if (a[end]>='0' && a[end]<='9') {
    }
    else {
      cout<<start<<" "<<end<<endl;
      cout<<"entering "<<a.substr(start,(end-start))<<endl;
      store.push_back(a.substr(start,(end-start)));
      start = end;

      store.push_back(a.substr(end,1));
      start++; 
    }
    
    end++;
  }

  store.push_back(a.substr(start, a.size()-start));


  for (int i=0;i<store.size();i++){
    cout<<store[i]<<endl;
  }


  start = 0; end = 0;
  for (int i=0;i<store.size();i++){
    if (store[i] == "*"){
      store[start] = std::to_string(std::stoi(store[start])*std::stoi(store[i+1]));
      store[i] = "";
      store[i+1] = "";
    } else if (store[i] == "/"){
      store[start] = std::to_string(std::stoi(store[start])/std::stoi(store[i+1]));
      store[i] = "";
      store[i+1] = "";
    } else if (store[i]=="+" || store[i] == "-"){
      start = i+1;
    }
  }
  cout<<"-------------------\n";
  for (int i=0;i<store.size();i++){
    cout<<store[i]<<endl;
  }
  start = 0;
  for (int i=0;i<store.size();i++){
    if (store[i] == "+"){
      store[start] = std::to_string(std::stoi(store[start])+std::stoi(store[i+1]));
      store[i] = "";
      store[i+1] = "";
    } else if (store[i] == "-"){
      store[start] = std::to_string(std::stoi(store[start])-std::stoi(store[i+1]));
      store[i] = "";
      store[i+1] = "";
    }
  }
  cout<<"-------------------\n";
  for (int i=0;i<store.size();i++){
    cout<<store[i]<<endl;
  }

}