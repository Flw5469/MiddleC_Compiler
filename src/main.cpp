//TODO after: type check
//TODO after: class and functions

#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <iterator>
#include <fstream>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include "helper_functions.cpp"
#include "cyk.cpp"
#include "tree_functions.cpp"
#include "rules.cpp"
#include "assembly.cpp"
#include "parsing.cpp"

// directly implemeted from https://en.wikipedia.org/wiki/CYK_algorithm
// make sure to convert grammar into chomsky norm form

using namespace std;
int if_used_count = 0;
vector<string> input;
unordered_set<string> terminators;


int main(){
  
  cout<<"enter main\n";

  string raw_input = loadFileAsString("utilities/input.txt");
  input = parseTokensManual(raw_input);

  vector<string> assembly_code;
  vector<map<string, string>> variables;
  variables.push_back(map<string, string>());

  queue<triplet<int>>*** back;
  if (cyk(input,rules,terminators,back) == -1) {
    cout<<"NO"<<endl;
    return 1;
  } else {
    cout<<"YES"<<endl;
  }

  treeNode *tree = convert_into_tree(input, back, rules, terminators);
  cout<<"start generating assembly: \n";
  DFS(tree, assembly_code, variables, if_used_count, terminators);

  printTreeValues(tree);
  outputAssembly(assembly_code);
  displayAssembly(assembly_code);

  cout<<"Terminated Successfully!";
  return 0;


}