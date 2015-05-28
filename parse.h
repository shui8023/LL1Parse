/*
 * =====================================================================================
 *
 *       Filename:  parse.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年05月27日 20时41分37秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shui(dubingyang), dubingyangxiyou@gmail.com
 *        Company:  Class 1201 of software engineering
 *
 * =====================================================================================
 */

#ifndef PARSE_H
#define PARSE_H


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <utility>
#include <algorithm>
#include <iomanip>
#include <stack>

class LLParse{
private:
	std::string StartSymbol;
	std::vector<std::string> NonTerminal;
	std::vector<std::string> Terminal;
	std::vector<std::string> Tokens;
	std::vector<std::vector<std::string> > ParseTable;
	std::vector<std::string> Tokensize(std::string String, std::string String1);
	bool IsNoterminal(std::string);
public:
	LLParse();
	~LLParse();
	LLParse(std::vector<std::string>);
	bool parse(bool print=true);
};
#endif
