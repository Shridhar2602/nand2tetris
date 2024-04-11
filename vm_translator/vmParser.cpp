#include "../lib/parser.cpp"

enum commmandType {
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
};

class VMParser : public Parser 
{
public:
	VMParser(string inputPath) : Parser(inputPath) {};

	void advance() 
	{
		if(!has_more_lines()) {
			cerr << "Input doesn't have any more lines!\n";
			return;
		}

		m_curr_command = get_next_line();
		cout << "Command: " << m_curr_command << "\n";

		parse_command();

		if(m_command_type != C_ARITHMETIC) {
			parse_args();
		}
	}

	int command_type() { return m_command_type; }

	string arg1() 
	{
		if(m_command_type == C_RETURN) {
			cerr << "arg1() must be called only if commandType is not C_RETURN\n";
			return "null";
		}

		if(m_command_type == C_ARITHMETIC)
			return m_curr_command;

		return m_arg1;
	}

	int arg2()
	{
		if(m_command_type == C_PUSH || m_command_type == C_POP || m_command_type == C_FUNCTION || m_command_type == C_CALL)
			return stoi(m_arg2);

		cerr << "arg2() must be called only if command_type is PUSH, POP, FUNCTION or CALL\n";
		return -1;
	}

private:
	string m_curr_command {};
	int m_command_type = -1;
	string m_arg1 {}, m_arg2 {};

	void parse_command()
	{
		if (m_curr_command == "add" || 
			m_curr_command == "sub" || 
			m_curr_command == "neg" || 
			m_curr_command == "eq" || 
			m_curr_command == "gt" || 
			m_curr_command == "lt" || 
			m_curr_command == "and" || 
			m_curr_command == "or" || 
			m_curr_command == "not") 
		{
			m_command_type = C_ARITHMETIC;
			return;
		}

		string first_word = m_curr_command.substr(0, m_curr_command.find(' ', 0));

		if(first_word == "push") {
			m_command_type = C_PUSH;
		}
		
		else if(first_word == "pop") {
			m_command_type = C_POP;
		}
	}

	void parse_args()
	{
		int pos2 = m_curr_command.rfind(' ');
		int pos1 = m_curr_command.rfind(' ', pos2 - 1);
		
		m_arg2 = m_curr_command.substr(pos2 + 1);
		m_arg1 = m_curr_command.substr(pos1 + 1, pos2 - pos1);

		remove_trailing_spaces(m_arg1);
	}
};