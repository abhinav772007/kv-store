#include "parser.h"
#include<sstream>

std::vector<std::string> tokenize(const std::string &input){
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string word;
    while(ss>>word){
        tokens.push_back(word);
    }
    return tokens;
}