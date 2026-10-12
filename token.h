#pragma once

#include <string>

enum class Token_Type {
    Int,
    Char,
    Bool,
    Void,
    String,
    StringLiteral,
    Number,
    Identifier,

    Plus,
    Subtract,
    Division,
    Multiplication,
    Xor,
    Modulo,
    Bitwise_And,
    Bitwise_Or,
    Bitwise_Not,
    NewLine,
    Tab,

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

    Logical_Not,
    Question_Mark,
    Logical_And,
    Logical_Or,

    IF,
    ELSE,
    WHILE,
    CONTINUE,
    BREAK,
    RETURN,
    TRUE,
    FALSE,
    PRINT,
    SCAN,

    Left_parenthesis,
    Right_parenthesis,
    Left_Brace,
    Right_Brace,
    Left_Bracket,
    Right_Bracket,

    Assign,

    Semicolon,
    Comma,

    End_Of_File
};

class Token {
public:

    Token() = default;

    Token(Token_Type t, std::string l) {
        this->type = t;
        this->lexeme = l;
    }

    Token(Token_Type t) {
        this->type = t;
    }

    std::string lexeme;
    Token_Type type;
};