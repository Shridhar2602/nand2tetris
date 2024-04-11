#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
using namespace std;

class CodeWriter
{
public:
	CodeWriter(string outFilePath) 
	{
		init(outFilePath);
	}

	void write_push(string segment, int index)
	{
		push(segment, index);
	}

	void write_pop(string segment, int index)
	{
		pop(segment, index);
	}

	void write_arithmetic(string command)
	{
		calc_arithmetic(command);
	}

	void write_end_loop()
	{
		outf << "(VM_END)\n";
		outf << "@VM_END\n";
		outf <<	"0;JMP\n";
	}

	void close()
	{
		outf.close();
	}

private:
	ofstream outf;
	string m_outFileName {};
	int m_comp_counter = 0;
	unordered_map <string, string> umap {{"local", "LCL"}, {"argument", "ARG"}, {"this", "THIS"}, {"that", "THAT"}};

	int init(string outputPath) 
	{
		outf.open(outputPath + ".asm", ios::out | ios::trunc);

		if(!outf) {
			cerr << outputPath << ".asm could not be opened for writing\n" << endl;
			return 1;
		}

		m_outFileName = outputPath.substr(outputPath.rfind('/') + 1);

		return 0;
	}

	// Results of all ops are stored in D. 
	void calc_arithmetic(string command) 
	{
		decrement_SP();
		access_SP();		// the first arg is accessed by all ops
		if(command == "add") {	
			decrement_SP();
			outf << "A = M\n";
			outf << "D = D + M\n";
		}
		else if(command == "sub") {
			decrement_SP();
			outf << "A = M\n";
			outf << "D = M - D\n";
		}
		else if(command == "and") {
			decrement_SP();
			outf << "A = M\n";
			outf << "D = D & M\n";
		}
		else if(command == "or") {
			decrement_SP();
			outf << "A = M\n";
			outf << "D = D | M\n";
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
			outf << "A = M\n";														
			outf << "D = M - D\n";													
			outf << "@" << m_outFileName << ".NOTEQ" << m_comp_counter << "\n";
			outf << "D;JEQ\n";
			outf << "D = 0\n";
			outf << "@" << m_outFileName << ".EQCONT" << m_comp_counter << "\n";
			outf << "0;JMP\n";
			outf << "(" << m_outFileName << ".NOTEQ" << m_comp_counter << ")\n";
			outf << "D = -1\n";
			outf << "(" << m_outFileName << ".EQCONT" << m_comp_counter << ")\n";
		}
		else if(command == "lt") {
			m_comp_counter++;
			decrement_SP();
			outf << "A = M\n";
			outf << "D = M - D\n";
			outf << "@" << m_outFileName << ".NOTLT" << m_comp_counter << "\n";
			outf << "D;JLT\n";
			outf << "D = 0\n";
			outf << "@" << m_outFileName << ".LTCONT" << m_comp_counter << "\n";
			outf << "0;JMP\n";
			outf << "(" << m_outFileName << ".NOTLT" << m_comp_counter << ")\n";
			outf << "D = -1\n";
			outf << "(" << m_outFileName << ".LTCONT" << m_comp_counter << ")\n";
		}
		else if(command == "gt") {
			m_comp_counter++;
			decrement_SP();
			outf << "A = M\n";
			outf << "D = M - D\n";
			outf << "@" << m_outFileName << ".NOTGT" << m_comp_counter << "\n";
			outf << "D;JGT\n";
			outf << "D = 0\n";
			outf << "@" << m_outFileName << ".GTCONT" << m_comp_counter << "\n";
			outf << "0;JMP\n";
			outf << "(" << m_outFileName << ".NOTGT" << m_comp_counter << ")\n";
			outf << "D = -1\n";
			outf << "(" << m_outFileName << ".GTCONT" << m_comp_counter << ")\n";
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
			outf << "@" << index << "\n";
			outf << "D = A\n";
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
			outf << "@" << 5 + index << "\n";
			outf << "D = M\n";
		}

		else if(segment == "static") {
			outf << "@" << m_outFileName << "." << index << "\n";
			outf << "D = M\n";
		}

		else {
			cerr << "Invalid segment : " << segment << "\n";
		}

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
				outf << "@THIS\n";
				outf << "M = D\n";
			}
			else {
				outf << "@THAT\n";
				outf << "M = D\n";
			}
		}

		else if(segment == "temp") {
			access_SP();
			outf << "@" << (5 + index) << "\n";
			outf << "M = D\n";
		}

		else if(segment == "static") {
			access_SP();
			outf << "@" << m_outFileName << "." << index << "\n";
			outf << "M = D\n";
		}

		else {
			cerr << "Invalid segment : " << segment << "\n";
		}
	}

	// gets value from RAM and stores it in D
	void access_memory(string segment, int index)
	{
		outf << "@" << segment << "\n";		// A = segment
		outf << "D = M\n";					// D = RAM[segment] (start address of segment)
		outf << "@" << index << "\n";		
		outf << "A = D + A\n";				// A = RAM[segment] + index
		outf << "D = M\n";					// D = RAM[RAM[segment] + index]
	}

	void access_memory_direct(string segment) 
	{
		outf << "@" << segment << "\n";
		outf << "D = M\n";
	}

	// gets value stored at top of stack and stores it in R13
	void access_SP(string reg) 
	{
		outf << "@SP\n";
		outf << "A = M\n";
		outf << "D = M\n";
		outf << "@" << reg << "\n";
		outf << "M = D\n";
	}

	// gets value stored at top of stack and store it in D
	void access_SP()
	{
		outf << "@SP\n";
		outf << "A = M\n";
		outf << "D = M\n";
	}

	void write_D_on_stack()
	{
		outf << "@SP\n";
		outf << "A = M\n";
		outf << "M = D\n";
	}

	// write contents of R13 in RAM[segment] + index
	void write_reg_on_memory(string segment, int index, string reg)
	{
		outf << "@" << segment << "\n";
		outf << "D = M\n";
		outf << "@" << index << "\n";
		outf << "D = D + A\n";
		outf << "@R15\n";
		outf << "M = D\n";
		outf << "@" << reg << "\n";
		outf << "D = M\n";
		outf << "@R15\n";
		outf << "A = M\n";
		outf << "M = M + D\n";
	}

	void decrement_SP()
	{
		outf << "@SP" << "\n";		// A = Stack Pointer
		outf << "M = M - 1\n";		// SP--
	}

	void increment_SP()
	{
		outf << "@SP" << "\n";		// A = Stack Pointer
		outf << "M = M + 1\n";		// SP++
	}
};