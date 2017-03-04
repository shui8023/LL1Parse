dsdsdc*
 * =====================================================================================
 *
 *       Filename:  compile.hpp
 *
 *    Description:  LL（1）grammar
 *
 *        Version:  1.0
 *        Created:  2015年05月26日 10时57分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shui(dubingyang), dubingyangxiyou@gmail.com
 *        Company:  Class 1201 of software engineering
 *
 * =====================================================================================
 */

#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include <utility>
#include <algorithm>
#include <iomanip>
class CLL1Grammar{
private:
	bool IsLL1, FirstSetscomputed, FollowSetscomputed;
	/*
	 *A->B|C|D|@
	 *B->a|b|c|d
	 * A   B
	 *	   C
	 *	   D
	 *	   @
	 *B	   a
	 *	   b
	 *	   c
	 *	   d
	 */
	std::vector<std::string> NonTerminal;
	std::vector<std::string> Terminal;
	std::vector<std::string> _NonTerminal;		//非终结符
	std::map<std::string, std::vector<std::string> > NoGrammar;	//no deal with grammar 
	std::map<std::string, std::vector<std::string> > Grammar;		//have remove left recurison grammar
	std::map<std::string, std::vector<std::string> > FirstSets;	//first set
	std::map<std::string, std::vector<std::string> > FollowSets;	//follow set
	std::map<std::string, std::vector<std::string> > SelectTable;	//select table
	std::string StartSymbol;									//start symbol
	std::vector<std::string> TerminalSymbol;		//终结符表
	std::vector<std::vector<std::string> > SymbolTable;  //symbol tables
	bool RemoveLeftRecursion();									//left recurison
	std::vector<std::string> GetFirstsets(std::string);
	std::vector<std::string> GetFollowsets(std::string);
	bool IsNonterminal(std::string);
	bool IsTerminal(std::string);
	bool GetSelectTable();
	void GetTerminalSymbol();
	std::string FindSelectTable(std::string, std::string);
	std::vector<std::string>& ReturnFollowsets(std::string);
	std::vector<std::string>& ReturnFirstsets(std::string);
public:
	CLL1Grammar();
	CLL1Grammar(std::string);
	~CLL1Grammar();
	void PrintFirstsets(bool print = false);
	void PrintFollowsets(bool print = false);
	void PrintSelectTable(bool print = false);
	std::vector<std::vector<std::string> >& GetSymbolTable();
	std::vector<std::string>&GetNonTerminal();
	std::vector<std::string>&GetTerminal();
	std::string GetStartSymbol();
//	void GetSymbolTable();
}; 

#endif 
