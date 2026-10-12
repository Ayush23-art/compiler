#pragma once
#include "token.h"
#include <iostream>
#include <vector>

using namespace std;

class Lexer{
    private:
        bool isAtEnd(){
            return pos>=source.size();
        }

        char peek(){
            if(isAtEnd()) return '\0';
            return source[pos];
        }

        char peekNext(){
            if(pos+1 >= source.size()) return '\0';
            return source[pos+1];
        }

        char advance(){
            return source[pos++];
        }

        bool Is_Digit(char c) {
            return (c >= '0' && c <= '9');
        }


        bool Is_Numeric(string &word){
            for(char c: word){
                if(!Is_Digit(c)) return false;
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
                    tokens.push_back(Token(Token_Type::Int,word));
                }
                else if(word == "char"){
                    tokens.push_back(Token(Token_Type::Char,word));
                }
                else if(word == "string"){
                    tokens.push_back(Token(Token_Type::String,word));
                }

                else if(word == "bool"){
                    tokens.push_back(Token(Token_Type::Bool,word));
                }
                else if(word == "void"){
                    tokens.push_back(Token(Token_Type::Void,word));
                }
                else if(word == "if"){
                    tokens.push_back(Token(Token_Type::IF,word));
                }
                else if(word == "else"){
                    tokens.push_back(Token(Token_Type::ELSE,word));
                }
                else if(word == "while"){
                    tokens.push_back(Token(Token_Type::WHILE,word));
                }
                else if(word == "continue"){
                    tokens.push_back(Token(Token_Type::CONTINUE,word));
                }
                else if(word == "break"){
                    tokens.push_back(Token(Token_Type::BREAK,word));
                }
                else if(word == "return"){
                    tokens.push_back(Token(Token_Type::RETURN,word));
                }
                else if(word == "true"){
                    tokens.push_back(Token(Token_Type::TRUE,word));
                }
                else if(word == "false"){
                    tokens.push_back(Token(Token_Type::FALSE,word));
                }
                else if(word == "print"){
                    tokens.push_back(Token(Token_Type::PRINT,word));
                }
                else if(word == "scan"){
                    tokens.push_back(Token(Token_Type::SCAN,word));
                }
                else if(Is_Numeric(word)){
                    tokens.push_back(Token(Token_Type::Number,word));
                }
                else {
                    tokens.push_back(Token(Token_Type::Identifier,word));
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

            void read_string(){
                advance();

                string value;
                while(!isAtEnd() && peek() !='"'){
                    value+=advance();
                }
                if(isAtEnd()){
                    throw runtime_error("Unterminated String Literal");
                }
                advance();
                tokens.push_back(Token(Token_Type::StringLiteral, value));
            }

            string source;
            size_t pos = 0;

    public:
        Lexer(string src){
            this->source = src;
        }

        vector<Token> tokens;
        vector<Token> tokenize(){
            string word;
            while(!isAtEnd()){
                char c = peek();
                bool Increment_pos = 1;
                switch(c){
                    case ' ' : break;
                    case '\n' : break;
                    case '\t' : break;
                    case '\\':
                        if(peekNext() == 'n'){
                            tokens.push_back(Token(Token_Type::NewLine, "\n"));
                        }
                        else if(peekNext() == 't'){
                            token.push_back(Token(Token_Type::Tab, "\t"));
                        }
                    case '"':
                        read_string();
                        Increment_pos = 0;

                        break;
                    case '+' : 
                        if(peekNext() == '+'){
                            tokens.push_back(Token(Token_Type::Increment, "++"));
                            advance();
                        }
                        else tokens.push_back(Token(Token_Type::Plus, "+"));
                        break;
                    case '-':
                        if(peekNext()=='-'){
                            tokens.push_back(Token(Token_Type::Decrement,"--"));
                            pos++;
                        }
                        else tokens.push_back(Token(Token_Type::Subtract,"-"));
                        break;
                    case '*':
                        tokens.push_back(Token(Token_Type::Multiplication,"*"));
                        break;
                    case '/':
                        tokens.push_back(Token(Token_Type::Division,"/"));
                        break;
                    case '%':
                        tokens.push_back(Token(Token_Type::Modulo,"%"));
                        break;

                    case '>':
                        if(peekNext()=='>'){
                            tokens.push_back(Token(Token_Type::Right_Shift,">>"));
                            pos++;
                        }
                        else if(peekNext()=='='){
                            tokens.push_back(Token(Token_Type::Greater_Equal,">="));
                            pos++;
                        }
                        else{
                        tokens.push_back(Token(Token_Type::Greater,">"));
                        }
                        break;
                    case '<':
                        if(peekNext()=='<'){
                            tokens.push_back(Token(Token_Type::Left_Shift,"<<"));
                            pos++;
                        }
                        else if(peekNext()=='='){
                            tokens.push_back(Token(Token_Type::Less_Equal,"<="));
                            pos++;
                        }
                        else{
                            tokens.push_back(Token(Token_Type::Less,"<"));
                        }
                        break;
                    case ';':
                        tokens.push_back(Token(Token_Type::Semicolon,";"));
                        break;
                    case ',':
                        tokens.push_back(Token(Token_Type::Comma,","));
                        break;
                    case '^':
                        tokens.push_back(Token(Token_Type::Xor,"^"));
                        break;
                    case '|':
                        if(peekNext()=='|'){
                            tokens.push_back(Token(Token_Type::Logical_Or,"||"));
                            pos++;
                        }
                        else tokens.push_back(Token(Token_Type::Bitwise_Or,"|"));
                        break;
                    case '&':
                        if(peekNext()=='&'){
                        tokens.push_back(Token(Token_Type::Logical_And,"&&"));
                        pos++;
                        }
                        else tokens.push_back(Token(Token_Type::Bitwise_And,"&"));
                        break;
                    case '=':
                        if(peekNext()=='='){
                        tokens.push_back(Token(Token_Type::Equal_Equal,"=="));
                        pos++;
                        }
                        else tokens.push_back(Token(Token_Type::Assign,"="));
                        break;
                    case '(':
                        tokens.push_back(Token(Token_Type::Left_parenthesis,"("));
                        break;
                    case ')':
                        tokens.push_back(Token(Token_Type::Right_parenthesis,")"));
                        break;
                    case '{':
                        tokens.push_back(Token(Token_Type::Left_Brace,"{"));
                        break;
                    case '}':
                        tokens.push_back(Token(Token_Type::Right_Brace,"}"));
                        break;
                    case '[':
                        tokens.push_back(Token(Token_Type::Left_Bracket,"["));
                        break;
                    case ']':
                        tokens.push_back(Token(Token_Type::Right_Bracket,"]"));
                        break;
                    case '!':
                        if(peekNext()=='='){
                            tokens.push_back(Token(Token_Type::Not_Equal,"!="));
                            pos++;
                        }
                        else tokens.push_back(Token(Token_Type::Logical_Not,"!"));
                        break;
                    case '?':
                        tokens.push_back(Token(Token_Type::Question_Mark,"?"));
                        break;
                    case '~':
                        tokens.push_back(Token(Token_Type::Bitwise_Not,"~"));
                        break;
                    default :
                        Increment_pos = 0;
                        read_word();
                }
                if(Increment_pos) advance();
            }
            tokens.push_back(Token(Token_Type::End_Of_File,"EOF"));
            return tokens;
        }
};

string Token_TypeToString(Token_Type t) {
    switch (t) {
        // Keywords
        case Token_Type::Int:                 return "Int";
        case Token_Type::IF:                  return "IF";
        case Token_Type::ELSE:                return "ELSE";
        case Token_Type::WHILE:               return "WHILE";
        case Token_Type::BREAK:               return "BREAK";
        case Token_Type::CONTINUE:            return "CONTINUE";

        // Literals
        case Token_Type::Number:              return "Number";
        case Token_Type::Identifier:          return "Identifier";
        case Token_Type::String:              return "String";
        case Token_Type::StringLiteral:       return "StringLiteral";
        case Token_Type::NewLine:             return "NewLine";
        case Token_Type::Tab:                 return "Tab";

        // Arithmetic operators
        case Token_Type::Plus:                return "Plus";
        case Token_Type::Subtract:            return "Subtract";
        case Token_Type::Multiplication:      return "Multiplication";
        case Token_Type::Division:            return "Division";
        case Token_Type::Modulo:              return "Modulo";

        // Bitwise operators
        case Token_Type::Bitwise_And:         return "Bitwise_And";
        case Token_Type::Bitwise_Or:          return "Bitwise_Or";
        case Token_Type::Xor:                 return "Xor";
        case Token_Type::Left_Shift:          return "Left_Shift";
        case Token_Type::Right_Shift:         return "Right_Shift";

        // Comparison operators
        case Token_Type::Greater:             return "Greater";
        case Token_Type::Greater_Equal:       return "Greater_Equal";
        case Token_Type::Less:                return "Less";
        case Token_Type::Less_Equal:          return "Less_Equal";
        case Token_Type::Equal_Equal:         return "Equal_Equal";

        // Assignment
        case Token_Type::Assign:              return "Assign";

        // Delimiters
        case Token_Type::Left_parenthesis:    return "Left_parenthesis";
        case Token_Type::Right_parenthesis:   return "Right_parenthesis";
        case Token_Type::Left_Brace:          return "Left_Brace";
        case Token_Type::Right_Brace:         return "Right_Brace";
        case Token_Type::Semicolon:           return "Semicolon";
        case Token_Type::Comma:               return "Comma";

        case Token_Type::End_Of_File:         return "End_Of_File";

        default:                             return "Unknown";
    }
}