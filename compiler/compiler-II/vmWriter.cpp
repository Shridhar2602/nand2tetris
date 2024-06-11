#include "vmWriter.h"

namespace vmWriter {
VMWriter::VMWriter(ofstream& outStream) : out(outStream) {

}

void VMWriter::writePush(Segment segment, int index) {
	switch(segment) {
		case CONST:
			out << "push constant " << index << "\n";
			break;
		case ARG:
			out << "push argument " << index << "\n";
			break;
		case LOCAL:
			out << "push local " << index << "\n";
			break;
		case STATIC:
			out << "push static " << index << "\n";
			break;
		case THIS:
			out << "push this " << index << "\n";
			break;
		case THAT:
			out << "push that " << index << "\n";
			break;
		case POINTER:
			out << "push pointer " << index << "\n";
			break;
		case TEMP:
			out << "push temp " << index << "\n";
			break;
		default:
			break;
	}
}

void VMWriter::writePop(Segment segment, int index) {
	switch(segment) {
		case CONST:
			out << "pop constant " << index << "\n";
			break;
		case ARG:
			out << "pop argument " << index << "\n";
			break;
		case LOCAL:
			out << "pop local " << index << "\n";
			break;
		case STATIC:
			out << "pop static " << index << "\n";
			break;
		case THIS:
			out << "pop this " << index << "\n";
			break;
		case THAT:
			out << "pop that " << index << "\n";
			break;
		case POINTER:
			out << "pop pointer " << index << "\n";
			break;
		case TEMP:
			out << "pop temp " << index << "\n";
			break;
		default:
			break;
	}
}

void VMWriter::writeArithmetic(Command command) {
	switch(command) {
		case ADD:
			out << "add\n";
			break;
		case SUB:
			out << "sub\n";
			break;
		case NEG:
			out << "neg\n";
			break;
		case EQ:
			out << "eq\n";
			break;
		case GT:
			out << "gt\n";
			break;
		case LT:
			out << "lt\n";
			break;
		case AND:
			out << "and\n";
			break;
		case OR:
			out << "or\n";
			break;
		case NOT:
			out << "not\n";
			break;
		default:
			cerr << "VMWriter: Invalid Arithmeticcommand: " << command << "\n";
	}
}

void VMWriter::writeLabel(string label) {
	out << "label " << label << "\n";
}

void VMWriter::writeGoto(string label) {
	out << "goto " << label << "\n";
}

void VMWriter::writeIf(string label) {
	out << "if-goto " << label << "\n";
}

void VMWriter::writeCall(string name, int nArgs) {
	out << "call " << name << " " << nArgs << "\n";
}

void VMWriter::writeFunction(string name, int nVars) {
	out << "function " << name << " " << nVars << "\n";
}

void VMWriter::writeReturn() {
	out << "return\n";
}

}	// namespace vmWriter