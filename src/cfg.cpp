//TODO after: array and type check
//TODO after: class and functions
//TODO after: sleep (irl)

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
#include <cstring>  // Add this at the top with your other includes

// directly implemeted from https://en.wikipedia.org/wiki/CYK_algorithm
// make sure to convert grammar into chomsky norm form

using namespace std;

int if_used_count = 0;

template <typename T>

struct triplet {
    T first;
    T second;
    T third;
    vector<string> fourth;
    T fifth;
};

typedef struct treeNode {
  treeNode* left;
  treeNode* right;
  pair<string, vector<string>> currentRule;
  string currentValue;
} treeNode;

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

//  string input[] = { "if", "(", "31234","-","31234", ")", "{" ,
//     "if" , "(", "50","-","50", ")", "{" ,"6969820", ";","}", ";","74", ";",
//     "}", ";"}; // no precedence = 6, precedence = 9
// string input[] = {"3","+", "5",";"}; // no precedence = 6, precedence = 9
// string input[] = { "if", "(", "31234","-","3124", ")", "{" ,"696969",";","}", ";", "1230007", ";"}; // no precedence = 6, precedence = 9
 //string input[] = {"var", "peter", ";" ,"peter", "=", "3", ";", "peter", "=", "peter", "+", "1000" , ";"
   //         , "var", "peter2", ";", "peter2", "=", "peter2", "+", "1997" , ";", "peter2", "=" , "peter" ,"+","peter2", ";" ,"peter2","+","peter",";"};
//string input[] = {"var", "peter", ";", "peter","=","10",";","while", "(", "peter", ")", "{", "peter", "=", "peter", "-", "1", ";", "peter",";","function:show",";" ,"}", ";"};
//string input[] = {"malloc", "100", ";" };
vector<string> input;
unordered_set<string> terminators;
//string raw_input = "var peter ; peter = malloc 100 ; ( peter ) = 20 ; * peter ; while ( * peter ) { ( peter ) = ( * peter ) - 1 ; if ( * peter ) { function:show ; } ; } ; "; 
//string raw_input = "var peter ; peter = malloc 100 ; ( peter ) = 20 ; if ( ( * peter ) / 5 ) { function:show ; } ; "; 
string raw_input = "var peter ; peter = 10 ; peter ;";

std::vector<std::string> parseTokensManual(const std::string& input) {
    std::vector<std::string> tokens;
    std::string current_token;
    
    for (char c : input) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            if (!current_token.empty()) {
                tokens.push_back(current_token);
                current_token.clear();
            }
        } else {
            current_token += c;
        }
    }
    
    // Add the last token if it exists
    if (!current_token.empty()) {
        tokens.push_back(current_token);
    }
    
    return tokens;
}


void printTreeValues(treeNode* root, int depth = 0) {
  if (root == nullptr) {
    return;
  }

  // Print indentation based on depth
  std::string indent(depth * 2, ' ');
  
  // Print current node's value
  std::cout << indent << "Node Value: \"" << root->currentValue << "\"" << std::endl;
  
  // Print rule if available
  if (!root->currentRule.first.empty()) {
    std::cout << indent << "Rule: " << root->currentRule.first << " -> ";
    for (size_t i = 0; i < root->currentRule.second.size(); i++) {
      if (i > 0) std::cout << " ";
      std::cout << root->currentRule.second[i];
    }
    std::cout << std::endl;
  }
  
  // Recursively print left and right subtrees
  if (root->left || root->right) {
    if (root->left) {
      std::cout << indent << "Left child of \"" << root->currentValue << "\":" << std::endl;
      printTreeValues(root->left, depth + 1);
    } else {
      std::cout << indent << "Left child: nullptr" << std::endl;
    }
    
    if (root->right) {
      std::cout << indent << "Right child of \"" << root->currentValue << "\":" << std::endl;
      printTreeValues(root->right, depth + 1);
    } else {
      std::cout << indent << "Right child: nullptr" << std::endl;
    }
  }
}

bool checkPrefix(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) {
        return false;
    }
    return str.substr(0, prefix.length()) == prefix;
}
bool isNumber(string str) {
    if (str.empty()) {
        return false;
    }
    
    int start = 0;
    // Handle optional negative sign
    if (str[0] == '-') {
        if (str.length() == 1) {
            return false; // Just a minus sign
        }
        start = 1;
    }
    
    // Check if all remaining characters are digits
    for (int i = start; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    
    return true;
}

void outputAssembly(vector<string> assembly_code){
  string asm_prefix = R"(; NASM syntax
bits 32
section .data
    msg_format db "EAP register's value = %d", 0    ; Format string with placeholder
    message db 32 dup(0)              ; Buffer for our formatted message
    title db "Assembly Calculation", 0

section .text
global start
extern _MessageBoxA@16               ; stdcall - decorated
extern wsprintfA                     ; cdecl - no decoration
extern _ExitProcess@4                ; stdcall - decorated
extern _GetProcessHeap@0
extern _HeapAlloc@12
extern _HeapFree@12

; Function: show
; Parameters: eax = number to display
; Uses cdecl calling convention
show_function:
    push ebp            ; Save caller's frame pointer
    mov ebp, esp        ; Set up our frame pointer
    
    ; Format the result string using wsprintfA (cdecl calling convention)
    push eax            ; Push the result (13)
    push msg_format     ; Push format string
    push message        ; Push destination buffer
    call wsprintfA      ; Call wsprintfA
    add esp, 12         ; Clean up stack (caller cleans for cdecl)
    
    ; Show message box with the result
    push 0              ; MB_OK
    push title          ; Title
    push message        ; Our formatted message
    push 0              ; hWnd
    call _MessageBoxA@16
    
    mov esp, ebp        ; Restore stack pointer
    pop ebp             ; Restore caller's frame pointer
    ret                 ; Return to caller

; Allocate heap memory
; Input: element in stack
; Output: eax = pointer to allocated memory (or 0 if failed)
malloc:
    push ebp
    mov ebp, esp
    push ebx            ; Save ebx
    
    mov ebx, [ebp+8]    ; Get size parameter from stack
    
    call _GetProcessHeap@0
    push ebx            ; Push size
    push 0              ; Push flags (HEAP_NO_SERIALIZE could be 0x1)
    push eax            ; Push heap handle
    call _HeapAlloc@12  ; Allocate memory
    
    pop ebx             ; Restore ebx
    pop ebp
    ret 4               ; Clean up 4 bytes (size parameter)

; Free heap memory  
; Input: element in stack as the heap memory location
free:
    push ebp
    mov ebp, esp
    push ebx            ; Save ebx
    
    mov ebx, [ebp+8]    ; Get pointer parameter from stack
    
    call _GetProcessHeap@0  ; Get heap handle in eax
    push ebx            ; Push pointer to free
    push 0              ; Push flags  
    push eax            ; Push heap handle
    call _HeapFree@12   ; Free memory
    
    pop ebx             ; Restore ebx
    pop ebp
    ret 4               ; Clean up pointer parameter

start:
    ; Perform the calculation
    mov ebp, esp

)";
  string asm_postfix = R"(


    call show_function


    ; Exit program
    push 0              ; Exit code
    call _ExitProcess@4)";

    string asm_content = asm_prefix;
    for (auto ele : assembly_code) {
        asm_content += "    " + ele + "\n";
    }
    asm_content+=asm_postfix;


    ofstream file("build/output/output.asm");
    if (file.is_open()) {
        file << asm_content;
        file.close();
        cout << "\nAssembly code written to output.asm" << endl;
    } else {
        cout << "\nError: Could not create output.asm file" << endl;
    }
}

void populateTerminators() {
    for (const auto& rule : rules) {
        const vector<vector<string>>& productions = rule.second;
        
        for (const auto& production : productions) {
            // If production has only one element, it's a terminal
            if (production.size() == 1) {
                terminators.insert(production[0]);
            }
        }
    }
}

bool isInteger(const string& rule, const string& str) {
    if (rule!="value") return false;

    if (str.empty()) return false;
    
    // Check if all characters are digits
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    
    // Check if it doesn't start with zero (unless it's just "0")
    if (str.length() > 1 && str[0] == '0') {
        return false;
    }
    
    return true;
}

// Function to check if string is a valid identifier
bool isIdentifier(const string& rule, const string& str) {
    if (rule!="identifier") return false;
    if (str.empty()) return false;
    
    // Check if it starts with a number
    if (isdigit(str[0])) return false;
    
    // Check if it's in the terminators set
    if (terminators.find(str) != terminators.end()) return false;
    
    // Check if all characters are valid for identifier (letters, digits, underscore)
    for (char c : str) {
        if (!isalnum(c) && c != '_') return false;
    }
    
    return true;
}

bool handleRules(const string& rule, const string& str){
   if ((rule == str)&&(str!="value")&&(str!="identifier")) {
    return true;
   } else {
    return (isInteger(rule,str) || isIdentifier(rule, str));
   }
}

// left first DFS
// In-order DFS traversal (left, root, right)
// seperate the assembly generation as pre left DFS, after left DFS, after right DFS.
void DFS(treeNode* tree, vector<string> &assembly_code, vector<map<string, string>>& variables) {

    int current_level_use_count; // needed so child wont affect the if_use_count (eg in nested if, the child increment the counter, parent wrongly use the incremented.)

    if (tree == nullptr) {
        return;
    }

    // add the high level statement if this is a statement or this is the last component of a S_list (unit production)
    if ( ( (tree->currentRule.first == "S_list") && (tree->currentRule.second[0] != "S;") )) {
      // this condition check is a work around since I messed up the Statement syntax, cannot seperate each  statement
      if ((assembly_code.size() ==0 ) || (assembly_code[assembly_code.size()-1][0]!=';')) {
        assembly_code.push_back("");
        assembly_code.push_back("; " + tree->currentValue);
      }}

    if (tree->currentRule.second.size()==1) {
      cout << tree->currentValue << " ";  // Print the current value
      
        // TODO: add a actual type check (whether it is numerical?) here
        if (isNumber(tree->currentValue)) {
          assembly_code.push_back("mov eax, " + tree->currentValue);
        }
      
        // load identifier into register. side effect: fresh initalize will also get loaded, but dont matter actually
        if (tree->currentRule.first == "identifier") {
          cout<<"looking for identifier "<<tree->currentValue<<endl;
          cout<<"variable length: "<< variables.size()<<endl;
          int count = 0;
          auto itr = variables.end();
          bool found = false;
          map<string, string>::iterator result;

          while (itr != variables.begin()) {
            itr--;
            cout<<"traversing!"<<endl;  
            result = itr->find(tree->currentValue);
            cout<<"result got"<<endl;
            if (result != itr->end()) {
              found = true;
            }
            count++;
          }

          if (found) {
              assembly_code.push_back("mov eax, ebp ; access the stack frame");        // Move current stack frame pointer (ebp) to EAX
              for (int i = 1; i < count; i++) {
                  assembly_code.push_back("mov eax, [eax] ; trace back the stack frame");      // Load value at address in EAX into EAX
              }
              assembly_code.push_back("mov eax, [eax" + result->second + "]" + "; load the variable into eax");      // load the value at eax+offset into eax          
          }
        }


        if (tree->currentRule.second[0] == "function:show") {
          assembly_code.push_back("call show_function");
        }
      return;
    }


    // enter closure, add new variable set, set pointer to previous pointer.
    if ((tree->left->currentRule.first == "{")) {

      variables.push_back(map<string, string>());
      assembly_code.push_back("push ebp          ; Save caller's base pointer");
      assembly_code.push_back("mov ebp, esp      ; Set up new base pointer");

    }

    // initalize variable: by putting the value of right tree (identifer) into the map, noted that differnt statement within a closure shares the same variables set.
    if (tree->currentRule.second[0] == "var") {
      string variable_name = tree->right->currentValue;
      int offset = (variables[variables.size()-1].size() + 1)*4;
      cout<<"offset is : "<<offset<<"current value is : "<< variable_name<<endl;
      variables[variables.size()-1].insert({tree->right->currentValue, to_string(-offset)});
      assembly_code.push_back("push 0");
    }

    if ((tree->currentRule.second[0]=="while(E)")) {
      if_used_count++;
      current_level_use_count = if_used_count;
      assembly_code.push_back("while_start_" + to_string(current_level_use_count) + ":");
    }
          



    //handle left tree
    ///////////////////////////////////
    DFS(tree->left, assembly_code, variables);
    /////////////////////////////////


    cout << tree->currentValue << " ";  // Print the current value

    if ((tree->currentRule.second[1]=="+T") || ((tree->currentRule.second[1]=="-T") || (tree->currentRule.second[1]=="*F") || (tree->currentRule.second[1]=="/F"))) {
      assembly_code.push_back("push eax");
    }

    if ((tree->currentRule.second[0] == "E") && (tree->currentRule.second[1] == "=E")) {
      assembly_code.push_back("push eax");
    }

    // 
    if ((tree->currentRule.second[0]=="if(E)")) {
      if_used_count++;
      current_level_use_count = if_used_count;
      assembly_code.push_back("cmp eax, 0             ; compare with 0");
      assembly_code.push_back("je if_end_" + to_string(current_level_use_count)); // jump if eax != 0
    }

    if ((tree->currentRule.second[0]=="while(E)")) {
      //current level use count incremented before left tree. (for the start label)
      assembly_code.push_back("cmp eax, 0             ; compare with 0");
      assembly_code.push_back("je while_end_" + to_string(current_level_use_count)); // jump if eax != 0
    }

    // handle Right tree
    ////////////////////////////
    DFS(tree->right, assembly_code, variables);
    ///////////////////////////

    // assign value to variable
    // assume eap contains the result already. look up identifier in vector of maps, use the #of map to determine how many closure pointers to look back.
    // then store it directly into the address hardcoded.
    if ((tree->currentRule.second[0] == "identifier") && (tree->currentRule.second[1] == "=E")) {
      int count = 0;
      auto itr = variables.end();
      bool found = false;
      map<string, string>::iterator result;

      // for (auto ele:variables){
      //   for (auto ele2:ele){
      //     cout<<ele2.first<<" "<<ele2.second<<endl;
      //   }
      // }

      while (itr != variables.begin()) {
        itr--;
        cout<<"traversing!"<<endl;  
        result = itr->find(tree->left->currentValue);
        if (result != itr->end()) {
          found = true;
        }
        count++;
      }

      if (found) {
          assembly_code.push_back("mov ebx, eax");
          assembly_code.push_back("mov eax, ebp; access the stack frame");        // Move current stack frame pointer (ebp) to EAX
          for (int i = 1; i < count; i++) {
              assembly_code.push_back("mov eax, [eax]; trace back the stack frame");      // Load value at address in EAX into EAX
          }  
          assembly_code.push_back("mov dword [eax" + result->second + "], ebx; store ebx into memory"); // store ebx into memory at (eax + offset)
        }
    }

    if ((tree->currentRule.second[0] == "E") && (tree->currentRule.second[1] == "=E")) {
        assembly_code.push_back("mov ebx, eax");
        assembly_code.push_back("pop eax");
        
        assembly_code.push_back("mov dword [eax], ebx"); // store ebx (the right result) into the left address
    }

    // enter closure, add new variable set, set pointer to previous pointer.
    if ((tree->left->currentRule.first == "{")) {
      variables.pop_back();
      assembly_code.push_back("mov esp, ebp      ; Restore stack pointer");
      assembly_code.push_back("pop ebp           ; Restore caller's base pointer");
    }

    // handle if by adding a ending tag here
    if ((tree->currentRule.second[0]=="if(E)")) {
      assembly_code.push_back("if_end_" + to_string(current_level_use_count) + ":");
    }
    
    // handle if by adding a ending tag here, but before entering the ending tag, it get jumped to the start condition check
    if ((tree->currentRule.second[0]=="while(E)")) {
      assembly_code.push_back("jmp while_start_" + to_string(current_level_use_count));
      assembly_code.push_back("while_end_" + to_string(current_level_use_count) + ":");
    }

    // move eax to ebx, so pop back original eax (left hand side result), do calculate and put in eax.
    // should also compatible for handling variable (since it is an expression)
    if ((tree->currentRule.second[1]=="+T") || (tree->currentRule.second[1]=="-T") || (tree->currentRule.second[1]=="*F") || (tree->currentRule.second[1]=="/F")) {
        assembly_code.push_back("mov ebx, eax");
        assembly_code.push_back("pop eax");

        if (tree->currentRule.second[1] == "+T"){
            assembly_code.push_back("add eax, ebx");  // eax = eax + ebx
        }
        if (tree->currentRule.second[1] == "-T"){
            assembly_code.push_back("sub eax, ebx");  // eax = eax - ebx
        }
        if (tree->currentRule.second[1] == "*F"){
            assembly_code.push_back("imul eax, ebx"); // eax = eax * ebx
        }
        if (tree->currentRule.second[1] == "/F"){
            assembly_code.push_back("cdq");           // Sign extend eax to edx:eax
            assembly_code.push_back("idiv ebx");      // eax = eax / ebx
        }
    }
    
    if (tree->currentRule.second[0] == "malloc") {
          assembly_code.push_back("push eax");
          assembly_code.push_back("call malloc");
    }
    if (tree->currentRule.second[0] == "free") {
          assembly_code.push_back("push eax");
          assembly_code.push_back("call free");
    }
    
    // TODO: bug: when storing value into heap, this wrongly get triggered and load the value instead of address into counter.
    // get things clear:
    // peter = 3 => variable assign to 3
    // *peter = 3 => "heap memory from peter" assign to 3
    // expression = 3 => "memory" assign to 3
    // need to distinglish expresion vs identifier QAQ
    if ((tree->currentRule.second[0] == "*") && (tree->currentRule.second[1] == "E")) {
      assembly_code.push_back("mov eax, [eax]      ; dereference the address");
    }

    return;

    // TODO: update intermediate value (precompute, ignore now)
}


treeNode* recursively_deal(vector<string> input, queue<triplet<int>> ***back, vector<pair<string, vector<vector<string>>>> rules, int start, int end) {

      cout<<"entered start: "<<start<<" end: "<<end<<endl;

      int i = end-start;
      int j = start;
      cout<<i<<j<<endl;


      treeNode* tree = new treeNode;//(treeNode*)malloc(sizeof(treeNode)); //***NOTE: new instead of malloc so stuff get initalized */
      tree->currentValue = "";

      if (i==0) {
      for (int k=0;k<rules.size();k++){
        for (const auto& rhs : rules[k].second) {
            if (rhs.size() == 1 && handleRules(rhs[0],input[start])) {
                cout << "Terminal match: " << input[start] << " as " << rules[k].first <<endl;
                tree->currentRule = {rules[k].first, rhs};
            }
        }
      }
      cout<<"exit loop"<<endl;
      tree->left = NULL;
      tree->right = NULL;
      tree->currentValue =  input[start];
      cout<<"leaf ready to return, current value is: "<<tree->currentValue<<endl;
      return tree;
      }

      for (int k=0;k<rules.size();k++){
        if (!back[i][j][k].empty()) {
          cout<<"\n\n start at "<<j<<" with length "<<i<<":"<< "\n";
        }

        int seperator = 0;
        struct triplet<int> path;
        if (!back[i][j][k].empty()){
          path = back[i][j][k].front();
          back[i][j][k].pop();

          cout<<"path: "<<path.first<<" "<<path.second<<" "<<path.third<<" rules: "<<rules[path.fifth].first<<" "<<path.fourth[0]<<" "<<path.fourth[1];
          seperator = path.first;

          for (int start = j; start<=i+j;start++){
            cout<<" "<<input[start];
            tree->currentValue += input[start];
            tree->currentValue += " ";
            if (start == j+seperator) cout<<" ||| ";
          }

          cout<<" , "<<" character constructed from this: "<<rules[k].first<<endl;
          //cout<<"confirm the follwing data are valud:"<<" rules: "<<rules[path.fifth].first<<" "<<path.fourth[0]<<" "<<path.fourth[1]<<endl;
          tree->currentRule = {rules[path.fifth].first, std::vector<string>{path.fourth[0], path.fourth[1]}};
          cout<<"ready to enter next depth, seperated at "<<start+seperator<<endl;
          tree->left = recursively_deal(input, back, rules, start, start+seperator);
          tree->right = recursively_deal(input, back, rules, start+seperator+1, end);
          return tree;
        }
      }

      return tree;

};


treeNode* convert_into_tree(vector<string> input, queue<triplet<int>> ***back, vector<pair<string, vector<vector<string>>>> rules) {
  treeNode* tree = (treeNode*)malloc(sizeof(treeNode));
  return recursively_deal(input, back, rules, 0, input.size()-1);
}


int main(){

  input = parseTokensManual(raw_input);
  int N = input.size();
  int R = rules.size();

  cout<<"enter main\n";
// Replace bool P[N][N][R] = {};
bool*** P = (bool***)malloc(N * sizeof(bool**));
for (int i = 0; i < N; i++) {
    P[i] = (bool**)malloc(N * sizeof(bool*));
    for (int j = 0; j < N; j++) {
        P[i][j] = (bool*)malloc(R * sizeof(bool));
        // Initialize to false (equivalent to = {})
        memset(P[i][j], false, R * sizeof(bool));
    }
}

// Replace queue<triplet<int>> back[N][N][R];
queue<triplet<int>>*** back = (queue<triplet<int>>***)malloc(N * sizeof(queue<triplet<int>>**));
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
  populateTerminators();

// Terminal productions
for (int s = 0; s < N; s++) {
    for (int v = 0; v < R; v++) {
        for (const auto& rhs : rules[v].second) {
            if (rhs.size() == 1 && s < N && handleRules(rhs[0],input[s])) {
                P[0][s][v] = true;  // Length 1, starting at position s
                cout << "Terminal match: " << input[s] << " as " << rules[v].first << " rule used: "<<rhs[0]<< endl;
            }
        }
    }
}

// Binary productions
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


  cout<<"answer: "<<P[N-1][0][0]<<endl;
  if (P[N-1][0][0]) cout<<"YES\n"; else {
    cout<<"NO\n"<<endl;
    return 1;
  }

  vector<string> assembly_code;
  vector<map<string, string>> variables;
  variables.push_back(map<string, string>());

  treeNode *tree = convert_into_tree(input, back, rules);
  cout<<"start generating assembly: \n";
  DFS(tree, assembly_code, variables);

  printTreeValues(tree);
  outputAssembly(assembly_code);


  cout<<"\nassembly code:"<<endl;
  for (auto ele:assembly_code)
    cout<<ele<<"\n";
  cout<<endl;
  cout<<"Terminated Successfully!";
  return 0;


}