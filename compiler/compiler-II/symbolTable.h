#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

namespace symbolTable {

using namespace std;

enum symbolKind {
	STATIC,
	FIELD,
	ARG,
	VAR,
	NONE
};

class SymbolTable
{
public:
	SymbolTable();
	void reset();
	void define(string name, string type, enum symbolKind kind);
	int varCount(enum symbolKind kind);
	enum symbolKind kindOf(string name);
	string typeOf(string name);
	int indexOf(string name);

private:
	struct symbol {
		string type;
		enum symbolKind kind;
		int index;
	};
	
	unordered_map<string, symbol> table {};
	int _staticCount {0};
	int _fieldCount {0};
	int _argCount {0};
	int _varCount {0};
};

}	// symbol table