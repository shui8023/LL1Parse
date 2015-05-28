/*
 * =====================================================================================
 *
 *       Filename:  parse.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年05月27日 21时10分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shui(dubingyang), dubingyangxiyou@gmail.com
 *        Company:  Class 1201 of software engineering
 *
 * =====================================================================================
 */


#include "parse.h"
#include "grammar.h"

LLParse::LLParse()
{
	
}

LLParse::~LLParse()
{
	
}

LLParse::LLParse(std::vector<std::string> tokens)
{
	CLL1Grammar CLL1("test");
	ParseTable = CLL1.GetSymbolTable();
	NonTerminal = CLL1.GetNonTerminal();
	Terminal = CLL1.GetTerminal();
	StartSymbol = CLL1.GetStartSymbol();
	Tokens = tokens;
}

bool LLParse::parse(bool print)
{
		std::stack<std::string> ParseStack;
		ParseStack.push("#");
		ParseStack.push(StartSymbol);
		std::vector<std::string>::size_type Pos = 0;
		std::vector<std::string> Str;
		std::cout << "\n";
		while (Pos < Tokens.size()) {
			if(find(Terminal.begin(), Terminal.end(), ParseStack.top()) != Terminal.end()) {
				if (ParseStack.top() == Tokens[Pos]) {
					if (print) {
						std::cout<<"POP: " << ParseStack.top() << std::endl;
					}
					ParseStack.pop();
					Pos++;
				} else {
					std::cout<< ParseStack.top()<< ":"<<Tokens[Pos] << std::endl;
					return false;
				}
			} else if(ParseStack.top() == "#") {
				if (Tokens[Pos] == "#") {
					return true;
				} else {
					return false;
				}
			} else if (find(NonTerminal.begin(),NonTerminal.end(),ParseStack.top()) != NonTerminal.end()) {
				Str = Tokensize(ParseStack.top(),Tokens[Pos]);
				if (Str.size()) {
					if (print) {
						std::cout << "POP : "<<ParseStack.top()<<std::endl;
					}
					ParseStack.pop();
					if (print) {
						std::cout << "PUSH : ";
					}
					for (int i = Str.size() - 1; i >= 0; i--) {
						if (Str[i] != "@") {
							if (print) {
								std::cout << Str[i] << ".";
							}
							ParseStack.push(Str[i]);
						}
					}
					if (print) {
						std::cout << std::endl;
					}
				} else {
					return false;
				}
			}
		}
	return false;
}

std::vector<std::string> LLParse::Tokensize(std::string String, std::string String1) 
{
	std::vector<std::string> token;
	std::vector<std::string> Temp;
	std::vector<std::string> _Temp;
	std::string Str;
	std::string s;
	Temp = ParseTable[0];
	for (int i = 0; i < ParseTable.size(); i++) {
			_Temp.push_back(ParseTable[i][0]);
	}
	int iter;
	int _iter;
	for (int i = 0; i < Temp.size(); i++) {
		if(Temp[i]==String) {
			iter = i;
		}
	}
	for (int i = 0; i < _Temp.size(); i++) {
		if(_Temp[i]==String1) {
			_iter = i;
		}
	}
	Str = ParseTable[_iter][iter];
	if (Str == "") {
		return token;
	} else {
		std::string::size_type beginops = Str.find("->");
		Str = Str.substr(beginops+2);
		while (true) {
			beginops = Str.find_first_not_of(" \t");
			if (beginops != std::string::npos) {
				s = Str.substr(beginops, 2);
				if (IsNoterminal(s)) {
					token.push_back(s);
					Str = Str.substr(beginops+2);
				} else {
					s = Str.substr(beginops, 1);
					token.push_back(s);
					Str = Str.substr(beginops+1);
				}
			} else {
				break;
			}
		}
	}
	return token;
}

bool LLParse::IsNoterminal(std::string str)
{
	for (std::vector<std::string>::size_type k = 0; k < NonTerminal.size(); k++) {
		if (str == NonTerminal[k]) {
			return true;
		}
	}
	return false;
}
