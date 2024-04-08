#include "parser.cpp"
#include "code.cpp"	
#include <bitset>
using namespace std;

string get_file_name(string path);
bool is_number(string s);

int main(int argc, char* argv[]) 
{
	if(argc < 2) {
		cerr << "Must pass the path of input .asm file as command line argument\n";
		return -1;
	}

	string filename = get_file_name(argv[1]);
	Code code = Code();

	// First Pass
	{
		int lineNum = 0;
		Parser parser = Parser(argv[1]);
		while(parser.has_more_lines()) {
			parser.advance();
			int type = parser.instruction_type();

			// add labels - Set address to the next line number (which would be lineNum because we are not incrementing it)
			if(type == L_instruction) {
				if(code.contains(parser.symbol()) == false)
					code.add_entry(parser.symbol(), lineNum);
			}
			else 
				lineNum++;
		}

		parser.close();
	}

	// Second Pass
	ofstream outf {filename + ".hack", ios::out | ios::trunc};

	if(!outf) {
		cerr << filename << ".hack could not be opened for writing\n" << endl;
		return -1;
	}

	Parser parser = Parser(argv[1]);
	string instruction {};
	int symbol;
	while(parser.has_more_lines()) {

		parser.advance();
		int type = parser.instruction_type();

		if(type == C_instruction) {
			instruction = "111" 
						+ code.comp(parser.comp()) 
						+ code.dest(parser.dest()) 
						+ code.jump(parser.jump());
		}

		else if(type == A_instruction) {

			// if variable, add to symbol table
			if(is_number(parser.symbol()) == false) {
				if(code.contains(parser.symbol()) == false) {
					code.add_entry(parser.symbol());
				}

				bitset<16> binary(code.get_address(parser.symbol()));
				instruction = binary.to_string();
			}

			else {
				int num = stoi(parser.symbol());
				bitset<15> binary(num);
				instruction = "0" + binary.to_string();
			}
		}

		else 
			continue;

		outf << instruction << "\n";
	}

	return 0;
}

string get_file_name(string path) 
{
	int dot_pos = path.find_last_of(".");
	return path.substr(0, dot_pos);
}

bool is_number(string s)
{
	int i = 0;
	while(i < s.size() && isdigit(s[i]))
		i++;

	return i == s.size();
}