#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include "datatype.h"
#include "semantic_analyzer.h"
#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "code_generator.h"

using namespace std;

int main(){
    ifstream file("test.cpp");
    string source{
        istreambuf_iterator<char>(file),
        istreambuf_iterator<char>()
    };
    Lexer lexer(source);
    vector<Token>tokens = lexer.tokenize();

    Parser parser(tokens);
    Program* program = parser.parse();
    AstPrinter* printer = new AstPrinter;
    printer->print(program);
    SemanticAnalyzer semantic;
    semantic.analyze(program);
    CodeGenerator generator;
    generator.generate(program);
    generator.dump();
    generator.dumpToFile("output.ll");

    int status = system("clang output.ll -o output");
    if (status != 0) {
        cerr << "Compilation failed\n";
        return 1;
    }

    cout << "\n===== Program Executing =====\n";
    system("./output");
}