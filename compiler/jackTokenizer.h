#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

enum tokenType {
	BLANK,
	KEYWORD,
	SYMBOL,
	IDENTIFIER,
	INT_CONST,
	STRING_CONST,
	COMMENT,
	MULTILINE_COMMENT,
	COMMENT_END,
};

const string debugEnum[] = {
	"BLANK",
	"KEYWORD",
	"SYMBOL",
	"IDENTIFIER",
	"INT_CONST",
	"STRING_CONST",
	"COMMENT",
	"MULTILINE_COMMENT",
	"COMMENT_END",
};

enum keyWord {
	CLASS,
	METHOD,
	FUNCTION,
	CONSTRUCTOR,
	INT,
	BOOLEAN,
	CHAR,
	VOID,
	VAR,
	STATIC,
	FIELD,
	LET,
	DO,
	IF,
	ELSE,
	WHILE,
	RETURN,
	TRUE,
	FALSE,
	_NULL,
	THIS,
	NOTKEYWORD
};

class Token 
{
public:
	enum tokenType type;
	enum keyWord keyword;
	int lineNum;
	string text;
};

class JackTokenizer
{
public:
	JackTokenizer(ifstream& fileStream, bool verbose=false);

	bool has_more_tokens();
	void advance();
	enum tokenType tokenType();
	enum keyWord keyWord();
	char symbol();
	string identifier();
	int intVal();
	string stringVal();
	string text();
	int lineNum();
	

private:
	ifstream fileStream;
	vector<Token> tokens {};
	Token token {};
	int _lineNum = 1;
	int tokenIdx = 0;
	bool verbose = false;

	unordered_map<string, enum keyWord> mapKeyword {
		{"class", 		CLASS},
		{"method",		METHOD},
		{"function",	FUNCTION},
		{"constructor",	CONSTRUCTOR},
		{"int",			INT},
		{"boolean",		BOOLEAN},
		{"char",		CHAR},
		{"void",		VOID},
		{"var",			VAR},
		{"static",		STATIC},
		{"field",		FIELD},
		{"let",			LET},
		{"do",			DO},
		{"if",			IF},
		{"else",		ELSE},
		{"while",		WHILE},
		{"return",		RETURN},
		{"true",		TRUE},
		{"false",		FALSE},
		{"null",		_NULL},
		{"this",		THIS},
	};

	vector<Token> parse(ifstream& fileStream);
	void end_token();
};