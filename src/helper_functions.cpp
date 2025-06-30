#ifndef HELPER_CPP
#define HELPER_CPP

#include <string>
#include <unordered_set>
using namespace std;

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

void populateTerminators(unordered_set<string> &terminators, vector<pair<string, vector<vector<string>>>> rules) {
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
bool isIdentifier(const string& rule, const string& str, unordered_set<string> terminators) {
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

bool handleRules(const string& rule, const string& str, unordered_set<string> terminators){
   if ((rule == str)&&(str!="value")&&(str!="identifier")) {
    return true;
   } else {
    return (isInteger(rule,str) || isIdentifier(rule, str, terminators));
   }
}


std::string loadFileAsString(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


#endif