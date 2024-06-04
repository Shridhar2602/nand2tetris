#pragma once

#include "jackTokenizer.h"
#include <iostream>
#include <fstream>

using namespace std;

class CompilationEngine {
public:
	CompilationEngine(ofstream& outStream, JackTokenizer& tokenizer, bool verbose);
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
	void compileExpressionList();

private:
	bool verbose {false};
	ofstream& outStream;
	JackTokenizer& tokenizer;

	void write(string text);
	void process_keyword(string token);
	void process_identifier();
	void process_symbol(string token);
	void process_type();
	void process_integerConstant();
	void process_stringConstant();
};