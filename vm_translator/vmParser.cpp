#include "../lib/parser.cpp"
#include <unordered_map>

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
	VMParser(string inputPath) : Parser(inputPath) { };

	void advance() 
	{
		if(!has_more_lines()) {
			cerr << "Input doesn't have any more lines!\n";
			return;
		}

		m_curr_command = get_next_line();
		// cout << "Command: " << m_curr_command << "\n";

		parse_command();

		if(m_command_type != C_ARITHMETIC && m_command_type != C_RETURN) {
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
		
		if(m_command_type == C_GOTO || m_command_type == C_IF || m_command_type == C_LABEL)
			return m_arg2;

		return m_arg1;
	}

	int arg2()
	{
		if(m_command_type == C_PUSH || m_command_type == C_POP || m_command_type == C_FUNCTION || m_command_type == C_CALL) {
			try {
				return stoi(m_arg2);
			} catch(const exception& exception) {
				throw runtime_error("Invalid argument for parser::arg2(): " + m_arg2);
			}
		}

		throw runtime_error("arg2() must be called only if command_type is PUSH, POP, FUNCTION or CALL\n");
	}

private:
	string m_curr_command {};
	int m_command_type = -1;
	string m_arg1 {}, m_arg2 {};
	static unordered_map <string, int> m_command_map;

	void parse_command()
	{
		if(m_command_map.count(m_curr_command) != 0) {
			m_command_type = m_command_map[m_curr_command];
		}
		else {
			string first_word = m_curr_command.substr(0, m_curr_command.find(' ', 0));

			if(m_command_map.count(first_word) != 0) {
				m_command_type = m_command_map[first_word];
			}
			else {
				cerr << "Invalid Command Type : " << first_word << endl;
			}
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


unordered_map <string, int> VMParser::m_command_map {
	{"add", C_ARITHMETIC},
	{"sub", C_ARITHMETIC},
	{"neg", C_ARITHMETIC},
	{"eq", C_ARITHMETIC},
	{"gt", C_ARITHMETIC},
	{"lt", C_ARITHMETIC},
	{"and", C_ARITHMETIC},
	{"or", C_ARITHMETIC},
	{"not", C_ARITHMETIC},
	{"return", C_RETURN},
	{"push", C_PUSH},
	{"pop", C_POP},
	{"function", C_FUNCTION},
	{"call", C_CALL},
	{"label", C_LABEL},
	{"goto", C_GOTO},
	{"if-goto", C_IF}
};