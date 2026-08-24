#pragma once
#include </Users/shyampremi/Desktop/project/compiler/lexer_dir/Token.h>
#include </Users/shyampremi/Desktop/project/compiler/lexer_dir/lexer.h>
#include </Users/shyampremi/Desktop/project/compiler/semantic_analyzer/datatype.h>
#include </Users/shyampremi/Desktop/project/compiler/parser_dir/parser.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <unordered_map>

using namespace std;

enum class SymbolKind{
    Variable,
    Function
};

class Symbol{
    public:
      string name;
      Symbol(string name)
      :name(name) 
       {}
      virtual ~Symbol() = default;
};

class VariableSymbol:public Symbol{
    public:
       DataType type;
       int scopedepth;
       VariableSymbol(DataType type,string name,int scopedepth)
       : Symbol(name),
         scopedepth(scopedepth),
         type(type)
         {}
};

class FunctionSymbol:public Symbol{
    public:
       DataType type;
       vector<Parameter>parameters;
       FunctionSymbol(DataType type,string name,vector<Parameter> parameters)
       : Symbol(name),
         type(type),
         parameters(parameters)
         {}
};

class SymbolTable{
    private:
       unordered_map<string,Symbol*>symbols;
    public:
       bool insert(Symbol * symbol);
       Symbol* lookup(const string&name);
       bool exists(const string&name);
};

bool SymbolTable:: insert(Symbol *symbol){
    if(symbols.find(symbol->name)!=symbols.end()){
        return false;
    }
    symbols[symbol->name] = symbol;
    return true;
}

Symbol* SymbolTable::lookup(const string&name){
    if(symbols.find(name)==symbols.end())return nullptr;
    return symbols[name];
}

bool SymbolTable::exists(const string&name){
    if(symbols.find(name)==symbols.end())return false;
    return true;
}

class SemanticAnalyzer{
    private:
        vector<SymbolTable>scopes;
        FunctionSymbol* currentfunction = nullptr;
        bool hasreturn = false;
        void beginScope();
        void endScope();
        SymbolTable& currentscope();
        bool exists(const string&name);
        Symbol* lookup(const string&name);
        
    public:
        void analyze(Program* program);
        DataType analyzeExpression(Expr* expr);
    private:
        void analyzeStatement(Stmt* stmt);

        void analyzeVariableDeclaration(VariableDeclaration* stmt);
        void analyzeFunctionDeclaration(FunctionDeclaration* stmt);
        void analyzeBlockStatement(BlockStmt* stmt,bool createscope);
        void analyzeIfStatement(IfStatement* stmt);
        void analyzeWhileStatement(WhileStatement* stmt);
        void analyzeReturnStatement(ReturnStatement* stmt);
        void analyzeExpressionStatement(ExpressionStmt* stmt);     
        void analyzeprintstatement(PrintStatement* stmt);
        void analyzescanstatement(ScanStatement* stmt);

        DataType analyzeBinaryExpression(BinaryExpr* expr);
        DataType analyzeAssignExpression(AssignExpr* expr);
        DataType analyzeIdentifierExpression(IdentifierExpr* expr);
        DataType analyzeFunctionCallExpression(FunctionExpr* expr);
        DataType analyzeNumberExpression(NumberExpr* expr);
        DataType analyzeCharExpression(CharExpr* expr);
        DataType analyzeBoolExpression(BoolExpr* expr);

        //helper
};


void SemanticAnalyzer :: analyze(Program* program){
    scopes.push_back(SymbolTable());
    for(auto stmt:program->statements){
        analyzeStatement(stmt);
    }
}

void SemanticAnalyzer :: analyzeStatement(Stmt* stmt){
    if(auto x = dynamic_cast<VariableDeclaration*>(stmt)){
        analyzeVariableDeclaration(x);
        return ;
    }
    else if(auto x = dynamic_cast<FunctionDeclaration*>(stmt)){
        analyzeFunctionDeclaration(x);
        return;
    }
    else if(auto x = dynamic_cast<BlockStmt*>(stmt)){
        analyzeBlockStatement(x,1);
        return;
    }
    else if(auto x = dynamic_cast<IfStatement*>(stmt)){
        analyzeIfStatement(x);
        return;
    }
    else if(auto x = dynamic_cast<WhileStatement*>(stmt)){
        analyzeWhileStatement(x);
        return;
    }
    else if(auto x = dynamic_cast<ReturnStatement*>(stmt)){
        analyzeReturnStatement(x);
        return;
    }
    else if(auto x = dynamic_cast<ExpressionStmt*>(stmt)){
        analyzeExpressionStatement(x);
        return;
    }
    else if(auto x = dynamic_cast<PrintStatement*>(stmt)){
        analyzeprintstatement(x);
        return;
    }
    else if(auto x = dynamic_cast<ScanStatement*>(stmt)){
        analyzescanstatement(x);
        return;
    }
    throw runtime_error("Unkown statement");
}
//helper semantic analyzer
void SemanticAnalyzer :: beginScope(){
    scopes.push_back(SymbolTable());
}

void SemanticAnalyzer :: endScope(){
    scopes.pop_back();
}

SymbolTable& SemanticAnalyzer :: currentscope(){
    return scopes.back();
}

bool SemanticAnalyzer :: exists(const string&name){
    for(int i = scopes.size()-1;i>=0;i--){
        if(scopes[i].exists(name))return true;
    }
    return false;
}

Symbol* SemanticAnalyzer :: lookup(const string&name){
    for(int i = scopes.size()-1;i>=0;i--){
        Symbol* symbol = scopes[i].lookup(name);
        if(symbol!=nullptr)return symbol;
    }
    return nullptr;
}

void SemanticAnalyzer :: analyzeExpressionStatement(ExpressionStmt*stmt){
    DataType dt = analyzeExpression(stmt->expression);
    stmt->expression->type = dt;
}

DataType SemanticAnalyzer :: analyzeExpression(Expr* expr){
    if(auto x = dynamic_cast<BinaryExpr*>(expr)){
       return analyzeBinaryExpression(x);
    }
    else if(auto x = dynamic_cast<AssignExpr*>(expr)){
       return analyzeAssignExpression(x);
    }
    else if(auto x = dynamic_cast<IdentifierExpr*>(expr)){
       return analyzeIdentifierExpression(x);
    }
    else if(auto x = dynamic_cast<FunctionExpr*>(expr)){
       return analyzeFunctionCallExpression(x);
    }
    else if(auto x = dynamic_cast<NumberExpr*>(expr)){
       return analyzeNumberExpression(x);
    }
    else if(auto x = dynamic_cast<CharExpr*>(expr)){
       return analyzeCharExpression(x);
    }
    else if(auto x = dynamic_cast<BoolExpr*>(expr)){
       return analyzeBoolExpression(x);
    }
    throw runtime_error("Unknown expression type");
}


void SemanticAnalyzer :: analyzeVariableDeclaration(VariableDeclaration* stmt){
    DataType type = stmt->type;
    string name = stmt->name;
    Symbol* symbol = new VariableSymbol(type,name,scopes.size());
    if(stmt->initializer){
        DataType dt = analyzeExpression(stmt->initializer);
        if(type!=dt){
            throw runtime_error(name + " cannot be initialised with different expression type");
        }
    }
    if(!currentscope().insert(symbol))throw runtime_error(std::string("Variable ") + symbol->name + " is already declared");
}

DataType SemanticAnalyzer :: analyzeNumberExpression(NumberExpr* expr){
    expr->type = DataType::Int;
    return DataType::Int;
}

DataType SemanticAnalyzer :: analyzeCharExpression(CharExpr* expr){
    expr->type = DataType::Char;
    return DataType::Char;
}

DataType SemanticAnalyzer :: analyzeBoolExpression(BoolExpr* expr){
    expr->type = DataType::Bool;
    return DataType:: Bool;
}

DataType SemanticAnalyzer :: analyzeBinaryExpression(BinaryExpr* expr){
    DataType lhs = analyzeExpression(expr->left);
    DataType rhs = analyzeExpression(expr->right);
    expr->left->type = lhs;
    expr->right->type = rhs;
    if(lhs!=rhs){
        throw runtime_error(expr->op.lexeme + " cannot operate on different expresionype");
    }
    switch(expr->op.type){
        case TokenType::Plus:
        case TokenType::Subtract:
        case TokenType::Mutliplication:
        case TokenType::Division:
        case TokenType::Modulo:
        case TokenType::Bitwise_And:
        case TokenType::Bitwise_Or:
        case TokenType::Left_Shift:
        case TokenType::Right_Shift:
        case TokenType::Xor:
             if (lhs != DataType::Int)
             throw runtime_error("Arithmetic operators require int operands");
             return DataType::Int;
        case TokenType::Less:
        case TokenType::Less_Equal:
        case TokenType::Greater:
        case TokenType::Greater_Equal:
        case TokenType::Equal_Equal:
        case TokenType::Not_Equal:
        case TokenType::Logical_And:
        case TokenType::Logical_Or:
             return DataType::Bool;
        default:
             throw runtime_error("Unknown binary operator");
    }
}

DataType SemanticAnalyzer :: analyzeAssignExpression(AssignExpr* expr){
    if(!exists(expr->name))throw runtime_error(std::string("Variable ") + expr->name + " is not declared");
    auto variable = dynamic_cast<VariableSymbol*>(lookup(expr->name));
    if(variable == nullptr){
        throw runtime_error(expr->name + " is not a variable");
    }
    DataType dt = analyzeExpression(expr->value);
    if(dt != variable->type){
        throw runtime_error(std::string("Expression is not assignable due to type mismatch for ")+variable->name);
    }
    return variable->type;
}

DataType SemanticAnalyzer :: analyzeIdentifierExpression(IdentifierExpr* expr){
    if(!exists(expr->name))throw runtime_error(std::string("Variable ") + expr->name + " is not declared");
    Symbol* symbol = lookup(expr->name);
    auto variable = dynamic_cast<VariableSymbol*>(symbol);
    if(variable==nullptr){
        throw runtime_error(expr->name + " is not a variable");
    }
    return variable->type;
}

DataType SemanticAnalyzer :: analyzeFunctionCallExpression(FunctionExpr* expr){
    // check whether function name is defined or not 
    // Need to whether it is a variable or function 
    if(!exists(expr->name))throw runtime_error(std::string("Function ") + expr->name + " is not declared");
    Symbol* symbol = lookup(expr->name);
    auto variable = dynamic_cast<FunctionSymbol*>(symbol);
    if(variable==nullptr){
        throw runtime_error(expr->name + " is not a function");
    }
    if(expr->prms.size() < variable->parameters.size()){
        throw runtime_error("Require more number of arguments");
    }
    else if(expr->prms.size()>variable->parameters.size()){
        throw runtime_error("Too many arguments are passed");
    }
    for(int i = 0;i<variable->parameters.size();i++){
        Expr* pexpr = expr->prms[i];
        DataType dt = analyzeExpression(pexpr);
        expr->prms[i]->type = dt;
        if(dt!=variable->parameters[i].type){
            throw runtime_error(std::string("Parameter type in ")+ variable->name + " and function call are not matching");
        }
    }
    return variable->type;
}

void SemanticAnalyzer :: analyzeFunctionDeclaration(FunctionDeclaration* stmt){
    Symbol* symbol = new FunctionSymbol(stmt->type,stmt->name,stmt->parameters);
    if(!currentscope().insert(symbol))throw runtime_error(std::string("Function ") + stmt->name + " is already declared");
    hasreturn = false;
    currentfunction = dynamic_cast<FunctionSymbol*>(symbol);
    beginScope();
    for(auto parameter : stmt->parameters){
        Symbol* symbol = new VariableSymbol(parameter.type,parameter.name,scopes.size());
        if(!currentscope().insert(symbol)){
            throw runtime_error(std::string("Parameter ") + parameter.name + "is already declared");
        }
    }
    analyzeBlockStatement(stmt->blockstmt,0);
    if(!hasreturn && currentfunction->type!=DataType::Void){
        throw runtime_error(std::string("Function ") + currentfunction->name +" expects return statement");
    }
    currentfunction = nullptr;
    endScope();
}

void SemanticAnalyzer :: analyzeBlockStatement(BlockStmt* stmt,bool createscope){
    if(createscope)beginScope();
    for(auto bstmt : stmt->statements){
        analyzeStatement(bstmt);
    }
    if(createscope)endScope();
}

void SemanticAnalyzer :: analyzeIfStatement(IfStatement* stmt){
    // to add datatype
    DataType dt = analyzeExpression(stmt->condition);
    stmt->condition->type = dt;
    analyzeStatement(stmt->thenBranch);
    if(stmt->elseBranch){
    analyzeStatement(stmt->elseBranch);
    }
}

void SemanticAnalyzer :: analyzeWhileStatement(WhileStatement* stmt){
    // to add datatype
    DataType dt = analyzeExpression(stmt->condition);
    stmt->condition->type = dt;
    analyzeBlockStatement(stmt->blockstmt,1);
}

void SemanticAnalyzer :: analyzeReturnStatement(ReturnStatement* stmt){
    if(currentfunction==nullptr){
        throw runtime_error("There is no function for this return statement");
    }
    if(stmt->value != nullptr){
        DataType dt = analyzeExpression(stmt->value);
        stmt->value->type = dt;
        if(dt != currentfunction->type){
            throw runtime_error("Return type and Function type are not matching");
        }
    }
    if(stmt->value ==nullptr && currentfunction->type != DataType::Void){
         throw runtime_error("Return type and Function type are not matching");
    }
    hasreturn = 1;
}

void SemanticAnalyzer :: analyzeprintstatement(PrintStatement* stmt){
    for(auto& value:stmt->values){
       DataType dt = analyzeExpression(value);
       value->type = dt;
    }
}

void SemanticAnalyzer :: analyzescanstatement(ScanStatement* stmt){
    for(auto &x : stmt->inputs){
        auto id = dynamic_cast<IdentifierExpr*>(x);
        if(id==nullptr)throw runtime_error("Expected a variable which is assignable");
        if(lookup(id->name)==nullptr)throw runtime_error(id->name + ": Variable is not declared");
        else{
            auto s = dynamic_cast<VariableSymbol*>(lookup(id->name));
            x->type = s->type;
        }
    }
}

