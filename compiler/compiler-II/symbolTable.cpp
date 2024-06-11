#include "symbolTable.h"

namespace symbolTable {

SymbolTable::SymbolTable() {

}

void SymbolTable::reset() {
	_staticCount = 0;
	_fieldCount = 0;
	_argCount = 0;
	_varCount = 0;
	table.clear();
}

void SymbolTable::define(string name, string type, enum symbolKind kind) {

	int* count {};
	switch(kind) {
		case STATIC:
			count = &_staticCount; break;
		case FIELD:
			count = &_fieldCount; break;
		case ARG:
			count = &_argCount; break;
		case VAR:
			count = &_varCount; break;
		case NONE:
			cerr << "SymbolTable: Invalid kind\n";
	}

	table[name] = {type, kind, *count};
	(*count)++;
}

int SymbolTable::varCount(enum symbolKind kind) {
	switch(kind) {
		case STATIC: return _staticCount;
		case FIELD: return _fieldCount;
		case ARG: return _argCount;
		case VAR: return _varCount;
		case NONE: cerr << "SymbolTable: Invalid kind\n";
	}

	return -1;
}

enum symbolKind SymbolTable::kindOf(string name) {
	if(table.count(name) == 0)
		return NONE;

	return table[name].kind;
}

string SymbolTable::typeOf(string name) {
	if(table.count(name) == 0) {
		cerr << "SymbolTable: " << name << " not found\n";
		return nullptr;
	}

	return table[name].type;
}

int SymbolTable::indexOf(string name) {
	if(table.count(name) == 0) {
		// cerr << "SymbolTable: " << name << " not found\n";
		return -1;
	}

	return table[name].index;
}

}	// symboltable