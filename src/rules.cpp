#include <vector>
#include <string>
using namespace std;

vector<pair<string, vector<vector<string>>>> rules = {

      {"S_list_end", {{"S_list", ";"}}},
      {"S_list", {{"S;", "S_list"}, 
            {"S;","E"}, {"while(E)", "{S_list;}"},{"if(E)", "{S_list;}"}, {"temp_statement"}, {"var","identifier"},
            {"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"},{"*","E"}, {"value"}, {"identifier"},
            {"function:show"},
            {"identifier","=E"}, {"identifier", "+T"},{"identifier", "-T"}, {"identifier", "*F"},{"identifier", "/F"},
            {"E", "=E"},
      }},

      {"S;", {{"S", ";"}}},
      {"S", {{"S;","E"}, {"while(E)", "{S_list;}"}, {"if(E)", "{S_list;}"}, {"temp_statement"}, {"var","identifier"},
            {"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"},{"*","E"}, {"value"}, {"identifier"}, {"malloc", "E"}, {"free", "E"}, {"malloc", "E"}, {"free", "E"},
            {"function:show"},
            {"identifier","=E"}, {"identifier", "+T"},{"identifier", "-T"}, {"identifier", "*F"},{"identifier", "/F"},
            {"E", "=E"},
      }},

      {"while(E)", {{"while", "(E)"}}},

      {"if(E)", {{"if", "(E)"}}},
      {"=E", {{"=","E"}}},
      {"E;", {{"E", ";"}}},

      
      {"{S}", {{"{","S}"}}},
      {"{S;}", {{"{","S;}"}}},
      {"{S_list;}", {{"{","S_list;}"}}},
      
      {"S}", {{"S", "}"}}},
      {"S;}", {{"S;", "}"}}},
      {"S_list;}", {{"S_list_end", "}"}}},

      {"S;", {{"S", ";"}}},


      {"E", {{"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"},{"*","E"}, {"value"}, {"identifier"}, {"malloc", "E"}, {"free", "E"},}},
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
      {"while",{{"while"}}},

      {"(", {{"("}}},
      {")", {{")"}}},
      {"{", {{"{"}}},
      {"}", {{"}"}}},
      {";", {{";"}}},
      {"=", {{"="}}},

      {"var", {{"var"}}},
      {"identifier", {{"identifier"}}},

      {"malloc", {{"malloc"}}},
      {"free", {{"free"}}},

    };