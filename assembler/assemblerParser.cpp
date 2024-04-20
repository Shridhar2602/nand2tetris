#include "../lib/parser.cpp"

enum instruction 
{
	A_instruction,
	C_instruction,
	L_instruction,
};

class AssemblerParser : public Parser
{
public:
	AssemblerParser(string inputFile) : Parser(inputFile)
	{

	}

	void advance() 
	{
		if(!has_more_lines()) { 
			cerr << "Input doesn't have any more lines!\n"; 
			return; 
		}

		curr_inst = get_next_line();
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

private:
	string curr_inst {};
	string m_comp {};
	string m_dest {};
	string m_jump {};
	string m_symbol {};
	int inst_type = -1;
	
	void parse_instruction_type() 
	{
		if(curr_inst[0] == '@')
			inst_type = A_instruction;
		else if(curr_inst[0] == '(')
			inst_type = L_instruction;
		else
			inst_type = C_instruction;
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
};