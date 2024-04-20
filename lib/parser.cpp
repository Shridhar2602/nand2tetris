#include <iostream>
#include <string>
#include <fstream>
using namespace std;

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

	void close() 
	{
		inf.close();
	}

private:

	int init(string inputPath) 
	{
		inf.open(inputPath, std::ios::in);

		if(!inf) {
			cerr << "Input file could not be opened!\n";
			return 1;
		}

		return 0;
	}

protected:
	ifstream inf;
	string parsed {};

	string get_next_line() 
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