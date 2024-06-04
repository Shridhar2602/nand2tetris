#include "compilationEngine.h"

CompilationEngine::CompilationEngine(
	ofstream& fileStream, JackTokenizer& tokenizer, bool verbose
) 
	: outStream(fileStream), // member initializer list - allows us to initialize references before constructor is called
	  tokenizer(tokenizer)
{
	this->verbose = verbose;
}

// 'class' className '{' classVarDec* subroutineDec* '}'
void CompilationEngine::compileClass() {
	
	if(tokenizer.tokenType() != KEYWORD) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected keyword class: Got " << tokenizer.tokenType() << " " << tokenizer.text() << "\n";
		return;
	}

	// write("<tokens>");
	write("<class>");

	process_keyword("class");
	process_identifier();	// class name
	process_symbol("{");

	while(tokenizer.tokenType() != SYMBOL && tokenizer.symbol() != '}') {
		if(tokenizer.tokenType() == KEYWORD && (tokenizer.keyWord() == STATIC || tokenizer.keyWord() == FIELD)) {
			compileClassVarDec();
		}

		compileSubroutine();
	}

	process_symbol("}");

	write("</class>");
	// write("</tokens>");
}

// ('static' | 'field') type varName (',' varName)* ';'
void CompilationEngine::compileClassVarDec() {
	write("<classVarDec>");

	if(tokenizer.keyWord() == STATIC) {
		process_keyword("static");
	}
	else if(tokenizer.keyWord() == FIELD) {
		process_keyword("field");
	}
	else {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected keyword static or field : Got " << tokenizer.stringVal() << "\n";
		return;
	}

	process_type();
	process_identifier();	// variable name
	while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
		process_symbol(",");
		process_identifier();	// variable name
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

		if(keyword == CONSTRUCTOR) {
			process_keyword("constructor");
		}
		else if(keyword == METHOD) {
			process_keyword("method");
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

		process_identifier();	// subrouting name
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
	if(tokenizer.tokenType() == KEYWORD || tokenizer.tokenType() == IDENTIFIER) {
		process_type();
		process_identifier();
	}

	while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
		process_symbol(",");
		process_type();
		process_identifier();
	}
	write("</parameterList>");
}

// '{' varDec* statements '}'
void CompilationEngine::compileSubroutineBody() {

	write("<subroutineBody>");
	process_symbol("{");

	compileVarDec();
	compileStatements();

	process_symbol("}");
	write("</subroutineBody>");
}

// 'var' type varName (',' varName)* ';'
void CompilationEngine::compileVarDec() {
	while(tokenizer.tokenType() == KEYWORD && tokenizer.keyWord() == VAR) {
		write("<varDec>");

		process_keyword("var");
		process_type();
		process_identifier();	// variable name

		while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
			process_symbol(",");
			process_identifier();	// variable name
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
	process_identifier(); 	// varName
	if(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == '[') {
		process_symbol("[");
		compileExpression();
		process_symbol("]");
	}
	process_symbol("=");
	compileExpression();	
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
	process_symbol("{");
	compileStatements();
	process_symbol("}");

	if(tokenizer.tokenType() == KEYWORD && tokenizer.keyWord() == ELSE) {
		process_keyword("else");
		process_symbol("{");
		compileStatements();
		process_symbol("}");
	}

	write("</ifStatement>");
}

// 'while' '('expression')' '{'statements'}'
void CompilationEngine::compileWhile() {
	write("<whileStatement>");

	process_keyword("while");
	process_symbol("(");
	compileExpression();
	process_symbol(")");
	process_symbol("{");
	compileStatements();
	process_symbol("}");

	write("</whileStatement>");
}

// 'do' subroutineCall ';'
void CompilationEngine::compileDo() {
	write("<doStatement>");

	process_keyword("do");

	// implementation of subroutineCall
	// copied from compileTerm as a temporary measure
	process_identifier();
	auto lookahead = tokenizer.tokenType();
	if(lookahead == SYMBOL) {
		auto symbol = tokenizer.symbol();
		if(symbol == '(') {
			process_symbol("(");
			compileExpressionList();
			process_symbol(")");
		}
		else if(symbol == '.') {
			process_symbol(".");
			process_identifier();
			process_symbol("(");
			compileExpressionList();
			process_symbol(")");
		}
	}
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
		return;
	}
	compileExpression();
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
			case '-':
			case '*':
			case '/':
			case '&':
			case '|':
			case '<':
			case '>':
			case '=':
				process_symbol(tokenizer.text());
				compileTerm();
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
			process_integerConstant();
			break;
		case STRING_CONST:
			process_stringConstant();
			break;
		case IDENTIFIER:
		{
			process_identifier();
			auto lookahead = tokenizer.tokenType();
			if(lookahead == SYMBOL) {
				auto symbol = tokenizer.symbol();
				switch(symbol) {
					case '[':
						process_symbol("[");
						compileExpression();
						process_symbol("]");
						break;
					// Next 2 cases are implementation of subroutineCall
					case '(':
						process_symbol("(");
						compileExpressionList();
						process_symbol(")");
						break;
					case '.':
						process_symbol(".");
						process_identifier();
						process_symbol("(");
						compileExpressionList();
						process_symbol(")");
						break;
					// default or non-symbol lookahead implies varName as Term
					default:
						break;
				}
			}
			break;
		}
		case KEYWORD:
		{
			auto keyword = tokenizer.keyWord();
			switch(keyword) {
				case TRUE:
					process_keyword("true");
					break;
				case FALSE:
					process_keyword("false");
					break;
				case _NULL:
					process_keyword("null");
					break;
				case THIS:
					process_keyword("this");
					break; 
				default:
					break;
			}
			break;
		}

		// WRONG REQUIRES LOOKAHEAD
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
					break;
				case '~':
					process_symbol("~");
					compileTerm();
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
void CompilationEngine::compileExpressionList() {
	write("<expressionList>");

	if(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ')') {
		write("</expressionList>");
		return;
	}

	compileExpression();
	while(tokenizer.tokenType() == SYMBOL && tokenizer.symbol() == ',') {
		process_symbol(",");
		compileExpression();
	}

	write("</expressionList>");
}




// ================== HELPER FUNCTIONS =====================

void CompilationEngine::process_keyword(string token) {

	if(tokenizer.tokenType() != KEYWORD || tokenizer.text() != token) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected " << token << " : Got " << tokenizer.text() << "\n";
		return;
	}

	write("<keyword> " + token + " </keyword>");
	tokenizer.advance();
}

void CompilationEngine::process_identifier() {

	if(tokenizer.tokenType() != IDENTIFIER) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected identifier : Got " << tokenizer.text() << "\n";
		return;
	}

	write("<identifier> " + tokenizer.identifier() + " </identifier>");
	tokenizer.advance();
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

void CompilationEngine::process_type() {
	if(tokenizer.tokenType() == KEYWORD) {
		auto keyword = tokenizer.keyWord();
		if(keyword == INT)
			process_keyword("int");
		else if(keyword == BOOLEAN)
			process_keyword("boolean");
		else if(keyword == CHAR)
			process_keyword("char");
	}
	else if(tokenizer.tokenType() == IDENTIFIER) {
		process_identifier();
	}
	else {
		cerr << tokenizer.lineNum()  << ": ERROR: Wrong type : Got " << tokenizer.text() << "\n";
	}
}

void CompilationEngine::process_integerConstant() {
	if(tokenizer.tokenType() != INT_CONST) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected integer constant : Got " << tokenizer.text() << "\n";
		return;
	}

	write("<integerConstant> " + to_string(tokenizer.intVal()) + " </integerConstant>");
	tokenizer.advance();
}

void CompilationEngine::process_stringConstant() {
	if(tokenizer.tokenType() != STRING_CONST) {
		cerr << tokenizer.lineNum()  << ": ERROR: Expected string constant : Got " << tokenizer.text() << "\n";
		return;
	}

	write("<stringConstant> " + tokenizer.stringVal() + " </stringConstant>");
	tokenizer.advance();
}

void CompilationEngine::write(string text) {
	outStream << text << "\n";
}