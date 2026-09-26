#pragma once
#include <iostream>
#include <compiler/parser_dir/parser.h>
#include <compiler/semantic_analyzer/semanticanalyzer.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
using namespace std;

struct VariableInfo{
    DataType type;
    llvm::Value* address;
};

class CodeGenerator{

    private:
    llvm::LLVMContext context;
    unique_ptr<llvm::Module>module;
    llvm::IRBuilder<>builder;
    vector<unordered_map<string,VariableInfo> >scopes;
    llvm::Function* currentFunction = nullptr;
    llvm::FunctionCallee ScanfFunction = 
                       module->getOrInsertFunction(
                           "scanf",
                         llvm::FunctionType::get(
                             llvm::Type::getInt32Ty(context),
                             llvm::PointerType::getUnqual(context),
                             true
                         )
                       );

    public:

       CodeGenerator();
       unordered_map<string,VariableInfo>& currentscope();
       void endscope();
       void beginscope();
       void generate(Program*program);
       VariableInfo& lookup(string name);
       void generateStatement(Stmt*);
       void generateVariableStatement(VariableDeclaration*);
       void generateFunctionStatement(FunctionDeclaration*);
       void generateBlockStatement(BlockStmt*,bool);
       void generatePrintStatement(PrintStatement*);
       void generateReturnStatement(ReturnStatement*);
       void generateExpressionStatement(ExpressionStmt*);
       void generateIfStatement(IfStatement*);
       void generateWhileStatement(WhileStatement*);
       void generateScanStatement(ScanStatement*);
       llvm::Value* generateExpression(Expr*);
       llvm::Value* generateNumberExpression(NumberExpr*);
       llvm::Value* generateBinaryExpression(BinaryExpr*);
       llvm::Value* generateIdentifierExpression(IdentifierExpr*);
       llvm::Value* generateAssignExpression(AssignExpr*);
       llvm::Value* generateFunctionExpression(FunctionExpr*);
       llvm::Value* generateAndBinaryExpression(BinaryExpr*);
       llvm::Value* generateOrBinaryExpression(BinaryExpr*);
       llvm::Value* generatePrintFunctionExpression(FunctionExpr*);
       void generatePrintf(llvm::Value* value, DataType type);
       void dumpToFile(const std::string& filename);
       // helper
       llvm::Type* getLLVMType(DataType type);
       void dump();
};

CodeGenerator::CodeGenerator()
: module(make_unique<llvm::Module>("Compiler",context)),
  builder(context)
  {}

void CodeGenerator::generate(Program*program){
    for(auto stmt:program->statements){
        generateStatement(stmt);
    }
}

void CodeGenerator::beginscope(){
    scopes.push_back(unordered_map<string,VariableInfo>());
}

unordered_map<string,VariableInfo>& CodeGenerator:: currentscope(){
      return scopes.back();
}

void CodeGenerator::endscope(){
     scopes.pop_back();

}

VariableInfo& CodeGenerator:: lookup(string name){
     for(int i = scopes.size()-1;i>=0;i--){
          if(scopes[i].find(name)!=scopes[i].end()){
             return scopes[i][name];
          }
     }
     throw runtime_error("Something Unusual has happened");
}

llvm::Type* CodeGenerator::getLLVMType(DataType type){
    if(type == DataType::Bool){
        return llvm::Type::getInt1Ty(context);
    }
    else if(type == DataType::Char){
        return llvm::Type::getInt8Ty(context);
    }
    else if(type == DataType::Int){
        return llvm::Type::getInt32Ty(context);
    }
    else if(type == DataType::Void){
        return llvm::Type::getVoidTy(context);
    }
    throw runtime_error("Unknown Datatype");
}

void CodeGenerator::generateStatement(Stmt* stmt){
    if(auto x = dynamic_cast<VariableDeclaration*>(stmt)){
       return generateVariableStatement(x);
    }
    else if(auto x = dynamic_cast<FunctionDeclaration*>(stmt)){
       return  generateFunctionStatement(x);
    }
    else if(auto x = dynamic_cast<BlockStmt*>(stmt)){
       return  generateBlockStatement(x,true);
    }
    else if(auto x = dynamic_cast<PrintStatement*>(stmt)){
       return  generatePrintStatement(x);
    }
    else if(auto x = dynamic_cast<ReturnStatement*>(stmt)){
       return  generateReturnStatement(x);
    }
    else if(auto x = dynamic_cast<ExpressionStmt*>(stmt)){
       return generateExpressionStatement(x);
    }
    else if(auto x = dynamic_cast<IfStatement*>(stmt)){
       return generateIfStatement(x);
    }
    else if(auto x = dynamic_cast<WhileStatement*>(stmt)){
       return generateWhileStatement(x);
    }
    else if(auto x = dynamic_cast<ScanStatement*>(stmt)){
        return generateScanStatement(x);
    }
    throw runtime_error("CodeGeneration Unknown Statement");
}

void CodeGenerator:: generateVariableStatement(VariableDeclaration*stmt){
    llvm:: Value* address = builder.CreateAlloca(
        getLLVMType(stmt->type),
        nullptr,
        stmt->name
    );
    currentscope()[stmt->name] = {stmt->type,address};
    if(stmt->initializer==nullptr) return;
    llvm:: Value* ival = generateExpression(stmt->initializer);
    builder.CreateStore(
        ival,
        address
    );
}

void CodeGenerator :: generateFunctionStatement(FunctionDeclaration*stmt){
    vector<llvm::Type*>params;
    for(auto &prm: stmt->parameters){
        params.push_back(getLLVMType(prm.type));
    }
    llvm::FunctionType* functiontype =
            llvm::FunctionType::get(
                getLLVMType(stmt->type),
                params,
                false
            );
    llvm::Function* function =  llvm::Function::Create(
        functiontype,
        llvm::Function::ExternalLinkage,
        stmt->name,
        module.get()
    );
    auto argit = function->arg_begin();
    llvm::BasicBlock* entry = 
       llvm:: BasicBlock::Create(
           context,
           "entry",
            function
    );
    builder.SetInsertPoint(entry);
    beginscope();
    for(auto &prm:stmt->parameters){
        llvm::Argument &arg = *argit;
        arg.setName(prm.name);
        llvm::Value* address = builder.CreateAlloca(
                                    getLLVMType(prm.type),
                                    nullptr,
                                    prm.name
                               );
        builder.CreateStore(&arg,address);
        currentscope()[prm.name] = {prm.type,address};
        ++argit;
    }
    currentFunction = function;
    generateBlockStatement(stmt->blockstmt,false);
    currentFunction = nullptr;
    endscope();
}

void CodeGenerator:: generateBlockStatement(BlockStmt*stmt,bool scopegenerated){
    if(scopegenerated)beginscope();
    for(auto bstmt: stmt->statements){
        generateStatement(bstmt);
    }
    if(scopegenerated)endscope();
}


void CodeGenerator:: generateReturnStatement(ReturnStatement* stmt){
    if(stmt->value == nullptr){
        builder.CreateRetVoid();
    }
    else{
        llvm::Value*value  =  generateExpression(stmt->value);
        builder.CreateRet(value);
    }
}

void CodeGenerator::generateExpressionStatement(ExpressionStmt* stmt){
    generateExpression(stmt->expression);
}

void CodeGenerator::generateIfStatement(IfStatement*stmt){
    llvm::BasicBlock* thenblock = 
    llvm::BasicBlock::Create(
        context,
        "then",
        currentFunction
    );
    llvm::BasicBlock* elseblock = 
    llvm::BasicBlock::Create(
        context,
        "else",
        currentFunction
    );
    llvm::BasicBlock* mergeblock =
    llvm::BasicBlock::Create(
        context,
        "merge",
        currentFunction
    );
    llvm::Value* value = generateExpression(stmt->condition);
    builder.CreateCondBr(
        value,
        thenblock,
        elseblock
    );
    builder.SetInsertPoint(thenblock);
    generateStatement(stmt->thenBranch);
    if (!builder.GetInsertBlock()->getTerminator()) builder.CreateBr(mergeblock);
    builder.SetInsertPoint(elseblock);
    if(stmt->elseBranch!=nullptr){
        generateStatement(stmt->elseBranch);
    }
    if (!builder.GetInsertBlock()->getTerminator()){
        builder.CreateBr(mergeblock);
        builder.SetInsertPoint(mergeblock);
    }
   
}

void CodeGenerator :: generateWhileStatement(WhileStatement*stmt){
    llvm::BasicBlock* conditionblock = 
    llvm::BasicBlock::Create(
        context,
        "condition",
        currentFunction
    );
    llvm::BasicBlock* loopblock = 
    llvm::BasicBlock::Create(
        context,
        "loop",
        currentFunction
    );
    llvm::BasicBlock* mergeblock = 
    llvm::BasicBlock::Create(
        context,
        "merge",
        currentFunction
    );
    builder.CreateBr(conditionblock);
    builder.SetInsertPoint(conditionblock);
    llvm::Value*cond = generateExpression(stmt->condition);
    builder.CreateCondBr(
        cond,
        loopblock,
        mergeblock
    );
    builder.SetInsertPoint(loopblock);
    generateStatement(stmt->blockstmt);
    builder.CreateBr(conditionblock);
    builder.SetInsertPoint(mergeblock);
}

llvm::Value* CodeGenerator :: generateExpression(Expr* expr){
    if(auto x = dynamic_cast<NumberExpr*>(expr)){
        return generateNumberExpression(x);
    }
    else if(auto x = dynamic_cast<BinaryExpr*>(expr)){
        return generateBinaryExpression(x);
    }
    else if(auto x = dynamic_cast<IdentifierExpr*>(expr)){
        return generateIdentifierExpression(x);
    }
    else if(auto x = dynamic_cast<AssignExpr*>(expr)){
        return generateAssignExpression(x);
    }
    else if(auto x = dynamic_cast<FunctionExpr*>(expr)){
        return generateFunctionExpression(x);
    }
    throw runtime_error("Unknown expression");
}

llvm::Value* CodeGenerator::generateAssignExpression(AssignExpr* expr){
    llvm::Value* address = lookup(expr->name).address;
    llvm::Value* value = generateExpression(expr->value);
    builder.CreateStore(value,address);
    return value;
}

llvm::Value* CodeGenerator:: generateNumberExpression(NumberExpr* expr){
    llvm::Value* value = llvm::ConstantInt::get(
        llvm::Type::getInt32Ty(context),
        expr->value                        
    );
    return value;
}

llvm::Value* CodeGenerator:: generateIdentifierExpression(IdentifierExpr*expr){
    llvm::Value* value = builder.CreateLoad(
        getLLVMType(lookup(expr->name).type),
        lookup(expr->name).address
    );
    return value;
}

llvm::Value* CodeGenerator:: generateBinaryExpression(BinaryExpr* expr){
    llvm::Value* left = generateExpression(expr->left);
    llvm::Value* right = generateExpression(expr->right);
    switch(expr->op.type){
        case TokenType::Plus:
        return builder.CreateAdd(left,right,"addtmp");
        case TokenType::Subtract:
        return builder.CreateSub(left,right,"minustmp");
        case TokenType::Mutliplication:
        return builder.CreateMul(left,right,"multmp");
        case TokenType::Division:
        // for signed integers
        return builder.CreateSDiv(left,right,"divtmp");
        case TokenType::Modulo:
        return builder.CreateSRem(left,right,"modtmp");
        case TokenType::Less:
        return builder.CreateICmpSLT(left,right,"ltmp");
        case TokenType::Less_Equal:
        return builder.CreateICmpSLE(left,right,"letmp");
        case TokenType::Greater:
        return builder.CreateICmpSGT(left,right,"gtmp");
        case TokenType::Greater_Equal:
        return builder.CreateICmpSGE(left,right,"getmp");
        case TokenType::Equal_Equal:
        return builder.CreateICmpEQ(left,right,"etmp");
        case TokenType::Not_Equal:
        return builder.CreateICmpNE(left,right,"netmp");
        case TokenType::Logical_And:
        return generateAndBinaryExpression(expr);
        case TokenType::Logical_Or:
        return generateOrBinaryExpression(expr);
        default:
        throw runtime_error("Unsupported operation");
    }
}

llvm::Value* CodeGenerator::generateAndBinaryExpression(BinaryExpr* expr){
    llvm::Value* lhs = generateExpression(expr->left);
    llvm::BasicBlock* rhsblock = llvm::BasicBlock::Create(
        context,
        "rhs",
        currentFunction
    );
    llvm::BasicBlock* falseblock = llvm::BasicBlock::Create(
        context,
        "false",
        currentFunction
    );
    llvm::BasicBlock* mergeblock = llvm::BasicBlock::Create(
        context,
        "merge",
        currentFunction
    );
    builder.CreateCondBr(
        lhs,
        rhsblock,
        falseblock
    );
    builder.SetInsertPoint(rhsblock);
    llvm::Value* rhs = generateExpression(expr->right);
    builder.CreateCondBr(
        rhs,
        mergeblock,
        falseblock
    );
    builder.SetInsertPoint(falseblock);
    builder.CreateBr(mergeblock);
    builder.SetInsertPoint(mergeblock);
    llvm::PHINode* phi =
    builder.CreatePHI(
        llvm::Type::getInt1Ty(context),
        2,
        "landtmp"
    );
    phi->addIncoming(
        llvm::ConstantInt::getFalse(context),
        falseblock
    );
    
    phi->addIncoming(
        llvm::ConstantInt::getTrue(context),
        rhsblock
    );
    return phi;
}

llvm::Value* CodeGenerator::generateOrBinaryExpression(BinaryExpr* expr){
    llvm::Value* lhs = generateExpression(expr->left);
    llvm::BasicBlock* rhsblock = llvm::BasicBlock::Create(
        context,
        "rhs",
        currentFunction
    );
    llvm::BasicBlock* trueblock = llvm::BasicBlock::Create(
        context,
        "true",
        currentFunction
    );
    llvm::BasicBlock* mergeblock = llvm::BasicBlock::Create(
        context,
        "merge",
        currentFunction
    );
    builder.CreateCondBr(
        lhs,
        trueblock,
        rhsblock
    );
    builder.SetInsertPoint(rhsblock);
    llvm::Value* rhs = generateExpression(expr->right);
    builder.CreateCondBr(
        rhs,
        trueblock,
        mergeblock
    );
    builder.SetInsertPoint(trueblock);
    builder.CreateBr(mergeblock);
    builder.SetInsertPoint(mergeblock);
    llvm::PHINode* phi = builder.CreatePHI(
        llvm::Type::getInt1Ty(context),
        2,
        "lortmp"
    );
    phi->addIncoming(
        llvm::ConstantInt::getTrue(context),
        trueblock
    );
    phi->addIncoming(
        llvm::ConstantInt::getFalse(context),
        rhsblock
    );
    return phi;
}

void CodeGenerator::generatePrintf(llvm::Value* value ,DataType type){
    llvm::FunctionCallee printfFunc = 
    module->getOrInsertFunction(
        "printf",
        llvm::FunctionType::get(
            llvm::Type::getInt32Ty(context),
            llvm::PointerType::getUnqual(context),
            true
        )
    );
    llvm::Value* format = nullptr;
    switch(type){
        case DataType::Int:
        format = builder.CreateGlobalStringPtr("%d");
        break;
        case DataType::Char:
        format = builder.CreateGlobalStringPtr("%c");
        break;
        case DataType::Bool:
        format = builder.CreateGlobalStringPtr("%d");
        break;
    }
    builder.CreateCall(printfFunc,{format,value});
}


void CodeGenerator:: generatePrintStatement(PrintStatement*stmt){
    for(auto& x:stmt->values){
        llvm::Value* value = generateExpression(x);
        generatePrintf(value,x->type);
    }    
}

void CodeGenerator:: generateScanStatement(ScanStatement* stmt){
    for(auto &x : stmt->inputs){
        auto id = dynamic_cast<IdentifierExpr*>(x);
        llvm::Value* format = nullptr;
        VariableInfo info = lookup(id->name);
        switch(info.type){
            case DataType::Int:
            format = builder.CreateGlobalStringPtr("%d");
            break;
            case DataType::Char:
            format = builder.CreateGlobalStringPtr("%c");
            break;
            case DataType::Bool:
            format = builder.CreateGlobalStringPtr("%d");
            break;
        }
        builder.CreateCall(ScanfFunction,{format,info.address});
    }
}

llvm::Value* CodeGenerator::generateFunctionExpression(FunctionExpr*expr){
    llvm::Function*function = module->getFunction(expr->name);
    vector<llvm::Value*>args;
    for(auto pexpr:expr->prms){
        args.push_back(generateExpression(pexpr));
    }
    llvm::Value*call = builder.CreateCall(function, args,"calltmp");
    return call;
}
// printing 

void CodeGenerator :: dump(){
    module->print(llvm::outs(),nullptr);
}

void CodeGenerator::dumpToFile(const std::string& filename) {
    std::error_code EC;
    llvm::raw_fd_ostream out(filename, EC);

    if (EC)
        throw runtime_error("Cannot open output file.");

    module->print(out, nullptr);
}
