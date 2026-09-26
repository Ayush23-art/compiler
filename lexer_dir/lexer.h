#pragma once
#include </compiler/lexer_dir/Token.h>
#include <iostream>
#include <vector>
using namespace std;
class Lexer{
    private:

        bool isAtEnd(){
            return (pos>=source.size());
        }

        char peek(){
            if(isAtEnd())return '\0';
            return source[pos];
        }
        
        char peekNext(){
            if(pos+1 >= source.size())return '\0';
            return source[pos+1];
        }

        char advance(){
            return source[pos++];
        }

        bool Is_digit(char c){
            switch(c){
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                     return true;
                default : return false;
            }
        }

        bool Is_Numeric(string &word){
             for(auto e : word){
                 if(!Is_digit(e))return false;
             } 
             return true;
        }
        bool Is_newtoken(char c){
            switch(c){
                case ' ':
                case '\n':
                case '\t':
                case '+':
                case '-':
                case '*':
                case '/':
                case '^':
                case '|':
                case '&':
                case '=':
                case '>':
                case '<':
                case ',':
                case '%':
                case ';': 
                case '(':
                case ')':
                case '}':
                case '{':
                case '?':
                case '!':
                case '[':
                case ']':
                case '~':
                    return true;
                default:
                   return false;
            }
        }
        
        void Identify(string &word){
            if(word == "int"){
                tokens.push_back(Token(TokenType::Int,word));
            }
            else if(word == "char"){
                tokens.push_back(Token(TokenType::Char,word));
            }
            else if(word == "bool"){
                tokens.push_back(Token(TokenType::Bool,word));
            }
            else if(word == "void"){
                tokens.push_back(Token(TokenType::Void,word));
            }
            else if(word == "if"){
                tokens.push_back(Token(TokenType::IF,word));
            }
            else if(word == "else"){
                tokens.push_back(Token(TokenType::ELSE,word));
            }
            else if(word == "while"){
                tokens.push_back(Token(TokenType::WHILE,word));
            }
            else if(word == "continue"){
                tokens.push_back(Token(TokenType::CONTINUE,word));
            }
            else if(word == "break"){
                tokens.push_back(Token(TokenType::BREAK,word));
            }
            else if(word == "return"){
                tokens.push_back(Token(TokenType::RETURN,word));
            }
            else if(word == "true"){
                tokens.push_back(Token(TokenType::TRUE,word));
            }
            else if(word == "false"){
                tokens.push_back(Token(TokenType::FALSE,word));
            }
            else if(word == "print"){
                tokens.push_back(Token(TokenType::PRINT,word));
            }
            else if(word == "scan"){
                tokens.push_back(Token(TokenType::SCAN,word));
            }
            else if(Is_Numeric(word)){
                tokens.push_back(Token(TokenType::Number,word));
            }
            else {
                tokens.push_back(Token(TokenType::Identifier,word));
            }
        }

        void read_word(){
            string word;
            while(!isAtEnd()){
                char c = peek();
                if(Is_newtoken(c)){
                    Identify(word);
                    return;
                }
                word+=c;
                pos++;
            }
            Identify(word);
        }

        string source;
        size_t pos = 0;

    public:

        Lexer (string src)
        :  source(src)
          {}

        vector<Token>tokens;
        
        vector<Token> tokenize(){
             string word ;
             while(!isAtEnd()){
                 char c = peek();
                 bool Increment_pos = 1;
                 switch(c){
                    case ' ' : break;
                    case '\n': break;
                    case '\t': break;
                    case '+':
                         if(peekNext()=='+'){
                             tokens.push_back(Token(TokenType::Increment,"++"));
                             pos++;
                         }
                         else tokens.push_back(Token(TokenType::Plus,"+"));
                         break;
                    case '-':
                         if(peekNext()=='-'){
                            tokens.push_back(Token(TokenType::Decrement,"--"));
                            pos++;
                         }
                         else tokens.push_back(Token(TokenType::Subtract,"-"));
                         break;
                    case '*':
                         tokens.push_back(Token(TokenType::Mutliplication,"*"));
                         break;
                    case '/':
                         tokens.push_back(Token(TokenType::Division,"/"));
                         break;
                    case '%':
                         tokens.push_back(Token(TokenType::Modulo,"%"));
                         break;
                    case '>':
                         if(peekNext()=='>'){
                            tokens.push_back(Token(TokenType::Right_Shift,">>"));
                            pos++;
                         }
                         else if(peekNext()=='='){
                            tokens.push_back(Token(TokenType::Greater_Equal,">="));
                            pos++;
                         }
                         else{
                            tokens.push_back(Token(TokenType::Greater,">"));
                         }
                         break;
                    case '<':
                         if(peekNext()=='<'){
                            tokens.push_back(Token(TokenType::Left_Shift,"<<"));
                            pos++;
                         }
                         else if(peekNext()=='='){
                            tokens.push_back(Token(TokenType::Less_Equal,"<="));
                            pos++;
                         }
                         else{
                            tokens.push_back(Token(TokenType::Less,"<"));
                         }
                         break;
                    case ';':
                         tokens.push_back(Token(TokenType::Semicolon,";"));
                         break;
                    case ',':
                         tokens.push_back(Token(TokenType::Comaa,","));
                         break;
                    case '^':
                         tokens.push_back(Token(TokenType::Xor,"^"));
                         break;
                    case '|':
                         if(peekNext()=='|'){
                            tokens.push_back(Token(TokenType::Logical_Or,"||"));
                            pos++;
                         }
                         else tokens.push_back(Token(TokenType::Bitwise_Or,"|"));
                         break;
                    case '&':
                         if(peekNext()=='&'){
                            tokens.push_back(Token(TokenType::Logical_And,"&&"));
                            pos++;
                         }
                         else tokens.push_back(Token(TokenType::Bitwise_And,"&"));
                         break;
                    case '=':
                         if(peekNext()=='='){
                            tokens.push_back(Token(TokenType::Equal_Equal,"=="));
                            pos++;
                         }
                         else tokens.push_back(Token(TokenType::Assign,"="));
                         break;
                    case '(':
                         tokens.push_back(Token(TokenType::Left_parenthesis,"("));
                         break;
                    case ')':
                         tokens.push_back(Token(TokenType::Right_parenthesis,")"));
                         break;
                    case '{':
                         tokens.push_back(Token(TokenType::Left_Brace,"{"));
                         break;
                    case '}':
                         tokens.push_back(Token(TokenType::Right_Brace,"}"));
                         break;
                    case '[':
                         tokens.push_back(Token(TokenType::Left_Bracket,"["));
                         break;
                    case ']':
                         tokens.push_back(Token(TokenType::Right_Bracket,"]"));
                         break;
                    case '!':
                         if(peekNext()=='='){
                            tokens.push_back(Token(TokenType::Not_Equal,"!="));
                            pos++;
                         }
                         else tokens.push_back(Token(TokenType::Logical_Not,"!"));
                         break;
                    case '?':
                         tokens.push_back(Token(TokenType::Question_Mark,"?"));
                         break;
                    case '~':
                         tokens.push_back(Token(TokenType::Bitwise_Not,"~"));
                         break;
                    default :
                         Increment_pos = 0;
                         read_word();
                 }
                 if(Increment_pos)pos++;
             }
             tokens.push_back(Token(TokenType::End_Of_File,"EOF"));
             return tokens;
        }
};

string TokenTypeToString(TokenType t) {
    switch (t) {
        // Keywords
        case TokenType::Int:                 return "Int";
        case TokenType::IF:                  return "IF";
        case TokenType::ELSE:                return "ELSE";
        case TokenType::WHILE:               return "WHILE";
        case TokenType::BREAK:               return "BREAK";
        case TokenType::CONTINUE:            return "CONTINUE";

        // Literals
        case TokenType::Number:              return "Number";
        case TokenType::Identifier:          return "Identifier";

        // Arithmetic operators
        case TokenType::Plus:                return "Plus";
        case TokenType::Subtract:            return "Subtract";
        case TokenType::Mutliplication:      return "Multiplication";
        case TokenType::Division:            return "Division";
        case TokenType::Modulo:              return "Modulo";

        // Bitwise operators
        case TokenType::Bitwise_And:         return "Bitwise_And";
        case TokenType::Bitwise_Or:          return "Bitwise_Or";
        case TokenType::Xor:                 return "Xor";
        case TokenType::Left_Shift:          return "Left_Shift";
        case TokenType::Right_Shift:         return "Right_Shift";

        // Comparison operators
        case TokenType::Greater:             return "Greater";
        case TokenType::Greater_Equal:       return "Greater_Equal";
        case TokenType::Less:                return "Less";
        case TokenType::Less_Equal:          return "Less_Equal";
        case TokenType::Equal_Equal:         return "Equal_Equal";

        // Assignment
        case TokenType::Assign:              return "Assign";

        // Delimiters
        case TokenType::Left_parenthesis:    return "Left_parenthesis";
        case TokenType::Right_parenthesis:   return "Right_parenthesis";
        case TokenType::Left_Brace:          return "Left_Brace";
        case TokenType::Right_Brace:         return "Right_Brace";
        case TokenType::Semicolon:           return "Semicolon";
        case TokenType::Comaa:               return "Comma";

        case TokenType::End_Of_File:         return "End_Of_File";

        default:                             return "Unknown";
    }
}
