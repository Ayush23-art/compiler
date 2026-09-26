#pragma once

#include </compiler/lexer_dir/Token.h>
#include </compiler/lexer_dir/lexer.h>
#include </compiler/semantic_analyzer/datatype.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>

using namespace std;

class Stmt{
   public:
      virtual ~Stmt() = default;
};

class Expr{
   public:
      DataType type;
      virtual ~Expr() = default;     
};

// Expr childs
class FunctionExpr : public Expr{
   public:
      string name;
      vector<Expr*> prms;
      FunctionExpr(string name,vector<Expr*> prms)
      : name(name),
        prms(prms)
        {}
};

class CharExpr: public Expr{
   public:
     char value;
     CharExpr(char value)
     : value(value)
       {}
};

class BoolExpr: public Expr{
   public:
     bool value;
     BoolExpr(bool value)
     : value(value)
       {}
};

class NumberExpr : public Expr{
   public:
   int value;
   NumberExpr(int value){
      this->value = value;
   }
};

class AssignExpr: public Expr{
   public:
      string name;
      Expr* value;
      AssignExpr(string name,Expr* value)
      : name(name),value(value) {}
};

class IdentifierExpr : public Expr{
   public:
   string name;
   IdentifierExpr(string name)
   :   name(name)
       {}
};

class BinaryExpr : public Expr{
   public:
   Expr* left;
   Token op;
   Expr* right;
   BinaryExpr(Expr* left, const Token& op, Expr* right)
    : left(left), op(op), right(right) {}
};

// Statements
class PrintStatement:public Stmt{
   public:
      Token keyword;
      vector<Expr*>values;
      PrintStatement(Token keyword,vector<Expr*>values)
      : keyword(keyword),
        values(values)
        {}
};

class ScanStatement:public Stmt{
   public:
      Token keyword;
      vector<Expr*>inputs;
      ScanStatement(Token keyword,vector<Expr*>inputs)
      : keyword(keyword),
        inputs(inputs)
        {}
};

class BlockStmt : public Stmt{
   public:
      vector<Stmt*>statements;     
};

class ExpressionStmt : public Stmt{
   public:
      Expr* expression;
      ExpressionStmt(Expr* expression){
         this->expression = expression;
      }
};

class VariableDeclaration : public Stmt {
   public:
      DataType type;
      string name;
      Expr* initializer;
      VariableDeclaration(DataType type,string name,Expr*initializer)
      :   type(type),
         name(name),
         initializer(initializer)
         {}
};

class IfStatement:public Stmt{
   public:
      Token keyword;
      Expr* condition;
      Stmt* thenBranch;
      Stmt* elseBranch;
      IfStatement(Token keyword,Expr* condition, Stmt* thenBranch,Stmt* elseBranch)
       : keyword(keyword),
         condition(condition),
         thenBranch(thenBranch), 
         elseBranch(elseBranch)
         {}
};

class WhileStatement:public Stmt{
   public:
      Token keyword;
      Expr* condition;
      BlockStmt* blockstmt;
      WhileStatement(Token keyword,Expr*condition,BlockStmt* blockstmt)
      : keyword(keyword),
        condition(condition),
        blockstmt(blockstmt)
        {}
};

class ReturnStatement:public Stmt{
   public:
      Token keyword;
      Expr* value;
      ReturnStatement(Token keyword,Expr* value)
      : keyword(keyword),
        value(value)
        {}
};

class Parameter{
   public:
      DataType type;
      string name;
      Parameter(DataType type, string name)
      : type(type),
        name(name)
        {}
};

class FunctionDeclaration:public Stmt{
   public:
      DataType type;
      string name;
      vector<Parameter>parameters;
      BlockStmt* blockstmt;
      FunctionDeclaration(DataType type,string name,vector<Parameter>parameters,BlockStmt* blockstmt)
      : type(type),
        name(name),
        parameters(parameters),
        blockstmt(blockstmt)
        {}
};

class Program{
   public:
      vector<Stmt*>statements;
};

class Parser{
   
   private:
   
   vector<Token>tokens;
   size_t current = 0;
   
   public:
   
   Parser(vector<Token>tokens){
      this->tokens = tokens;
   }
   Program* parse();
   
   private:

   // grammar rules
   Stmt* declaration();
   Stmt* variableDeclaration();
   Stmt* exprstatement();
   BlockStmt* blockstatement();
   IfStatement* ifstatement();
   WhileStatement* whilestatement();
   ReturnStatement* returnstatement();
   FunctionDeclaration* functionstatement();
   Stmt* printstatement();
   Stmt* scanstatement();

   Expr* expression();
   Expr* assignment();
   Expr* logical_or();
   Expr* logical_and();
   Expr* notequal();
   Expr* bitwise_or();
   Expr* bitwise_xor();
   Expr* bitwise_and();
   Expr* equality();
   Expr* relational();
   Expr* shift();
   Expr* additive();
   Expr* multiplicative();
   Expr* primary();
   Expr* functionexpression();
   // utility function
   Token& peek(); // done
   Token& previous(); // done
   Token& peek(int offset);

   bool isAtEnd(); // done

   bool isType(TokenType type);
   Token& consumeType();
   Token& advance(); // done 
   
   bool check(TokenType type); // done
   bool match(initializer_list<TokenType>types); // done 
   
   Token& consume(TokenType type, const string& message);  // done 
   
   DataType tokentoDatatype(const Token& token);
   // void synchronize();

};

// Helper functions 
bool Parser :: isType(TokenType type){
    return type == TokenType::Int
            || type == TokenType :: Char
            || type == TokenType :: Bool
            || type == TokenType :: Void;
}

Token& Parser :: consumeType(){
    if(isType(peek().type)){
        return advance();
    }
    throw runtime_error("Expected Datatype");
}

DataType Parser:: tokentoDatatype(const Token&token){
    switch(token.type){
         case TokenType::Int:
            return DataType::Int;
         case TokenType::Char:
            return DataType::Char;
         case TokenType::Bool:
            return DataType::Bool;
         case TokenType::Void:
            return DataType::Void;
         default:
            throw runtime_error("Unknown Datatype");
    }
}

Token& Parser::peek(){
   return tokens[current];
}

Token& Parser::peek(int offset){
   if(current+offset >= tokens.size()){
      return tokens.back();
   }
   return tokens[current+offset];
}

Token& Parser::previous(){
   return tokens[current-1];
}

bool Parser::isAtEnd(){
   return peek().type == TokenType::End_Of_File;
}

Token& Parser::advance(){
   if(!isAtEnd()){
      current++;
   }
   return previous();
}

bool Parser::check(TokenType type){
   return peek().type == type;
}

bool Parser::match(initializer_list<TokenType>types){
   for(auto type:types){
      if(check(type)){
          advance();
          return true;
      }
   }
   return false;
}

Token& Parser::consume(TokenType type,const string&message){
   if(check(type)){
      return advance();
   }
   throw runtime_error(message);
}

// Grammar 

Program* Parser::parse(){
   Program* program = new Program();
   while(!isAtEnd()){
     program->statements.push_back(declaration());
   }
   return program;
}

Stmt* Parser::declaration(){
   if(isType(peek(0).type)
    && peek(1).type == TokenType::Identifier
     && peek(2).type == TokenType::Left_parenthesis){
       return functionstatement();
   }
   else if(isType(peek().type)){
      return variableDeclaration();
   }
   else if(check(TokenType::Identifier)){
      return exprstatement();
   }
   else if(check(TokenType::Left_Brace)){
       return blockstatement();
   }
   else if(check(TokenType::IF)){
      return ifstatement();
   }
   else if(check(TokenType::WHILE)){
      return whilestatement();
   }
   else if(check(TokenType::RETURN)){
      return returnstatement();
   }
   else if(check(TokenType::PRINT)){
      return printstatement();
   }
   else if(check(TokenType::SCAN)){
      return scanstatement();
   }
   throw runtime_error("Expected Declartion");
}

Stmt* Parser :: printstatement(){
   Token keyword = advance();
   consume(TokenType::Left_parenthesis,"Expected (");
   if(check(TokenType::Right_parenthesis)){
      throw runtime_error("there is nothing to print");
   }
   vector<Expr*>values;
   Expr* value = expression();
   values.push_back(value);
   while(match({TokenType::Comaa})){
      value = expression(); 
      values.push_back(value);
   }
   consume(TokenType::Right_parenthesis,"Expected )");
   consume(TokenType :: Semicolon,"Expected ;");
   return new PrintStatement(keyword,values);
}

Stmt* Parser::scanstatement(){
   Token keyword = advance();
   consume(TokenType::Left_parenthesis,"Expected (");
   if(check(TokenType::Right_parenthesis)){
      throw runtime_error("there is nothing to take as input");
   }
   vector<Expr*>inputs;
   Expr* value = expression();
   inputs.push_back(value);
   while(match({TokenType::Comaa})){
      value = expression();
      inputs.push_back(value);
   }
   consume(TokenType::Right_parenthesis,"Expected )");
   consume(TokenType:: Semicolon , "Expected ;");
   return new ScanStatement(keyword,inputs);
}

Stmt* Parser::variableDeclaration(){
   DataType type = tokentoDatatype(consumeType());
   Token &name = consume(TokenType::Identifier,"Expected Identifier");
   Expr* initializer = nullptr;
   if(match({TokenType::Assign})){
        initializer = expression();
   }
   consume(TokenType::Semicolon,"Expected Semicolon");
   return new VariableDeclaration(type,name.lexeme,initializer);
}

Stmt* Parser::exprstatement(){
   Expr* expr = expression();
   consume(TokenType::Semicolon,"Expected Semicolon");
   return new ExpressionStmt(expr);
}

BlockStmt* Parser::blockstatement(){
   consume(TokenType::Left_Brace,"Expected Left_Brace");
   BlockStmt* blockstmt = new BlockStmt();
   while(!isAtEnd() && !check(TokenType::Right_Brace)){
      blockstmt->statements.push_back(declaration());
   }
   consume(TokenType::Right_Brace,"Expected Right_Brace");
   return blockstmt;
}

IfStatement* Parser::ifstatement(){
   consume(TokenType::IF,"Expected if");
   Token keyword = previous();
   consume(TokenType::Left_parenthesis,"Expected (");
   Expr* condition = expression();
   consume(TokenType::Right_parenthesis,"Expected )");
   Stmt* thenbranch = blockstatement();
   Stmt* elsebranch = nullptr;
   if(match({TokenType::ELSE})){
       if(check(TokenType::IF)){
          elsebranch = ifstatement();
       }
       else elsebranch = blockstatement();
   }
   return new IfStatement(keyword,condition,thenbranch,elsebranch);
}

WhileStatement * Parser::whilestatement(){
   Token keyword = consume(TokenType::WHILE,"Expected while");
   consume(TokenType::Left_parenthesis,"Expected (");
   Expr* condition = expression();
   consume(TokenType::Right_parenthesis,"Expected )");
   BlockStmt* blockstmt = blockstatement();
   return new WhileStatement(keyword,condition,blockstmt);
}

ReturnStatement* Parser::returnstatement(){
   Token keyword = consume(TokenType::RETURN,"Expected return");
   Expr* value = nullptr;
   if(!check(TokenType::Semicolon)){
      value = expression();
   }
   consume(TokenType::Semicolon,"Expected ;");
   return new ReturnStatement(keyword,value);
}

FunctionDeclaration* Parser:: functionstatement(){
   DataType type = tokentoDatatype(consumeType());
   string name = consume(TokenType::Identifier,"Expected Identifier").lexeme;
   consume(TokenType::Left_parenthesis,"Expected (");
   vector<Parameter>prms;
   bool found_comma = true;
   while(!isAtEnd() && !check(TokenType::Right_parenthesis) && found_comma){
        DataType ptype = tokentoDatatype(consumeType());
        string pname;
        consume(TokenType::Identifier,"Expected Identifier");
        pname = previous().lexeme;
        found_comma = match({TokenType::Comaa});
        prms.push_back({ptype,pname});
   }
   consume(TokenType::Right_parenthesis,"Expected )");
   BlockStmt* blockstmt = nullptr;
   blockstmt = blockstatement();
   return new FunctionDeclaration(type,name,prms,blockstmt);
}

Expr* Parser:: assignment(){
     Expr* expr = logical_or();
     if(match({TokenType::Assign})){
       Expr* right = assignment();
       auto id = dynamic_cast<IdentifierExpr*>(expr);
       if(id!=nullptr){
          return new AssignExpr(id->name,right);
       }
       else throw runtime_error("Invalid assignment target.");
     }
     return expr;
}

Expr* Parser:: logical_or(){
    Expr* expr = logical_and();
    while(match({TokenType::Logical_Or})){
        Token op = previous();
        Expr* right = logical_and();
        expr = new BinaryExpr(expr,op,right);
    }
    return expr;
}

Expr* Parser:: logical_and(){
    Expr* expr = notequal();
    while(match({TokenType::Logical_And})){
        Token op = previous();
        Expr* right = notequal();
        expr = new BinaryExpr(expr,op,right);
    }
    return expr;
}

Expr*Parser:: notequal(){
    Expr* expr = bitwise_or();
    while(match({TokenType::Not_Equal})){
        Token op = previous();
        Expr* right = bitwise_or();
        expr = new BinaryExpr(expr,op,right);
    }
    return expr;
}

Expr* Parser:: bitwise_or(){
    Expr* expr = bitwise_xor();
    while(match({TokenType::Bitwise_Or})){
        Token op = previous();
        Expr* right = bitwise_xor();
        expr = new BinaryExpr(expr,op,right);
    }
    return expr;
}

Expr* Parser:: bitwise_xor(){
     Expr* expr = bitwise_and();
     while(match({TokenType::Xor})){
        Token op = previous();
        Expr* right = bitwise_and();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: bitwise_and(){
     Expr* expr = equality();
     while(match({TokenType::Bitwise_And})){
        Token op = previous();
        Expr* right = equality();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: equality(){
     Expr* expr = relational();
     while(match({TokenType::Equal_Equal})){
        Token op = previous();
        Expr* right = relational();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: relational(){
     Expr* expr = shift();
     while(match({TokenType::Less,TokenType::Less_Equal,TokenType::Greater,TokenType::Greater_Equal})){
        Token op = previous();
        Expr* right = shift();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: shift(){
     Expr* expr = additive();
     while(match({TokenType::Left_Shift,TokenType::Right_Shift})){
        Token op = previous();
        Expr* right = additive();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: additive(){
     Expr* expr = multiplicative();
     while(match({TokenType::Plus,TokenType::Subtract})){
        Token op = previous();
        Expr* right = multiplicative();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: multiplicative(){
     Expr* expr = primary();
     while(match({TokenType::Mutliplication,TokenType::Modulo,TokenType::Division})){
        Token op = previous();
        Expr* right = primary();
        expr = new BinaryExpr(expr,op,right);
     }
     return expr;
}

Expr* Parser:: functionexpression(){
     string name = previous().lexeme;
     consume(TokenType::Left_parenthesis,"Expected (");
     vector<Expr*>prms;
     if(!check(TokenType::Right_parenthesis)){
        prms.push_back(expression());
        while(match({TokenType::Comaa})){
            prms.push_back(expression());
        }
     }
     consume(TokenType::Right_parenthesis,"Expected )");
     return new FunctionExpr(name,prms);
}

Expr* Parser :: primary(){
     if(match({TokenType::Number})){
         int value = stoi(previous().lexeme);
         return new NumberExpr(value);
     }
     else if(match({TokenType::Identifier})){
         string name = previous().lexeme;
         if(match({TokenType::Left_parenthesis})){
              current--;
              return functionexpression();
         }
         else return new IdentifierExpr(previous().lexeme);
     }
     else if(match({TokenType::Left_parenthesis})){
         Expr* expr = expression();
         consume(TokenType::Right_parenthesis,"Expected )");
         return expr;
     }
     throw runtime_error("Expected expression is missing");
}

Expr* Parser::expression(){
    return assignment();
}

// Printing the Abstract syntax tree

class AstPrinter {
public:

    void print(Program* program) {
        cout << "Program\n";
        for (Stmt* stmt : program->statements)
            printStmt(stmt, 1);
    }

private:

    void indent(int level) {
        while (level--)
            cout << "    ";
    }

    //---------------------- Statements ----------------------

    void printStmt(Stmt* stmt, int level) {

        if (!stmt) return;

        if (auto x = dynamic_cast<VariableDeclaration*>(stmt)) {

            indent(level);
            cout << "VariableDeclaration\n";

            indent(level + 1);
            cout << "Type : " << datatypeToString(x->type) << '\n';

            indent(level + 1);
            cout << "Name : " << x->name << '\n';

            if (x->initializer) {
                indent(level + 1);
                cout << "Initializer\n";
                printExpr(x->initializer, level + 2);
            }

            return;
        }

        if (auto x = dynamic_cast<FunctionDeclaration*>(stmt)) {

            indent(level);
            cout << "FunctionDeclaration\n";

            indent(level + 1);
            cout << "Return Type : " << datatypeToString(x->type) << '\n';

            indent(level + 1);
            cout << "Name : " << x->name << '\n';

            indent(level + 1);
            cout << "Parameters\n";

            for (auto& p : x->parameters) {
                indent(level + 2);
                cout << datatypeToString(p.type)
                     << " " << p.name << '\n';
            }

            indent(level + 1);
            cout << "Body\n";

            printStmt(x->blockstmt, level + 2);
            return;
        }

        if (auto x = dynamic_cast<BlockStmt*>(stmt)) {

            indent(level);
            cout << "Block\n";

            for (auto s : x->statements)
                printStmt(s, level + 1);

            return;
        }

        if (auto x = dynamic_cast<PrintStatement*>(stmt)) {

            indent(level);
            cout << "PrintStatement\n";
            for(auto value:x->values){
                printExpr(value, level + 1);
            }

            return;
        }

        if (auto x = dynamic_cast<ExpressionStmt*>(stmt)) {

            indent(level);
            cout << "ExpressionStatement\n";

            printExpr(x->expression, level + 1);

            return;
        }

        if (auto x = dynamic_cast<ReturnStatement*>(stmt)) {

            indent(level);
            cout << "ReturnStatement\n";

            if (x->value)
                printExpr(x->value, level + 1);

            return;
        }

        if (auto x = dynamic_cast<IfStatement*>(stmt)) {

            indent(level);
            cout << "IfStatement\n";

            indent(level + 1);
            cout << "Condition\n";
            printExpr(x->condition, level + 2);

            indent(level + 1);
            cout << "Then\n";
            printStmt(x->thenBranch, level + 2);

            if (x->elseBranch) {
                indent(level + 1);
                cout << "Else\n";
                printStmt(x->elseBranch, level + 2);
            }

            return;
        }
        if(auto x = dynamic_cast<ScanStatement*>(stmt)){
           indent(level);
           cout << "ScanStatement\n";
           for(auto id : x->inputs){
              printExpr(id,level+1);
           }
        }
        if (auto x = dynamic_cast<WhileStatement*>(stmt)) {

            indent(level);
            cout << "WhileStatement\n";

            indent(level + 1);
            cout << "Condition\n";
            printExpr(x->condition, level + 2);

            indent(level + 1);
            cout << "Body\n";
            printStmt(x->blockstmt, level + 2);

            return;
        }
    }

    //---------------------- Expressions ----------------------

    void printExpr(Expr* expr, int level) {

        if (!expr) return;

        if (auto x = dynamic_cast<NumberExpr*>(expr)) {

            indent(level);
            cout << "Number(" << x->value << ")\n";
            return;
        }

        if (auto x = dynamic_cast<IdentifierExpr*>(expr)) {

            indent(level);
            cout << "Identifier(" << x->name << ")\n";
            return;
        }

        if (auto x = dynamic_cast<AssignExpr*>(expr)) {

            indent(level);
            cout << "Assign\n";

            indent(level + 1);
            cout << "Variable : " << x->name << '\n';

            indent(level + 1);
            cout << "Value\n";
            printExpr(x->value, level + 2);

            return;
        }

        if (auto x = dynamic_cast<BinaryExpr*>(expr)) {

            indent(level);
            cout << "Binary(" << x->op.lexeme << ")\n";

            indent(level + 1);
            cout << "Left\n";
            printExpr(x->left, level + 2);

            indent(level + 1);
            cout << "Right\n";
            printExpr(x->right, level + 2);

            return;
        }

        if (auto x = dynamic_cast<FunctionExpr*>(expr)) {

            indent(level);
            cout << "FunctionCall(" << x->name << ")\n";

            for (auto arg : x->prms)
                printExpr(arg, level + 1);

            return;
        }

        if (auto x = dynamic_cast<CharExpr*>(expr)) {

            indent(level);
            cout << "Char('" << x->value << "')\n";
            return;
        }

        if (auto x = dynamic_cast<BoolExpr*>(expr)) {

            indent(level);
            cout << "Bool(" << (x->value ? "true" : "false") << ")\n";
            return;
        }
    }

    string datatypeToString(DataType type) {

        switch (type) {
            case DataType::Int:  return "int";
            case DataType::Char: return "char";
            case DataType::Bool: return "bool";
            case DataType::Void: return "void";
        }

        return "unknown";
    }
};
