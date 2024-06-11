#pragma once

#include "jackTokenizer.h"
#include "vmWriter.h"
#include "symbolTable.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace tokenizer::tokenType;
using namespace tokenizer::keyWord;

class CompilationEngine {
public:
	CompilationEngine(ofstream& outStream, tokenizer::JackTokenizer& tokenizer, vmWriter::VMWriter& writer);
	void compileClass();
	void compileClassVarDec();
	void compileSubroutine();
	void compileParameterList();
	void compileSubroutineBody();
	void compileVarDec();
	void compileStatements();
	void compileLet();
	void compileIf();
	void compileWhile();
	void compileDo();
	void compileReturn();
	void compileExpression();
	void compileTerm();
	int compileExpressionList();

private:
	ofstream& outStream;
	tokenizer::JackTokenizer& tokenizer;
	vmWriter::VMWriter& writer;
	symbolTable::SymbolTable classSymbols;
	symbolTable::SymbolTable subroutineSymbols;
	string className {};
	string funcName {};
	int label_counter = 0;
	bool is_constructor = false;
	bool is_method = false;

	void write(string text);
	string process_keyword(string token);
	string process_identifier();
	void process_symbol(string token);
	string process_type();
	int process_integerConstant();
	void process_stringConstant();
	void process_variable_assignment(string varName);
	void process_variable_value(string varName);
	vmWriter::Segment kind2segment(symbolTable::symbolKind kind);
};