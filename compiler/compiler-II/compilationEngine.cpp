#include "compilationEngine.h"

CompilationEngine::CompilationEngine(
	ofstream& fileStream, tokenizer::JackTokenizer& tokenizer, vmWriter::VMWriter& writer
) 
	: outStream(fileStream), // member initializer list - allows us to initialize references before constructor is called
	  tokenizer(tokenizer),
	  writer(writer)
{
	classSymbols = symbolTable::SymbolTable();
	subroutineSymbols = symbolTable::SymbolTable();
}

// 'class' className '{' classVarDec* subroutineDec* '}'
void CompilationEngine::compileClass() {
	
	if(tokenizer.tokenType() != KEYWORD) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected keyword class: Got " << tokenizer.tokenType() << " " << tokenizer.text() << "\n";
		return;
	}

	write("<class>");

	classSymbols.reset();
	subroutineSymbols.reset();

	process_keyword("class");
	className = process_identifier();	// class name
	process_symbol("{");

	while(tokenizer.tokenType() != SYMBOL && tokenizer.symbol() != '}') {
		if(tokenizer.tokenType() == KEYWORD && (tokenizer.keyWord() == STATIC || tokenizer.keyWord() == FIELD)) {
			compileClassVarDec();
		}

		compileSubroutine();
	}

	process_symbol("}");

	write("</class>");
}

// ('static' | 'field') type varName (',' varName)* ';'
void CompilationEngine::compileClassVarDec() {
	write("<classVarDec>");
	string kindStr {};

	if(tokenizer.keyWord() == STATIC) {
		kindStr = process_keyword("static");
	}
	else if(tokenizer.keyWord() == FIELD) {
		kindStr = process_keyword("field");
	}
	else {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected keyword static or field : Got " << tokenizer.stringVal() << "\n";
		return;
	}

	auto kind = (kindStr == "static" ? symbolTable::STATIC : symbolTable::FIELD);
	string type = process_type();
	string name = process_identifier();	// variable name
	classSymbols.define(name, type, kind);

	while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
		process_symbol(",");
		name = process_identifier();
		classSymbols.define(name, type, kind);
	}
	process_symbol(";");

	write("</classVarDec>");
}

// ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
void CompilationEngine::compileSubroutine() {
	if(tokenizer.tokenType() != KEYWORD) 
		return;

	auto keyword = tokenizer.keyWord();
	if(keyword == CONSTRUCTOR || keyword == METHOD || keyword == FUNCTION) {
		write("<subroutineDec>");

		subroutineSymbols.reset();

		if(keyword == CONSTRUCTOR) {
			process_keyword("constructor");
			is_constructor = true;
		}
		else if(keyword == METHOD) {
			process_keyword("method");
			subroutineSymbols.define("this", className, symbolTable::ARG);
			is_method = true;
		}
		else {
			process_keyword("function");
		}

		// return type
		if(tokenizer.tokenType() == KEYWORD && tokenizer.keyWord() == VOID) {
			process_keyword("void");
		}
		else {
			process_type();
		}

		funcName = process_identifier();	// subrouting name
		process_symbol("(");
		compileParameterList();
		process_symbol(")");

		compileSubroutineBody();

		write("</subroutineDec>");
	}
}

// (type varName (',' type varName)*)?
void CompilationEngine::compileParameterList() {

	write("<parameterList>");
	string type {}, name {};
	if(tokenizer.tokenType() == KEYWORD || tokenizer.tokenType() == IDENTIFIER) {
		type = process_type();
		name = process_identifier();
		subroutineSymbols.define(name, type, symbolTable::ARG);
	}

	while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
		process_symbol(",");
		type = process_type();
		name = process_identifier();
		subroutineSymbols.define(name, type, symbolTable::ARG);
	}
	write("</parameterList>");
}

// '{' varDec* statements '}'
void CompilationEngine::compileSubroutineBody() {

	write("<subroutineBody>");
	process_symbol("{");

	compileVarDec();
	writer.writeFunction(className + "." + funcName, subroutineSymbols.varCount(symbolTable::VAR));

	if(is_constructor) {
		// allocates memory of size number of fields on heap, and aligns pointer 0 to the base address of this object
		writer.writePush(vmWriter::CONST, classSymbols.varCount(symbolTable::FIELD));
		writer.writeCall("Memory.alloc", 1);
		writer.writePop(vmWriter::POINTER, 0);
		is_constructor = false;
	}

	if(is_method) {
		// aligns virtual memory segment this with the base address of the object on which the method is called
		writer.writePush(vmWriter::ARG, 0);
		writer.writePop(vmWriter::POINTER, 0);
		is_method = false;
	}

	compileStatements();

	process_symbol("}");
	write("</subroutineBody>");
}

// 'var' type varName (',' varName)* ';'
void CompilationEngine::compileVarDec() {
	while(tokenizer.tokenType() == KEYWORD && tokenizer.keyWord() == VAR) {
		write("<varDec>");

		process_keyword("var");
		string type = process_type();
		string name = process_identifier();	// variable name
		subroutineSymbols.define(name, type, symbolTable::VAR);

		while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
			process_symbol(",");
			name = process_identifier();	// variable name
			subroutineSymbols.define(name, type, symbolTable::VAR);
		}

		process_symbol(";");
		write("</varDec>");
	}
}

// letStatement | ifStatement | whileStatement | doStatement | returnStatement
void CompilationEngine::compileStatements() {

	write("<statements>");

	while(tokenizer.tokenType() == KEYWORD) {
		auto keyword = tokenizer.keyWord();
		if(keyword == LET || keyword == IF || keyword == WHILE || keyword == DO || keyword == RETURN) {
			switch(keyword) {
				case LET:
					compileLet();
					break;
				case IF:
					compileIf();
					break;
				case WHILE:
					compileWhile();
					break;
				case DO:
					compileDo();
					break;
				case RETURN:
					compileReturn();
					break;
				default:
					break;
			}

		}
		else {
			break;
		}
	}

	write("</statements>");
}

// 'let' varName ('['expression']')? '=' expression
void CompilationEngine::compileLet() {
	write("<letStatement>");
	
	process_keyword("let");
	string varName = process_identifier();

	// let varName[expression1] = expression2
	if(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '[') {
		process_symbol("[");

		// get varName[expression1]
		process_variable_value(varName);
		compileExpression();
		writer.writeArithmetic(vmWriter::ADD);

		process_symbol("]");

		process_symbol("=");
		// get expression2 and store it in temp
		compileExpression();
		writer.writePop(vmWriter::TEMP, 0);

		// get varName[expression1] and assign expression2 to it
		writer.writePop(vmWriter::POINTER, 1);
		writer.writePush(vmWriter::TEMP, 0);
		writer.writePop(vmWriter::THAT, 0);
	}
	// let varName = expression
	else {
		process_symbol("=");
		compileExpression();

		process_variable_assignment(varName);
	}

	process_symbol(";");
	write("</letStatement>");
}

// 'if' '('expression')' '{'statements'}' ('else' '{'statements'}')?
void CompilationEngine::compileIf() {
	write("<ifStatement>");

	process_keyword("if");
	process_symbol("(");
	compileExpression();
	process_symbol(")");

	int counter1 = label_counter++;
	int counter2 = label_counter++;

	writer.writeArithmetic(vmWriter::NOT);	
	writer.writeIf("IF_" + className + "_" + funcName + "_" + to_string(counter1));
	process_symbol("{");
	compileStatements();
	process_symbol("}");
	writer.writeGoto("IF_END_" + className + "_" + funcName + "_" + to_string(counter2));
	writer.writeLabel("IF_" + className + "_" + funcName + "_" + to_string(counter1));

	if(tokenizer.tokenType() == KEYWORD && tokenizer.keyWord() == ELSE) {
		process_keyword("else");
		process_symbol("{");
		compileStatements();
		process_symbol("}");
	}
	writer.writeLabel("IF_END_" + className + "_" + funcName + "_" + to_string(counter2));

	write("</ifStatement>");
}

// 'while' '('expression')' '{'statements'}'
void CompilationEngine::compileWhile() {
	write("<whileStatement>");

	int counter1 = label_counter++;
	int counter2 = label_counter++;

	process_keyword("while");
	writer.writeLabel("WHILE_EXP_START_" + className + "_" + funcName + "_" + to_string(counter1));
	process_symbol("(");
	compileExpression();
	process_symbol(")");

	writer.writeArithmetic(vmWriter::NOT);
	writer.writeIf("WHILE_EXP_END_" + className + "_" + funcName + "_" + to_string(counter2));

	process_symbol("{");
	compileStatements();
	process_symbol("}");
	writer.writeGoto("WHILE_EXP_START_" + className + "_" + funcName + "_" + to_string(counter1));
	writer.writeLabel("WHILE_EXP_END_" + className + "_" + funcName + "_" + to_string(counter2));

	write("</whileStatement>");
}

// 'do' subroutineCall ';'
void CompilationEngine::compileDo() {
	write("<doStatement>");

	process_keyword("do");

	compileTerm();
	writer.writePop(vmWriter::TEMP, 0);
	process_symbol(";");

	write("</doStatement>");
}

// 'return' expression? ';'
void CompilationEngine::compileReturn() {

	write("<returnStatement>");

	process_keyword("return");
	if(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ';') {
		process_symbol(";");
		write("</returnStatement>");
		writer.writePush(vmWriter::CONST, 0);
		writer.writeReturn();
		return;
	}
	compileExpression();
	writer.writeReturn();
	process_symbol(";");

	write("</returnStatement>");
}

// term (op term)* 
void CompilationEngine::compileExpression() {
	write("<expression>");

	compileTerm();

	bool flag = false;
	while(tokenizer.tokenType() == SYMBOL) {
		
		auto symbol = tokenizer.symbol();
		switch(symbol) {
			case '+':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::ADD);
				break;
			case '-':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::SUB);
				break;
			case '*':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeCall("Math.multiply", 2);
				break;
			case '/':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeCall("Math.divide", 2);
				break;
			case '&':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::AND);
				break;
			case '|':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::OR);
				break;
			case '<':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::LT);
				break;
			case '>':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::GT);
				break;
			case '=':
				process_symbol(tokenizer.text());
				compileTerm();
				writer.writeArithmetic(vmWriter::EQ);
				break;
			default:
				flag = true;
		}

		if(flag)
			break;
	}

	write("</expression>");
}

// integerConstant | stringConstant | keywordConstant | varName |
// varName '['expression']' | '('expression')' | (unaryOp term) | subroutineCall
void CompilationEngine::compileTerm() {
	
	write("<term>");

	auto tokenType = tokenizer.tokenType();
	switch(tokenType) {
		case INT_CONST:
			writer.writePush(vmWriter::CONST, process_integerConstant());
			// process_integerConstant();
			break;
		case STRING_CONST:
			process_stringConstant();
			break;
		case IDENTIFIER:
		{
			auto identifier = process_identifier();
			auto lookahead = tokenizer.tokenType();
			if(lookahead == SYMBOL) {
				auto symbol = tokenizer.symbol();
				switch(symbol) {
					case '[':	// array access
						process_symbol("[");

						process_variable_value(identifier);
						compileExpression();
						writer.writeArithmetic(vmWriter::ADD);
						writer.writePop(vmWriter::POINTER, 1);
						writer.writePush(vmWriter::THAT, 0);
						process_symbol("]");
						return;
						break;
					// Next 2 cases are implementation of subroutineCall
					case '(': {
						process_symbol("(");
						// all calls without class name are method calls. Push this to stack as the first argument
						// writer.writePush(vmWriter::THIS, subroutineSymbols.indexOf("this"));
						writer.writePush(vmWriter::POINTER, 0);
						int num_arguments = compileExpressionList();
						identifier = className + "." + identifier;
						writer.writeCall(identifier, num_arguments + 1);
						process_symbol(")");
						return;
						break;
					}
					case '.': {
						process_symbol(".");
						int num_arguments = 0;

						// if a method call, push object to stack as the first argument
						// if variable does not exist it is a function call
						if(subroutineSymbols.indexOf(identifier) != -1) {
							writer.writePush(kind2segment(subroutineSymbols.kindOf(identifier)), subroutineSymbols.indexOf(identifier));
							num_arguments = 1;
							identifier = subroutineSymbols.typeOf(identifier);
						}
						else if(classSymbols.indexOf(identifier) != -1) {
							writer.writePush(kind2segment(classSymbols.kindOf(identifier)), classSymbols.indexOf(identifier));
							num_arguments = 1;
							identifier = classSymbols.typeOf(identifier);
						}

						identifier += "." + process_identifier();
						process_symbol("(");
						num_arguments += compileExpressionList();
						writer.writeCall(identifier, num_arguments);
						process_symbol(")");
						return;
						break;
					}
					// default or non-symbol lookahead implies varName as Term
					default:
						break;
				}
			}
			process_variable_value(identifier);
			break;
		}
		case KEYWORD:
		{
			auto keyword = tokenizer.keyWord();
			switch(keyword) {
				case TRUE:
					process_keyword("true");
					writer.writePush(vmWriter::CONST, 1);
					writer.writeArithmetic(vmWriter::NEG);
					break;
				case FALSE:
					process_keyword("false");
					writer.writePush(vmWriter::CONST, 0);
					break;
				case _NULL:
					process_keyword("null");
					writer.writePush(vmWriter::CONST, 0);
					break;
				case THIS:
					process_keyword("this");
					writer.writePush(vmWriter::POINTER, 0);		// pushes base address of current object to stack
					break; 
				default:
					break;
			}
			break;
		}
 
		case SYMBOL:
			auto symbol = tokenizer.symbol();
			switch(symbol) {
				case '(':
					process_symbol("(");
					compileExpression();
					process_symbol(")");
					break;
				case '-':
					process_symbol("-");
					compileTerm();
					writer.writeArithmetic(vmWriter::NEG);
					break;
				case '~':
					process_symbol("~");
					compileTerm();
					writer.writeArithmetic(vmWriter::NOT);
					break;
				default:
					cerr << tokenizer.lineNum()  << ": ERROR: Expected term symbol : Got " << tokenizer.text() << "\n";
					break;
			}
			break;
	}

	write("</term>");
}

// (expression (',' expression)*)?
int CompilationEngine::compileExpressionList() {
	write("<expressionList>");

	if(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ')') {
		write("</expressionList>");
		return 0;
	}

	int num_expressions = 1;

	compileExpression();
	while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
		process_symbol(",");
		compileExpression();
		num_expressions++;
	}

	write("</expressionList>");
	return num_expressions;
}




// ================== HELPER FUNCTIONS =====================

string CompilationEngine::process_keyword(string token) {

	if(tokenizer.tokenType() != KEYWORD || tokenizer.text() != token) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected " << token << " : Got " << tokenizer.text() << "\n";
		return "invalidKeyword";
	}

	write("<keyword> " + token + " </keyword>");
	tokenizer.advance();
	return token;
}

string CompilationEngine::process_identifier() {

	if(tokenizer.tokenType() != IDENTIFIER) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected identifier : Got " << tokenizer.text() << "\n";
		return "invalidIdentifier";
	}

	auto identifier = tokenizer.identifier();
	write("<identifier> " + identifier + " </identifier>");
	tokenizer.advance();
	return identifier;
}

void CompilationEngine::process_symbol(string token) {
	if(tokenizer.tokenType() != SYMBOL || tokenizer.text() != token) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected " << token << " : Got " << tokenizer.text() << "\n";
		return;
	}

	auto symbol = tokenizer.symbol();
	switch(symbol) {
		case '<':
			token = "&lt;";
			break;
		case '>':
			token = "&gt;";
			break;
		case '"':
			token = "&quot;";
			break;
		case '&':
			token = "&amp;";
			break;
	}
	
	write("<symbol> " + token + " </symbol>");
	tokenizer.advance();
}

string CompilationEngine::process_type() {
	if(tokenizer.tokenType() == KEYWORD) {
		auto keyword = tokenizer.keyWord();
		if(keyword == INT)
			return process_keyword("int");
		else if(keyword == BOOLEAN)
			return process_keyword("boolean");
		else if(keyword == CHAR)
			return process_keyword("char");
	}
	else if(tokenizer.tokenType() == IDENTIFIER) {
		return process_identifier();
	}
	
	cerr << tokenizer.lineNum()  << ": ERROR: Wrong type : Got " << tokenizer.text() << "\n";
	return "invalidType";
}

int CompilationEngine::process_integerConstant() {
	if(tokenizer.tokenType() != INT_CONST) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected integer constant : Got " << tokenizer.text() << "\n";
		return -1;
	}

	int val = tokenizer.intVal();
	write("<integerConstant> " + to_string(val) + " </integerConstant>");
	tokenizer.advance();
	return val;
}

void CompilationEngine::process_stringConstant() {
	if(tokenizer.tokenType() != STRING_CONST) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected string constant : Got " << tokenizer.text() << "\n";
		return;
	}

	string s = tokenizer.stringVal();
	writer.writePush(vmWriter::CONST, s.length());
	writer.writeCall("String.new", 1);
	for(auto c : s) {
		writer.writePush(vmWriter::CONST, c);
		writer.writeCall("String.appendChar", 2);
	}

	write("<stringConstant> " + s + " </stringConstant>");
	tokenizer.advance();
}

void CompilationEngine::process_variable_assignment(string varName) {
	if(subroutineSymbols.indexOf(varName) != -1) {
		writer.writePop(
			kind2segment(subroutineSymbols.kindOf(varName)),
			subroutineSymbols.indexOf(varName)
		);
	}
	else if(classSymbols.indexOf(varName) != -1) {
		writer.writePop(
			kind2segment(classSymbols.kindOf(varName)),
			classSymbols.indexOf(varName)
		);
	}
	else {
		cerr << tokenizer.lineNum() << "ERROR (assignment): Variable " << varName << " not found\n";
	}
}

void CompilationEngine::process_variable_value(string varName) {
	if(subroutineSymbols.indexOf(varName) != -1) {
		writer.writePush(
			kind2segment(subroutineSymbols.kindOf(varName)),
			subroutineSymbols.indexOf(varName)
		);
	}
	else if(classSymbols.indexOf(varName) != -1) {
		writer.writePush(
			kind2segment(classSymbols.kindOf(varName)),
			classSymbols.indexOf(varName)
		);
	}
	else {
		cerr << tokenizer.lineNum() << "ERROR (value): Variable " << varName << " not found\n";
	}
}

vmWriter::Segment CompilationEngine::kind2segment(symbolTable::symbolKind kind) {
	switch(kind) {
		case symbolTable::STATIC:
			return vmWriter::STATIC;
		case symbolTable::FIELD:
			return vmWriter::THIS;
		case symbolTable::ARG:
			return vmWriter::ARG;
		case symbolTable::VAR:
			return vmWriter::LOCAL;
		default:
			cerr << "ERROR: kind2segment: Invalid kind\n";
			return vmWriter::NONE;
	}
}

void CompilationEngine::write(string text) {
	outStream << text << "\n";
}