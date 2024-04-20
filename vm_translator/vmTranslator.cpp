#include "vmParser.cpp"
#include "codeWriter.cpp"
#include <sys/stat.h>
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

struct file {
	string path;
	string name;

	file(string p, string n) {
		path = p;
		name = n;
	}
};

string get_files(string path, vector<file>& fileList);

int main(int argc, char* argv[]) 
{
	if(argc < 2) {
		cerr << "Must pass the path of input .vm file as command line argument\n";
		return -1;
	}

	string path = argv[1];
	string outputPath {};
	vector<file> fileList;

	outputPath = get_files(path, fileList);

	CodeWriter codeWriter = CodeWriter(outputPath);

	for(auto file : fileList) 
	{
		try {
			VMParser parser = VMParser(file.path);

			codeWriter.set_fileName(file.name);
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
				else if(command_type == C_LABEL) {
					codeWriter.write_label(parser.arg1());
				}
				else if(command_type == C_GOTO) {
					codeWriter.write_goto(parser.arg1());
				}
				else if(command_type == C_IF) {
					codeWriter.write_if_goto(parser.arg1());
				}
				else if(command_type == C_FUNCTION) {
					codeWriter.write_function(parser.arg1(), parser.arg2());
				}
				else if(command_type == C_CALL) {
					codeWriter.write_call(parser.arg1(), parser.arg2());
				}
				else if(command_type == C_RETURN) {
					codeWriter.write_return();
				}
			}

			codeWriter.write_end_loop();	
			parser.close();
		} 
		
		catch(const exception& exception) {
			codeWriter.close();
			cout << "Something went wrong\n";
		}
	}

	codeWriter.close();
	return 0;
}

string get_files(string path, vector<file>& fileList) {

	string outputPath {};
	string fileName {};
	struct stat s;
	if(stat(path.c_str(), &s) != -1) {
		if(S_ISDIR(s.st_mode)) {
			// a directory
			if(path.substr(path.size() - 1) != "/")
				path = path + "/";	
			fileName = path.substr(0, path.size() - 1);
			fileName = fileName.substr(fileName.find_last_of("/") + 1);
			outputPath = path + fileName;
			for (const auto & entry : fs::directory_iterator(path)) {
        		string curFile = entry.path();

				if(curFile.size() > 3 && curFile.substr(curFile.size() - 3) == ".vm") {
					string curPath = curFile.substr(0, curFile.size() - 3);
					fileName = curPath.substr(curPath.find_last_of("/") + 1);
					fileList.push_back(file (curFile, fileName));
				}
			}
		}
		else {
			// a file
			if(path.size() > 3 && path.substr(path.size() - 3) == ".vm") {
				outputPath = path.substr(0, path.size() - 3);
				fileName = outputPath.substr(outputPath.find_last_of("/") + 1);
				fileList.push_back(file (path, fileName));
			}
		}

		return outputPath;
	}
	else {
		cout << "Invalid file/folder\n";
		exit(-1);
	}
}