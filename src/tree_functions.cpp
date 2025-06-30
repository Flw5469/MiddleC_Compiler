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
#include <fstream>
#include <sstream>
#include "helper_functions.cpp"
#include "structures.cpp"


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




// left first DFS
// In-order DFS traversal (left, root, right)
// seperate the assembly generation as pre left DFS, after left DFS, after right DFS.
void DFS(treeNode* tree, vector<string> &assembly_code, vector<map<string, string>>& variables, int &if_used_count, unordered_set<string> terminators) {

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
    DFS(tree->left, assembly_code, variables, if_used_count, terminators);
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
    DFS(tree->right, assembly_code, variables, if_used_count , terminators);
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


treeNode* recursively_deal(vector<string> input, queue<triplet<int>> ***& back, vector<pair<string, vector<vector<string>>>> rules, int start, int end, unordered_set<string> terminators) {

      cout<<"entered start: "<<start<<" end: "<<end<<endl;

      int i = end-start;
      int j = start;
      cout<<i<<j<<endl;


      treeNode* tree = new treeNode;//(treeNode*)malloc(sizeof(treeNode)); //***NOTE: new instead of malloc so stuff get initalized */
      tree->currentValue = "";

      if (i==0) {
      for (int k=0;k<rules.size();k++){
        for (const auto& rhs : rules[k].second) {
            if (rhs.size() == 1 && handleRules(rhs[0],input[start], terminators)) {
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
          tree->left = recursively_deal(input, back, rules, start, start+seperator, terminators);
          tree->right = recursively_deal(input, back, rules, start+seperator+1, end, terminators);
          return tree;
        }
      }

      return tree;

};


treeNode* convert_into_tree(vector<string> input, queue<triplet<int>> ***&back, vector<pair<string, vector<vector<string>>>> rules, unordered_set<string> terminators) {
  return recursively_deal(input, back, rules, 0, input.size()-1, terminators);
}


