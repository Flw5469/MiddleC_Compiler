// #include <string>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

// typedef struct treeNode {
//   pair<string, vector<string>> currentRule;
// } treeNode;

// int main(){
//   string a = "abc";
//   string b = "deaf";
//   string c = "afw";

//   treeNode* tree = new treeNode;
//   tree->currentRule = {a,vector<string>{b,c}};
//   cout<<"LOOKS GOOD TO ME\n";

//   return 0;
// }


pair<string, vector<vector<string>>> rules[] = {

      {"S_list_end", {{"S_list", ";"}}},
      {"S_list", {{"S;", "S_list"}, 
            {"S;","E"}, {"if(E)", "{S;}"}, {"temp_statement"}, {"var","identifier"},
            {"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"}, {"value"}, {"identifier"},
            {"identifier","=E"}, {"identifier", "+T"},{"identifier", "-T"}, {"identifier", "*F"},{"identifier", "/F"},
      }},

      {"S;", {{"S", ";"}}},
      {"S", {{"S;","E"}, {"if(E)", "{S;}"}, {"temp_statement"}, {"var","identifier"},
            {"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"}, {"value"}, {"identifier"},
            {"identifier","=E"}, {"identifier", "+T"},{"identifier", "-T"}, {"identifier", "*F"},{"identifier", "/F"},
      }},

      {"if(E)", {{"if", "(E)"}}},
      {"=E", {{"=","E"}}},
      {"E;", {{"E", ";"}}},

      
      {"{S}", {{"{","S}"}}},
      {"{S;}", {{"{","S;}"}}},
      
      {"S}", {{"S", "}"}}},
      {"S;}", {{"S;", "}"}}},
      {"S;", {{"S", ";"}}},


      {"E", {{"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"}, {"value"}, {"identifier"}}},
      {"+T", {{"+", "T"}}},
      {"-T", {{"-", "T"}}},
      {"(E)", {{"(","E)"}}},
      {"E)", {{"E",")"}}},


      {"*F", {{"*", "F"}}},
      {"/F", {{"/", "F"}}},

      {"T", {{"T","*F"},{"T","/F"}, {"(", "E)"} , {"value"}, {"identifier"}}},

      {"F", {{"(", "E)"}, {"value"}, {"identifier"}}},      

      {"*", {{"*"}}},
      {"/", {{"/"}}},

      {"+", {{"+"}}},
      {"-", {{"-"}}},

      {"if",{{"if"}}},

      {"(", {{"("}}},
      {")", {{")"}}},
      {"{", {{"{"}}},
      {"}", {{"}"}}},
      {";", {{";"}}},
      {"=", {{"="}}},

      {"var", {{"var"}}},
      {"identifier", {{"identifier"}}},
    };
 std::string input[] = {"var", "peter", ";" ,"peter", "=", "3", ";", "peter", "=", "peter", "+", "1000" , ";"
           , "var", "peter2", ";" ,"peter2", "=", "3", ";", "peter2", "=", "peter2", "+", "1997" , ";", "peter2", "hi", "=", "peter2", "+", "1997" , ";", "peter2", "hi"};

int main(){
  cout<<input[0]<<endl;
}
