#include "vmParser.cpp"
#include "codeWriter.cpp"

string get_file_name(string path);

int main(int argc, char* argv[]) 
{
	if(argc < 2) {
		cerr << "Must pass the path of input .vm file as command line argument\n";
		return -1;
	}

	string filename = get_file_name(argv[1]);

	VMParser parser = VMParser(argv[1]);
	CodeWriter codeWriter = CodeWriter(filename);

	int command_type = -1;
	while(parser.has_more_lines()) 
	{
		parser.advance();
		command_type = parser.command_type();

		if(command_type == C_ARITHMETIC) {
			codeWriter.write_arithmetic(parser.arg1());
		}
		else if(command_type == C_PUSH){
			codeWriter.write_push(parser.arg1(), parser.arg2());
		}
		else if(command_type == C_POP){
			codeWriter.write_pop(parser.arg1(), parser.arg2());
		}
	}

	codeWriter.write_end_loop();
	parser.close();
	codeWriter.close();
	return 0;
}

string get_file_name(string path) 
{
	int dot_pos = path.find_last_of(".");
	if(path.substr(dot_pos) != ".vm") {
		cerr << "Input file must be a .vm file\n";
		exit(-1);
	}

	return path.substr(0, dot_pos);
}