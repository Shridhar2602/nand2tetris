#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
using namespace std;

class CodeWriter
{
public:
	CodeWriter(string outFilePath) {
		init(outFilePath);

		// bootstrap code
		outf << 
				"@256\n"
				"D = A\n"
				"@SP\n"
				"M = D\n"
				"@1\n"
				"D = A\n"
				"@LCL\n"
				"M = -D\n"
				"@2\n"
				"D = A\n"
				"@ARG\n"
				"M = -D\n"
				"@3\n"
				"D = A\n"
				"@THIS\n"
				"M = -D\n"
				"@4\n"
				"D = A\n"
				"@THAT\n"
				"M = -D\n";
		write_call("Sys.init", 0);
				
	}

	void set_fileName(string fileName) {
		m_fileName = fileName;
	}

	void write_push(string segment, int index) {
		push(segment, index);
	}

	void write_pop(string segment, int index) {
		pop(segment, index);
	}

	void write_label(string label) {
		outf << "(" << generate_label(label) << ")\n";
	}

	void write_goto(string label) {
		outf << "@" << generate_label(label) << "\n"
				"0;JMP\n";
	}

	void write_if_goto(string label) {
		decrement_SP();
		access_SP();
		outf << "@" + generate_label(label) + "\n"
				"D;JNE\n";	
	} 

	void write_arithmetic(string command) {
		calc_arithmetic(command);
	}

	void write_end_loop() {
		outf << "(VM_END)\n"
				"@VM_END\n"\
				"0;JMP\n";
	}

	void write_function(string functionName, int nVars) {
		m_funcName = functionName;									

		outf << "(" + m_funcName + ")\n";	// label marking the start of this function
		for(int i = 0; i < nVars; i++) {						// push all local variables on stack (default val = 0)
			outf << "@0\n"
					"D = A\n";
			write_D_on_stack();
			increment_SP();
		}
	}

	/*
		The stack -
		[caller's stack			]
		[callee's arguments		]	// callee's return must restore the SP to here
		[return address			]	// start of caller's frame
		[LCL, ARG, THIS, THAT	]
		[callee's variables		]	// marked by LCL of callee
		[callee's stack			]
	*/

	void write_call(string functionName, int nArgs) {
		m_numCalls++;

		// Push the return address on stack - it will be used later to return control to the caller
		outf << "@" + m_funcName + "$ret." << m_numCalls << "\n"
				"D = A\n";
		write_D_on_stack();
		increment_SP();

		push("LCL");
		push("ARG");
		push("THIS");
		push("THAT");

		// ARG of the calle = SP - 5 - nArgs
		outf << "@SP\n"
				"D = M\n";
		outf << "@" << (5 + nArgs) << "\n"
				"D = D - A\n"
				"@ARG\n"
				"M = D\n";

		// The callee function will now push all local variables to top of stack
		outf << "@SP\n"
				"D = M\n";
		outf << "@LCL\n"
				"M = D\n";

		outf << "@" + functionName + "\n"
				"0;JMP\n";

		// The return label to which the callee must return. The value of this label is also stored in stack
		outf << "(" + m_funcName + "$ret." << m_numCalls << ")\n";
	}

	void write_return() {

		// The local segment of callee marks the end of frame of caller
		// Store LCL in R13 and return address *(LCL - 5) in R14
		access_memory_direct("LCL");
		outf << "@R13\n"
				"M = D\n"
				"@5\n"
				"D = D - A\n"
				"A = D\n"
				"D = M\n"
				"@R14\n"
				"M = D\n";

		// The argument segment of callee marks the point at which the caller's stack ended
		// So we must put the return value there
		decrement_SP();
		access_SP();
		outf << "@ARG\n"		// the address pointed by ARG should have return val
				"A = M\n"
				"M = D\n";
		
		// The SP = ARG + 1
		access_memory_direct("ARG");
		outf << "@SP\n"
				"M = D + 1\n";
		
		restore_from_frame("THAT", "1");
		restore_from_frame("THIS", "2");
		restore_from_frame("ARG",  "3");
		restore_from_frame("LCL",  "4");

		// jump to return address
		outf << "@R14\n"
				"A = M\n"
				"0;JMP\n";
	}

	void close() {
		outf.close();
	}

private:
	ofstream outf;
	string m_fileName {};
	string m_funcName {};
	int m_numCalls = 0;
	int m_comp_counter = 0;
	unordered_map <string, string> umap {{"local", "LCL"}, {"argument", "ARG"}, {"this", "THIS"}, {"that", "THAT"}};

	int init(string outputPath) 
	{
		outf.open(outputPath + ".asm", ios::out | ios::trunc);

		if(!outf) {
			cerr << outputPath << ".asm could not be opened for writing\n" << endl;
			return 1;
		}

		return 0;
	}

	// Results of all ops are stored in D. 
	void calc_arithmetic(string command) 
	{
		decrement_SP();
		access_SP();		// the first arg is accessed by all ops
		if(command == "add") {	
			decrement_SP();
			outf << "A = M\n"
					"D = D + M\n";
		}
		else if(command == "sub") {
			decrement_SP();
			outf << "A = M\n"
					"D = M - D\n";
		}
		else if(command == "and") {
			decrement_SP();
			outf << "A = M\n"
					"D = D & M\n";
		}
		else if(command == "or") {
			decrement_SP();
			outf << "A = M\n"
					"D = D | M\n";
		}
		else if(command == "not") {
			outf << "D = !D\n";
		}
		else if(command == "neg") {
			outf << "D = -D\n";
		}
		else if(command == "eq") {
			m_comp_counter++;
			decrement_SP();
			outf << "A = M\n"													
					"D = M - D\n"												
					"@" << m_fileName << ".NOTEQ" << m_comp_counter << "\n"
					"D;JEQ\n"
					"D = 0\n"
					"@" << m_fileName << ".EQCONT" << m_comp_counter << "\n"
					"0;JMP\n"
					"(" << m_fileName << ".NOTEQ" << m_comp_counter << ")\n"
					"D = -1\n"
					"(" << m_fileName << ".EQCONT" << m_comp_counter << ")\n";
		}
		else if(command == "lt") {
			m_comp_counter++;
			decrement_SP();
			outf << "A = M\n"
					"D = M - D\n"
					"@" << m_fileName << ".NOTLT" << m_comp_counter << "\n"
					"D;JLT\n"
					"D = 0\n"
					"@" << m_fileName << ".LTCONT" << m_comp_counter << "\n"
					"0;JMP\n"
					"(" << m_fileName << ".NOTLT" << m_comp_counter << ")\n"
					"D = -1\n"
					"(" << m_fileName << ".LTCONT" << m_comp_counter << ")\n";
		}
		else if(command == "gt") {
			m_comp_counter++;
			decrement_SP();
			outf << "A = M\n"
					"D = M - D\n"
					"@" << m_fileName << ".NOTGT" << m_comp_counter << "\n"
					"D;JGT\n"
					"D = 0\n"
					"@" << m_fileName << ".GTCONT" << m_comp_counter << "\n"
					"0;JMP\n"
					"(" << m_fileName << ".NOTGT" << m_comp_counter << ")\n"
					"D = -1\n"
					"(" << m_fileName << ".GTCONT" << m_comp_counter << ")\n";
		}
		else {
			cerr << "Invalid Arithmetic Command\n";
		}

		write_D_on_stack();
		increment_SP();
	}

	// push writes the contents of RAM[segment + index] to stack
	void push(string segment, int index)
	{
		if(segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
			access_memory(umap[segment], index);
		}

		else if(segment == "constant") {
			outf << "@" << index << "\n"
					"D = A\n";
		}

		else if(segment == "pointer") {
			if(index == 0) {
				access_memory_direct("THIS");
			}
			else {
				access_memory_direct("THAT");
			}
		}

		else if(segment == "temp") {
			outf << "@" << 5 + index << "\n"
					"D = M\n";
		}

		else if(segment == "static") {
			outf << "@" << m_fileName << "." << index << "\n"
					"D = M\n";
		}

		else {
			cerr << "Invalid segment : " << segment << "\n";
		}

		write_D_on_stack();
		increment_SP();
	}

	void push(string segment) 
	{
		access_memory_direct(segment);
		write_D_on_stack();
		increment_SP();
	}

	// pop writes the content of top of the stack to RAM[segment + index]
	void pop(string segment, int index)
	{
		decrement_SP();
		if(segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
			access_SP("R13");
			write_reg_on_memory(umap[segment], index, "R13");
		}

		else if(segment == "constant") {

		}

		else if(segment == "pointer") {
			access_SP();
			if(index == 0) {
				outf << "@THIS\n"
						"M = D\n";
			}
			else {
				outf << "@THAT\n"
						"M = D\n";
			}
		}

		else if(segment == "temp") {
			access_SP();
			outf << "@" << (5 + index) << "\n"
					"M = D\n";
		}

		else if(segment == "static") {
			access_SP();
			outf << "@" << m_fileName << "." << index << "\n"
					"M = D\n";
		}

		else {
			cerr << "Invalid segment : " << segment << "\n";
		}
	}

	// gets value from RAM and stores it in D
	void access_memory(string segment, int index)
	{
		outf << "@" << segment << "\n"		// A = segment
				"D = M\n"					// D = RAM[segment] (start address of segment)
				"@" << index << "\n"		
				"A = D + A\n"				// A = RAM[segment] + index
				"D = M\n";					// D = RAM[RAM[segment] + index]
	}

	void access_memory_direct(string segment) 
	{
		outf << "@" << segment << "\n"
				"D = M\n";
	}

	// gets value stored at top of stack and stores it in R13
	void access_SP(string reg) 
	{
		outf << "@SP\n"
				"A = M\n"
				"D = M\n"
				"@" << reg << "\n"
				"M = D\n";
	}

	// gets value stored at top of stack and store it in D
	void access_SP()
	{
		outf << "@SP\n"
				"A = M\n"
				"D = M\n";
	}

	void write_D_on_stack()
	{
		outf << "@SP\n"
				"A = M\n"
				"M = D\n";
	}

	// write contents of R13 in RAM[segment] + index
	void write_reg_on_memory(string segment, int index, string reg)
	{
		outf << "@" << segment << "\n"
				"D = M\n"
				"@" << index << "\n"
				"D = D + A\n"
				"@R15\n"
				"M = D\n"
				"@" << reg << "\n"
				"D = M\n"
				"@R15\n"
				"A = M\n"
				"M = D\n";
	}

	void decrement_SP()
	{
		outf << "@SP" << "\n"		// A = Stack Pointer
				"M = M - 1\n";		// SP--
	}

	void increment_SP()
	{
		outf << "@SP" << "\n"		// A = Stack Pointer
				"M = M + 1\n";		// SP++
	}

	string generate_label(string label) {
		return m_fileName + "." + m_funcName + "$" + label;
	}

	void restore_from_frame(string segment, string offset)
	{
		outf << "@R13\n"
				"D = M\n"
				"@" + offset + "\n"
				"D = D - A\n"
				"A = D\n"
				"D = M\n"
				"@" + segment + "\n"
				"M = D\n";
	}
};