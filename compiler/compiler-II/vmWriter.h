#pragma once

#include <iostream>
#include <fstream>

namespace vmWriter {

using namespace std;

namespace segment{
	enum Segment {
		CONST,
		ARG,
		LOCAL,
		STATIC,
		THIS,
		THAT,
		POINTER,
		TEMP,
		NONE
	};
}

namespace command {
	enum Command {
		ADD,
		SUB,
		NEG,
		EQ,
		GT,
		LT,
		AND,
		OR,
		NOT
	};
}

using namespace segment;
using namespace command;

class VMWriter 
{
public:
	VMWriter(ofstream& outStream);
	void writePush(Segment segment, int index);
	void writePop(Segment segment, int index);
	void writeArithmetic(Command command);
	void writeLabel(string label);
	void writeGoto(string label);
	void writeIf(string label);
	void writeCall(string name, int nArgs);
	void writeFunction(string name, int nVars);
	void writeReturn();

private:
	ofstream& out;
};

}	// namespace vmWriter