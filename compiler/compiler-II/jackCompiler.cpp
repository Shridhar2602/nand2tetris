#include "jackTokenizer.h"
#include "compilationEngine.h"
#include "symbolTable.h"
#include "vmWriter.h"

#include <sys/stat.h>
#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

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
		cerr << "Must pass the path of input .jack file/folder as command line argument\n";
		return -1;
	}

	string path = argv[1];
	string outputPath {};
	vector<file> fileList;

	outputPath = get_files(path, fileList);

	for(auto file : fileList) {
		
		ifstream fileStream(file.path);
		if(!fileStream.is_open()) {
			cerr << "Can't open input file" << endl;
			return -1;
		}

		ofstream outStream(file.path.substr(0, file.path.size() - 5) + ".vm");
		if(!outStream.is_open()) {
			cerr << "Can't open output file" << endl;
			return -1;
		}

		ofstream outXMLStream(file.path.substr(0, file.path.size() - 5) + ".xml");
		if(!outStream.is_open()) {
			cerr << "Can't open output file" << endl;
			return -1;
		}

		// cout << "\n============= Compiling " << file.name << " =============\n" << endl;

		tokenizer::JackTokenizer jt = tokenizer::JackTokenizer(fileStream, false);
		vmWriter::VMWriter vmw = vmWriter::VMWriter(outStream);	
		CompilationEngine ce = CompilationEngine(outXMLStream, jt, vmw);

		ce.compileClass();

		fileStream.close();
		outStream.close();
	}

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

				if(curFile.size() > 3 && curFile.substr(curFile.size() - 5) == ".jack") {
					string curPath = curFile.substr(0, curFile.size() - 5);
					fileName = curPath.substr(curPath.find_last_of("/") + 1);
					fileList.push_back(file (curFile, fileName));
				}
			}
		}
		else {
			// a file
			if(path.size() > 3 && path.substr(path.size() - 5) == ".jack") {
				outputPath = path.substr(0, path.size() - 5);
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