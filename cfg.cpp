#include <iostream>
#include <set>
#include <queue>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <iterator>
#include <fstream>

// directly implemeted from https://en.wikipedia.org/wiki/CYK_algorithm
// make sure to convert grammar into chomsky norm form

using namespace std;



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

pair<string, vector<vector<string>>> rules[] = {
  
      // {"expression", {{"(","expression_with_)"},{"value", "operator_with_expression:+-"}, 
      //   {"value","operator_with_expression:*/"}, {"expression", "operator_with_expression:*/"},{"expression", "operator_with_expression:+-"},
      //   {"1"}, {"2"}, {"3"}, {"4"} , {"5"}, {"6"}, {"7"}, {"8"}
      // }},
      // {"expression_with_)", {{"expression",")"}}},
      // {"operator_with_expression:*/", {{"operator:*/","expression"}}},
      // {"operator_with_expression:+-", {{"operator:+-","expression"}}},    
      // {"operator:+-", { {"+"}, {"-"}}},
      // {"operator:*/", { {"*"}, {"/"}}},        
      // {"null", {{" "}}}

      {"E", {{"E", "+T"},{"E", "-T"}, {"T", "*F"},{"T", "/F"}, {"(", "E)"}, {"1"}, {"2"}, {"3"}, {"4"} , {"5"}, {"6"}, {"7"}, {"8"}}},
      {"+T", {{"+", "T"}}},
      {"-T", {{"-", "T"}}},
      {"E)", {{"E",")"}}},
      {"(", {{"("}}},
      {")", {{")"}}},

      {"*F", {{"*", "F"}}},
      {"/F", {{"/", "F"}}},

      {"T", {{"T","*F"},{"T","/F"}, {"(", "E)"} , {"1"}, {"2"}, {"3"}, {"4"} , {"5"}, {"6"}, {"7"}, {"8"}}},

      {"F", {{"(", "E)"},  {"1"}, {"2"}, {"3"}, {"4"} , {"5"}, {"6"}, {"7"}, {"8"}}},      

      {"*", {{"*"}}},
      {"/", {{"/"}}},

      {"+", {{"+"}}},
      {"-", {{"-"}}},

    };

string input[] = {"(","6","+","4",")","/","2","+","1","/","1", }; // no precedence = 6, precedence = 9

const int N = size(input);  // 9
const int R = size(rules);  // 11

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
    msg_format db "The final answer = %d", 0    ; Format string with placeholder
    message db 32 dup(0)              ; Buffer for our formatted message
    title db "Assembly Calculation", 0

section .text
global start
extern _MessageBoxA@16               ; stdcall - decorated
extern wsprintfA                     ; cdecl - no decoration
extern _ExitProcess@4                ; stdcall - decorated

start:
    ; Perform the calculation
)";
  string asm_postfix = R"(
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


    ; Exit program
    push 0              ; Exit code
    call _ExitProcess@4)";

    string asm_content = asm_prefix;
    for (auto ele : assembly_code) {
        asm_content += "    " + ele + "\n";
    }
    asm_content+=asm_postfix;


    ofstream file("output.asm");
    if (file.is_open()) {
        file << asm_content;
        file.close();
        cout << "\nAssembly code written to output.asm" << endl;
    } else {
        cout << "\nError: Could not create output.asm file" << endl;
    }
}

// left first DFS
// In-order DFS traversal (left, root, right)
// seperate the assembly generation as pre left DFS, after left DFS, after right DFS.
void DFS(treeNode* tree, vector<string> &assembly_code) {
    if (tree == nullptr) {
        return;
    }
    if (tree->currentRule.second.size()==1) {
      cout << tree->currentValue << " ";  // Print the current value
      
      // TODO: add a actual type check (whether it is numerical?) here
      if (isNumber(tree->currentValue)) {
        assembly_code.push_back("mov eax, " + tree->currentValue);
      } 
      return;
    }

    DFS(tree->left, assembly_code);
    cout << tree->currentValue << " ";  // Print the current value

    if ((tree->currentRule.second[1]=="+T") || ((tree->currentRule.second[1]=="-T") || (tree->currentRule.second[1]=="*F") || (tree->currentRule.second[1]=="/F"))) {
      assembly_code.push_back("push eax");
    }

    DFS(tree->right, assembly_code);

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
    
    // // first value (know by seeing it as first character of the current rule), push previous value and load value in register
    // if (tree->currentRule.second[0]=="value"){
    //   assembly_code.push_back(std::string("mov eax, ") + tree->left->currentValue);
    // }

    // // do the calculation with the register and current operator_with_expression
    // if (checkPrefix(tree->currentRule.second[1],"operator_with_expression")){
    //     if (tree->right->left->currentValue == "+") {
    //         assembly_code.push_back(std::string("add eax, ") + tree->right->right->currentValue);
    //     } 
    //     else if (tree->right->left->currentValue == "-") {
    //         assembly_code.push_back(std::string("sub eax, ") + tree->right->right->currentValue);
    //     } 
    //     else if (tree->right->left->currentValue == "*") {
    //         assembly_code.push_back(std::string("mov ebx, ") + tree->right->right->currentValue);
    //         assembly_code.push_back("imul eax, ebx");
    //     } 
    //     else if (tree->right->left->currentValue == "/") {
    //         assembly_code.push_back(std::string("mov ebx, ") + tree->right->right->currentValue);
    //         assembly_code.push_back("xor edx, edx");  // Clear EDX for division
    //         assembly_code.push_back("idiv ebx");      // Signed division
    //     }
    // }
    
    return;

    // TODO: update intermediate value (precompute, ignore now)
    // TODO: define variables

    // 
}


treeNode* recursively_deal(string input[N], queue<triplet<int>> back[N][N][R], pair<string, vector<vector<string>>> rules[R], int start, int end) {

      cout<<"entered start: "<<start<<" end: "<<end<<endl;

      int i = end-start;
      int j = start;
      cout<<i<<j<<endl;


      treeNode* tree = new treeNode;//(treeNode*)malloc(sizeof(treeNode)); //***NOTE: new instead of malloc so stuff get initalized */
      tree->currentValue = "";

      if (i==0) {
      for (int k=0;k<R;k++){
        for (const auto& rhs : rules[k].second) {
            if (rhs.size() == 1 && rhs[0] == input[start]) {
                cout << "Terminal match: " << input[start] << " as " << rules[k].first << endl;
                tree->currentRule = {rules[k].first, rhs};
            }
        }
      }
      cout<<"exit loop"<<endl;
      tree->left = NULL;
      tree->right = NULL;
      tree->currentValue =  input[start];
      cout<<"leaf ready to return"<<endl;
      return tree;
      }

      for (int k=0;k<R;k++){
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

};


treeNode* convert_into_tree(string input[N], queue<triplet<int>> back[N][N][R], pair<string, vector<vector<string>>> rules[R]) {
  treeNode* tree = (treeNode*)malloc(sizeof(treeNode));
  return recursively_deal(input, back, rules, 0, N-1);
}


int main(){
  bool P[N][N][R] = {};
  queue<triplet<int>> back[N][N][R];


  map<string, int> m;
  for (int i=0;i<R;i++){
    if (m.find(rules[i].first)==m.end()){
      m.insert({rules[i].first, i});
    }
  }


// Terminal productions
for (int s = 0; s < N; s++) {
    for (int v = 0; v < R; v++) {
        for (const auto& rhs : rules[v].second) {
            if (rhs.size() == 1 && s < N && rhs[0] == input[s]) {
                P[0][s][v] = true;  // Length 1, starting at position s
                cout << "Terminal match: " << input[s] << " as " << rules[v].first << endl;
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
                        // } else {
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
    return 0;
  }
  // for (int i=0;i<N;i++){
  //   for (int j=0;j<i;j++){
  //     for (int k=0;k<R;k++){

  //       if (!back[i][j][k].empty()) {
  //         cout<<"\n\n start at "<<j<<" with length "<<i<<":"<< "\n";
  //       }

  //       int seperator = 0;
  //       while (!back[i][j][k].empty()){
  //         struct triplet<int> path = back[i][j][k].top();
  //         cout<<"path: "<<path.first<<" "<<path.second<<" "<<path.third<<" rules: "<<rules[path.fifth].first<<" "<<path.fourth[0]<<" "<<path.fourth[1];
  //         seperator = path.first;
  //         back[i][j][k].pop();

  //         for (int start = j; start<=i+j;start++){
  //           cout<<" "<<input[start];
  //           if (start == j+seperator) cout<<" ||| ";
  //         }

  //         cout<<" , character inex: "<<k<<" character: "<<rules[k].first<<endl;
  //         cout<<endl;
  //       }

  //     }
  //   }
  // }

  vector<string> assembly_code;
  treeNode *tree = convert_into_tree(input, back, rules);
  DFS(tree, assembly_code);

  printTreeValues(tree);

  outputAssembly(assembly_code);


  cout<<"\nassembly code:"<<endl;
  for (auto ele:assembly_code)
    cout<<ele<<"\n";
  cout<<endl;
  cout<<"Terminated Successfully!";
  return 0;


}