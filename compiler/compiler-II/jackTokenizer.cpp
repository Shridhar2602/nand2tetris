#include "jackTokenizer.h"

namespace tokenizer {

using namespace std;

JackTokenizer::JackTokenizer(ifstream& fileStream, bool verbose) {
	this->verbose = verbose;
	parse(fileStream);
	advance();
}

vector<Token> JackTokenizer::parse(ifstream& fileStream) {
	
	char ch {};
	token.type = BLANK;
	token.text = "";

	while(fileStream.get(ch)) {

		switch(ch)
		{
			case '/':
				if(token.type == BLANK) {
					token.type = SYMBOL;
					token.text += ch;
				}
				else if(token.type == SYMBOL) {
					token.type = COMMENT;
					token.text += ch;
				}
				else if(token.type == COMMENT_END) {
					token.text += ch;
					end_token();
				}
				break;
			
			case '*':
				if(token.type == BLANK) {
					token.type = SYMBOL;
					token.text += ch;
				}
				else if(token.type == SYMBOL) {
					token.type = MULTILINE_COMMENT;
					token.text += ch;
				}
				else if(token.type == MULTILINE_COMMENT) {
					if(token.text == "/*") {
						token.text += ch;
					}
					else if(fileStream.peek() == '/') {
						token.type = COMMENT_END;
						token.text += ch;
					}
				}
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(token.type == BLANK) {
					token.type = INT_CONST;
					token.text += ch;
				}
				else {
					token.text += ch;
				}
				break;

			case ' ':
				if(token.type == COMMENT || token.type == MULTILINE_COMMENT) {
					token.text += ch;
				}
				else if(token.type == STRING_CONST) {
					token.text += ch;	
				}
				else {
					end_token();
				}
				break;

			case '\t':
				break;
			case '\r':
				break;
			case '\n':
				if(token.type == COMMENT) {
					end_token();
				}
				else if(token.type == MULTILINE_COMMENT) {
					token.text += ch;
				}
				_lineNum++;
				break;

			case '(':
			case ')':
			case '{':
			case '}':
			case '[':
			case ']':
			case '.':
			case ',':
			case ';':
			case '+':
			case '-':
			case '&':
			case '|':
			case '<':
			case '>':
			case '=':
			case '~':
				if(token.type == COMMENT || token.type == MULTILINE_COMMENT) {
					token.text += ch;
				}
				else if(token.type != STRING_CONST) {
					end_token();
					token.text += ch;
					token.type = SYMBOL;
					end_token();
				}
				break;
			case '"':
				if(token.type == BLANK) {
					token.type = STRING_CONST;
				}
				else if(token.type == STRING_CONST) {
					end_token();
				}
				else {
					token.text += ch;
				}
				break;

			default:
				if(token.type == BLANK) {
					token.type = IDENTIFIER;
					token.text += ch;
				}
				else {
					token.text += ch;
				}
		}
	}

	return tokens;
}

void JackTokenizer::end_token() {

	if(token.type == COMMENT || token.type == COMMENT_END) {
		if(verbose)
			cout << "COMMENT " << token.text << endl;
		token.type = BLANK;
		token.text = "";
		return;
	}
	else if(token.type != BLANK) {

		token.keyword = NOTKEYWORD;
		if(token.type != STRING_CONST && token.type != INT_CONST && mapKeyword.count(token.text) != 0) {
			token.type = KEYWORD;
			token.keyword = mapKeyword[token.text];
		}

		token.lineNum = _lineNum;
		tokens.push_back(token);

		if(verbose) {
			cout << _lineNum << ": " << debugEnum[token.type] << ", " << token.text << endl;
		}
	}

	token.text = "";
	token.type = BLANK;
}

bool JackTokenizer::has_more_tokens() {
	return (tokenIdx < (int)tokens.size() - 1 ? true : false);
}

void JackTokenizer::advance() {
	token = tokens[tokenIdx];
	tokenIdx++;
}

enum tokenType JackTokenizer::tokenType() {
	return token.type;
}

enum keyWord JackTokenizer::keyWord() {
	if(token.type == KEYWORD)
		return token.keyword;
	return NOTKEYWORD;
}

char JackTokenizer::symbol() {
	if(token.type == SYMBOL)
		return token.text[0];
	return '0';
}

string JackTokenizer::identifier() {
	if(token.type == IDENTIFIER) 
		return token.text;
	return nullptr;
}

int JackTokenizer::intVal() {
	if(token.type == INT_CONST) {
		return stoi(token.text);
	}

	return -1;
}

string JackTokenizer::stringVal() {
	if(token.type == STRING_CONST) {
		return token.text;
	}

	return nullptr;
}

string JackTokenizer::text() {
	return token.text;
}

int JackTokenizer::lineNum() {
	return token.lineNum;
}

}	// namespace tokenizer