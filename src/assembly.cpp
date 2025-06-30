#include <vector>
#include <string>
#include <iostream>
#include "helper_functions.cpp"
#include <fstream>
#include <sstream>
#include <filesystem>  // Add this include

using namespace std;

void displayAssembly(vector<string> assembly_code){
  cout<<"\nassembly code:"<<endl;
  for (auto ele:assembly_code)
    cout<<ele<<"\n";
  cout<<endl;
}

void outputAssembly(vector<string> assembly_code){
    string asm_prefix = loadFileAsString("utilities/asm_prefix.txt");
    string asm_postfix = loadFileAsString("utilities/asm_postfix.txt");

    string asm_content = asm_prefix;
    for (auto ele : assembly_code) {
        asm_content += "    " + ele + "\n";
    }
    asm_content+=asm_postfix;

    std::filesystem::create_directories("build/output");
    ofstream file("build/output/output.asm");
    if (file.is_open()) {
        file << asm_content;
        file.close();
        cout << "\nAssembly code written to output.asm" << endl;
    } else {
        cout << "\nError: Could not create output.asm file" << endl;
    }
}