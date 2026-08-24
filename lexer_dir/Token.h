#pragma once
#include <iostream>
using namespace std;

enum class TokenType{
    // Data_Structures
    Int,
    Char,
    Bool,
    Void,

    Number,
   
    Identifier,

    // Operators

    Plus,
    Subtract,
    Division,
    Mutliplication,
    Xor,
    Modulo,
    Bitwise_And,
    Bitwise_Or,
    Greater,
    Less,
    
    
    Less_Equal,
    Equal_Equal,
    Not_Equal, 
    Left_Shift,
    Right_Shift,
    Greater_Equal,

    Increment,  
    Decrement, 
    Bitwise_Not, 

    Logical_Not, 
    Question_Mark,
    Logical_And,
    Logical_Or,

    // keywords
    IF,
    ELSE,
    WHILE,
    CONTINUE,
    BREAK,
    RETURN,
    TRUE,
    PRINT,
    SCAN,
    FALSE,
    // brackets


    Left_parenthesis,
    Right_parenthesis, 
    Left_Brace,
    Right_Brace,
    Left_Bracket,
    Right_Bracket,

    Assign,

    Semicolon,
    Comaa,


    End_Of_File
};

class Token{
    public:

    Token()=default;

    Token(TokenType t,string l)
     :  type(t),
       lexeme(l)
       {}
    

    Token(TokenType t){
        type = t;
    }

    string lexeme;
    TokenType type;
};