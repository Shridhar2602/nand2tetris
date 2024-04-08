#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

class Code
{
public:

	Code() 
	{
		init_comp_codes();
		init_dest_codes();
		init_jump_codes();
		init_symbol_table();
	}

	string dest(string d) 
	{
		if(dest_map.count(d) != 0)
			return dest_map[d];

		cerr << "Invalid Dest Instruction " << d << "\n";
		return "null";
	}

	string comp(string c)
	{
		if(comp_map.count(c) != 0)
			return comp_map[c];

		cerr << "Invalid Comp Instruction " << c << "\n";
		return "null";
	}

	string jump(string j)
	{
		if(jump_map.count(j) != 0)
			return jump_map[j];

		cerr << "Invalid Jump Instruction " << j << "\n";
		return "null";
	}

	void add_entry(string j, int address)
	{
		if(symbol_table.count(j) == 0)
			symbol_table[j] = address;

		else {
			cerr << "Already present in the symbol table: " << j << "\n";
		}
	}

	void add_entry(string j)
	{
		if(symbol_table.count(j) == 0)
			symbol_table[j] = var_loc++;

		else {
			cerr << "Already present in the symbol table: " << j << "\n";
		}
	}

	bool contains(string j)
	{
		return symbol_table.count(j);
	}

	unsigned int get_address(string j)
	{
		if(symbol_table.count(j))
			return symbol_table[j];

		cerr << "Not present in the symbol table: " << j << "\n";	
		return -1;
	}

private:
	unordered_map<string, string> jump_map;
	unordered_map<string, string> dest_map;
	unordered_map<string, string> comp_map;
	unordered_map<string, unsigned int> symbol_table;
	unsigned int var_loc = 16;

	void init_comp_codes()
	{
		comp_map = unordered_map<string, string> {
			{"0"	, 	"0101010"},
			{"1"	, 	"0111111"},
			{"-1"	, 	"0111010"},
			{"D"	, 	"0001100"},
			{"A"	, 	"0110000"},
			{"!D"	, 	"0001101"},
			{"!A"	, 	"0110001"},
			{"-D"	, 	"0001111"},
			{"-A"	, 	"0110011"},
			{"D+1"	, 	"0011111"},
			{"A+1"	, 	"0110111"},
			{"D-1"	, 	"0001110"},
			{"A-1"	, 	"0110010"},
			{"D+A"	, 	"0000010"},
			{"D-A"	, 	"0010011"},
			{"A-D"	, 	"0000111"},
			{"D&A"	, 	"0000000"},
			{"D|A"	, 	"0010101"},
			{"M"	, 	"1110000"},
			{"!M"	, 	"1110001"},
			{"-M"	, 	"1110011"},
			{"M+1"	, 	"1110111"},
			{"M-1"	, 	"1110010"},
			{"D+M"	, 	"1000010"},
			{"D-M"	, 	"1010011"},
			{"M-D"	, 	"1000111"},
			{"D&M"	, 	"1000000"},
			{"D|M"	, 	"1010101"}
		};
	}

	void init_dest_codes()
	{
		dest_map = unordered_map<string, string> {
			{"null", "000"},
			{"M", "001"},
			{"D", "010"},
			{"DM", "011"},
			{"MD", "011"},
			{"A", "100"},
			{"AM", "101"},
			{"MA", "101"},
			{"AD", "110"}, 
			{"DA", "110"}, 
			{"ADM", "111"},
			{"AMD", "111"},
			{"DAM", "111"},
			{"DMA", "111"},
			{"MAD", "111"},
			{"MDA", "111"},
		};
	}

	void init_jump_codes() 
	{
		jump_map = unordered_map<string, string> {
			{"null", "000"},
			{"JGT", "001"},
			{"JEQ", "010"},
			{"JGE", "011"},
			{"JLT", "100"},
			{"JNE", "101"},
			{"JLE", "110"},
			{"JMP", "111"}
    	};
	}

	void init_symbol_table()
	{
		symbol_table = unordered_map<string, unsigned int> {
			{"R0", 0},
			{"R1", 1},
			{"R2", 2},
			{"R3", 3},
			{"R4", 4},
			{"R5", 5},
			{"R6", 6},
			{"R7", 7},
			{"R8", 8},
			{"R9", 9},
			{"R10", 10},
			{"R11", 11},
			{"R12", 12},
			{"R13", 13},
			{"R14", 14},
			{"R15", 15},
			{"SP", 0},
			{"LCL", 1},
			{"ARG", 2},
			{"THIS", 3},
			{"THAT", 4},
			{"SCREEN", 16384},
			{"KBD", 24576}
		};
	}
};