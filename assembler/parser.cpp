#include <iostream>
#include <string>
#include <fstream>
using namespace std;

enum instruction 
{
	A_instruction,
	C_instruction,
	L_instruction,
};

class Parser 
{
public:
	Parser(string inputFile) 
	{
		init(inputFile);
	}

	bool has_more_lines() 
	{
		char peekedChar = inf.peek();
		if(peekedChar == EOF)
			return false;
		return true;
	}

	void advance() 
	{
		if(!has_more_lines()) { 
			cerr << "Input doesn't have any more lines!\n"; 
			return; 
		}

		curr_inst = get_next_instruction();
		// cout << "Instruction: " << curr_inst << "\n";

		parse_instruction_type();

		if(inst_type == C_instruction)
			parse_C_instruction();
		else
			parse_AL_instruction();
	}

	int instruction_type() { return inst_type; }

	string symbol() 
	{
		if(inst_type == C_instruction) {
			cerr << "symbol() must be called only if instructionType is A_instruction or L_instruction\n";
			return "null";
		}

		return m_symbol;
	}

	string dest() 
	{
		if(inst_type != C_instruction) {
			cerr << "dest() must be called only if instructionType is C_instruction\n";
			return "null";
		}

		return m_dest;
	}

	string comp() 
	{
		if(inst_type != C_instruction) {
			cerr << "comp() must be called only if instructionType is C_instruction\n";
			return "null";
		}

		return m_comp;
	}

	string jump() 
	{
		if(inst_type != C_instruction) {
			cerr << "jump() must be called only if instructionType is C_instruction\n";
			return "null";
		}

		return m_jump;
	}

	void close() 
	{
		inf.close();
	}

private:
	ifstream inf;
	string parsed {};
	string curr_inst {};
	string m_comp {};
	string m_dest {};
	string m_jump {};
	string m_symbol {};
	int inst_type = -1;
	
	int init(string inputPath) 
	{
		inf.open(inputPath, std::ios::in);

		if(!inf) {
			cerr << "Input .asm could not be opened!\n";
			return 1;
		}

		return 0;
	}

	void parse_instruction_type() 
	{
		if(curr_inst[0] == '@')
			inst_type = A_instruction;
		else if(curr_inst[0] == '(')
			inst_type = L_instruction;
		else
			inst_type = C_instruction;
	}

	string get_next_instruction() 
	{
		while(getline(inf, parsed)) 
		{
			// to deal with empty lines and LF/CRLF 
			if(parsed.empty() || (parsed.size() == 1 && parsed[0] == '\r'))
				continue;

			remove_leading_spaces(parsed);
			remove_eol(parsed);

			int comment_pos = parsed.find("//", 0);
			if(comment_pos == 0)
				continue;

			parsed = parsed.substr(0, comment_pos);
			remove_trailing_spaces(parsed);
			break;
		}


		return parsed;
	}

	void parse_AL_instruction() 
	{
		int n = curr_inst.size();

		if(inst_type == L_instruction) {
			int i = 1;
			while(i < n && curr_inst[i] != ')')
				i++;
			
			if(i == n) {
				cerr << "invalid L_instruction\n";
				m_symbol = "null";
			}

			m_symbol = curr_inst.substr(1, i - 1);
		}

		else {
			m_symbol = curr_inst.substr(1, n - 1);
		}

		remove_trailing_spaces(m_symbol);
	}

	void parse_C_instruction()
	{
		int i = 0;
		int n = curr_inst.size();
		int b1 = -1, b2 = -1;
		while(i < n) {
			if(curr_inst[i] == '=')
				b1 = i;
			else if(curr_inst[i] == ';')
				b2 = i;

			i++;		
		}
		
		// extracting dest 
		if(b1 != -1) {
			m_dest = curr_inst.substr(0, b1);
			remove_trailing_spaces(m_dest);
		}
		else 
			m_dest = "null";

		// extracting comp
		if(b2 != -1) {
			if(b1 != -1)
				m_comp = curr_inst.substr(b1 + 1, b2 - b1 - 1);
			else
				m_comp = curr_inst.substr(0, b2);
		}
		else {
			if(b1 != -1)
				m_comp = curr_inst.substr(b1 + 1, n - b1 - 1);
			else
				m_comp = curr_inst.substr(0, n - 1);
		}
			

		remove_leading_spaces(m_comp);
		remove_trailing_spaces(m_comp);

		// extracting jump
		if(b2 != -1) {
			m_jump = curr_inst.substr(b2 + 1, n - b2 - 1);
			remove_leading_spaces(m_jump);
			remove_trailing_spaces(m_jump);
		}
		else 
			m_jump = "null";
	}

	void remove_leading_spaces(string& s) 
	{
		int i = 0;
		int n = s.size();
		while(i < n && (s[i] == ' ' || s[i] == '\t'))
			i++;
		s = s.substr(i, n - i);
	}

	void remove_trailing_spaces(string& s)
	{
		int i = s.size() - 1;
		while(i >=0 && (s[i] == ' ' || s[i] == '\t'))
			i--;

		s = s.substr(0, i + 1);
	}

	void remove_eol(string& s) 
	{
		if(s.back() == '\n')
			s.pop_back();
		if(s.back() == '\r')
			s.pop_back();
	}
};