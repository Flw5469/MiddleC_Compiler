#include <vector>
#include <string>
#include <cstring>  // Add this at the top with your other includes
using namespace std;

//  string input[] = { "if", "(", "31234","-","31234", ")", "{" ,
//     "if" , "(", "50","-","50", ")", "{" ,"6969820", ";","}", ";","74", ";",
//     "}", ";"}; // no precedence = 6, precedence = 9
// string input[] = {"3","+", "5",";"}; // no precedence = 6, precedence = 9
// string input[] = { "if", "(", "31234","-","3124", ")", "{" ,"696969",";","}", ";", "1230007", ";"}; // no precedence = 6, precedence = 9
 //string input[] = {"var", "peter", ";" ,"peter", "=", "3", ";", "peter", "=", "peter", "+", "1000" , ";"
   //         , "var", "peter2", ";", "peter2", "=", "peter2", "+", "1997" , ";", "peter2", "=" , "peter" ,"+","peter2", ";" ,"peter2","+","peter",";"};
//string input[] = {"var", "peter", ";", "peter","=","10",";","while", "(", "peter", ")", "{", "peter", "=", "peter", "-", "1", ";", "peter",";","function:show",";" ,"}", ";"};
//string input[] = {"malloc", "100", ";" };
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