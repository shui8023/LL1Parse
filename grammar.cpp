/*
 * =====================================================================================
 *
 *       Filename:  grammar.cpp
 *
 *    Description:  LL1 grammar
 *
 *        Version:  1.0
 *        Created:  2015年05月26日 14时39分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shui(dubingyang), dubingyangxiyou@gmail.com
 *        Company:  Class 1201 of software engineering
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include "grammar.h"

CLL1Grammar::CLL1Grammar()
{
	
}

CLL1Grammar::~CLL1Grammar()
{
	
}


//from the file read the grammar and rmove left recursion
CLL1Grammar::CLL1Grammar(std::string FileName) 
{
	IsLL1 = true;
	FirstSetscomputed = false;
	FollowSetscomputed = false;
	const char * _FileName = FileName.c_str();
	std::ifstream in(_FileName);
	std::string Line;
	std::string Symbol;
	std::string _NonTerminal;
	std::string _Terminal;
	std::vector<std::string> _TempTerminal;
	while (getline(in, Line)) {
		_TempTerminal.clear();
		std::string::size_type BeginOps = Line.find_first_not_of(" \t");
		if (BeginOps != std::string::npos) {
			//deal with the NonTerminal
			std::string::size_type EndOps = Line.find("->", BeginOps);
#ifdef DEBUG
//	std::cout << BeginOps << "\t" << EndOps << std::endl;
#endif 
			if (EndOps != std::string::npos) {
				_NonTerminal = Line.substr(BeginOps, EndOps-BeginOps);
#ifdef DEBUG
	std::cout << _NonTerminal << std::endl;
#endif
				BeginOps = _NonTerminal.find_first_of(" \t");
				if (BeginOps != std::string::npos) {
					_NonTerminal = _NonTerminal.substr(0,BeginOps);
				}
			} else {
				std::cout << "error\n";
				IsLL1 = false;
				return;
			}
			if (StartSymbol.empty()) {
				StartSymbol = _NonTerminal;
			}
			NonTerminal.push_back(_NonTerminal);
			Line = Line.substr(EndOps+2);
			//deal with terminal
			while (true) {
				BeginOps = Line.find_first_not_of(" \t");
				if (BeginOps != std::string::npos) {
					EndOps = Line.find("|", BeginOps);
					if (EndOps != std::string::npos) {
						_Terminal = Line.substr(BeginOps, EndOps);
						Line = Line.substr(EndOps+1);
					} else {
						_Terminal = Line.substr(BeginOps, Line.size());
						Line = Line.substr(Line.size());
					}
					Terminal.push_back(_Terminal);
					_TempTerminal.push_back(_Terminal);
#ifdef DEBUG
	std::cout << _Terminal << std::endl;
#endif
				} else {
					break;
				}
			} 
		} else {
			std::cout << "error\n";
			IsLL1 = false;
			return;
		}
		NoGrammar.insert(make_pair(_NonTerminal, _TempTerminal));
	}
	RemoveLeftRecursion();
}

bool CLL1Grammar::RemoveLeftRecursion()
{
	if (IsLL1) {
		bool IsLeftRecursion = false;
		std::string _RecursionSymbol;
		std::vector<std::string> TempNonTerminal;
		std::vector<std::string> TempTerminal;
		// remove the direct recursion
		//
		//A->A+G|F
		//G->i
		//
		//A->FA'
		//A'->+GA'|@
		//G->i
		for (std::string::size_type i = 0; i < NonTerminal.size(); i++) {
			IsLeftRecursion = false;
			TempNonTerminal.clear();
			TempTerminal.clear();
			_RecursionSymbol = NonTerminal[i] +"'";
			std::string::size_type TempNumber = NoGrammar[NonTerminal[i]].size();
			for (std::string::size_type k = 0; k < NoGrammar[NonTerminal[i]].size(); k++) {
				std::cout <<i<<":" <<k<<NoGrammar[NonTerminal[i]][k] << "\t";
				if(NoGrammar[NonTerminal[i]][k].substr(0,1) == NonTerminal[i]) {
					IsLeftRecursion = true;		
					TempNonTerminal.push_back(NoGrammar[NonTerminal[i]][k].substr(1)+_RecursionSymbol);
					NoGrammar[NonTerminal[i]].erase(NoGrammar[NonTerminal[i]].begin()+k);  //bug the iterator is no use
					k--;
				}
			}
			std::cout<<std::endl;
			if (IsLeftRecursion) {
				for (std::string::size_type j = 0; j < NoGrammar[NonTerminal[i]].size(); j++) {
					TempTerminal.push_back(NoGrammar[NonTerminal[i]][j]+_RecursionSymbol);
				}
				TempNonTerminal.push_back("@");
				Grammar.insert(make_pair(NonTerminal[i],TempTerminal));
				Grammar.insert(make_pair(_RecursionSymbol, TempNonTerminal));
			} else {
				Grammar.insert(make_pair(NonTerminal[i],NoGrammar[NonTerminal[i]]));
			}
		}
#ifdef DEBUG
		for (std::map<std::string, std::vector<std::string> >::iterator it = Grammar.begin(); it!=Grammar.end(); it++) {
			std::cout << "\n\n"<< (*it).first<< std::endl;
			for (std::vector<std::string>::size_type i = 0; i < (*it).second.size(); i++) {
				std::cout << (*it).second[i] << "\t";
			}
		}

#endif 
		// remove the indirect recursion reserver
		GetTerminalSymbol();
		PrintFirstsets(true);
		PrintFollowsets(true);
		PrintSelectTable(true);
		return true;
	}
	return false;
}

void CLL1Grammar::PrintFirstsets(bool print) 
{
	if (print) {
		std::cout <<"\n---------the first sets---------------\n";
		std::vector<std::string> Temp;
		for (std::map<std::string, std::vector<std::string> >::iterator it = Grammar.begin(); it!=Grammar.end(); it++) {
			std::cout << (*it).first << ":\t"; 
			Temp = GetFirstsets((*it).first);
			std::vector<std::string>::iterator iter;
			sort(Temp.begin(), Temp.end());
			iter = unique(Temp.begin(), Temp.end());
			if (iter != Temp.end()) {
				Temp.erase(iter, Temp.end());
			}
			for (std::vector<std::string>::size_type index = 0; index != Temp.size(); index++ ) {
				std::cout << Temp[index] << "\t";
			}
			FirstSets.insert(make_pair((*it).first, Temp));
			std::cout << std::endl;
		}
			std::cout <<"-------------------------------------\n";
	}
}

std::vector<std::string> CLL1Grammar::GetFirstsets(std::string NonTerminal)
{
	std::vector<std::string> NonTerminalFirstsets;
	std::vector<std::string> Temp;
	std::map<std::string, std::vector<std::string> >::iterator it;
	it = Grammar.find(NonTerminal);
	if (it != Grammar.end()) {
		for (std::vector<std::string>::size_type k = 0; k < (*it).second.size(); k++) {
			Temp = GetFirstsets((*it).second[k].substr(0,1));
			NonTerminalFirstsets.insert(NonTerminalFirstsets.begin(),Temp.begin(), Temp.end());
		}
	} else {
		NonTerminalFirstsets.push_back(NonTerminal.substr(0,1));
	}

	return NonTerminalFirstsets;
}

void CLL1Grammar::PrintFollowsets(bool print) 
{
	if (print) {
		std::cout <<"\n---------the follow sets---------------\n";
		std::vector<std::string> Temp;
		for (std::map<std::string, std::vector<std::string> >::iterator it = Grammar.begin(); it!=Grammar.end(); it++) {
			std::cout << (*it).first << ":\t"; 
			Temp = GetFollowsets((*it).first);
			std::vector<std::string>::iterator iter;
			sort(Temp.begin(), Temp.end());
			iter = unique(Temp.begin(), Temp.end());
			if (iter != Temp.end()) {
				Temp.erase(iter, Temp.end());
			}
			for (std::vector<std::string>::size_type index = 0; index != Temp.size(); index++ ) {
				std::cout << Temp[index] << "\t";
			}
			FollowSets.insert(make_pair((*it).first, Temp));
			std::cout << std::endl;
		}
			std::cout <<"-------------------------------------\n";
	}
}

std::vector<std::string> CLL1Grammar::GetFollowsets(std::string NonTerminal)
{
	std::vector<std::string> NonTerminalFollowsets;
	std::vector<std::string> Firstsets;
	std::vector<std::string> Temp;
	std::string::size_type BeginOps;
	std::string NextSymbol;
	std::string TempString;
	std::string String,NextString;
	if (NonTerminal == StartSymbol) {
		NonTerminalFollowsets.push_back("#");
	}
	for (std::map<std::string,std::vector<std::string> >::iterator it = Grammar.begin(); it != Grammar.end(); it++) {
		for (std::vector<std::string>::size_type k = 0; k < (*it).second.size(); k++) {
			String.clear();
			NextSymbol.clear();
			BeginOps = (*it).second[k].find(NonTerminal);
			if (BeginOps != std::string::npos) {
				String = (*it).second[k].substr(BeginOps,1);
				if ((BeginOps+1) < (*it).second[k].size()) {
					NextString = (*it).second[k].substr(BeginOps+1, 1);
					if (NextString == "'") {
						String = String + NextString;
					}
				}
			}
//			std::cout << "asd:" << String << "\t";
			if (String == NonTerminal) {
	//		std::cout << "Terminal:" << NonTerminal<< "\t";
	//			std::cout <<"terminal:"<< (*it).second[k]<<"\t";
				NextSymbol = (*it).second[k].substr(BeginOps+NonTerminal.size(), 1);
				if ((BeginOps+NonTerminal.size()+1) < (*it).second[k].size()) {
					String = (*it).second[k].substr(BeginOps+NonTerminal.size()+1, 1);
					if (!String.empty()) {
						if (String == "'") {
							NextSymbol = (*it).second[k].substr(BeginOps+NonTerminal.size(), 2);
						}
					}
				}
		//		std::cout << "Symbol:"<< NextSymbol<< "\t";
				if (NextSymbol.empty()) {		//no symbol
					if (NonTerminal != (*it).first) {
						Temp = GetFollowsets((*it).first);
						NonTerminalFollowsets.insert(NonTerminalFollowsets.begin(),Temp.begin(),Temp.end());
					}	
				} else {
					if (!IsNonterminal(NextSymbol)) {
						NonTerminalFollowsets.push_back(NextSymbol);
					} else {
						Firstsets = GetFirstsets(NextSymbol);
						std::vector<std::string>::iterator iter;
						sort(Firstsets.begin(), Firstsets.end());
						iter = unique(Firstsets.begin(), Firstsets.end());
						if (iter != Firstsets.end()) {
							Firstsets.erase(iter, Firstsets.end());
						}
						iter = find(Firstsets.begin(), Firstsets.end(), "@");
						if (iter != Firstsets.end()) {
							Firstsets.erase(iter);
							if (NonTerminal != (*it).first) {
								Temp = GetFollowsets((*it).first);
								NonTerminalFollowsets.insert(NonTerminalFollowsets.begin(),Temp.begin(),Temp.end());
							}	
						}
						NonTerminalFollowsets.insert(NonTerminalFollowsets.begin(),Firstsets.begin(),Firstsets.end());
					}
				}
			}
		}
	}

	return NonTerminalFollowsets;
}

bool CLL1Grammar::IsNonterminal(std::string Nonterminal)
{
	for (std::map<std::string,std::vector<std::string> >::iterator it = Grammar.begin(); it != Grammar.end(); it++) {
		if (Nonterminal == (*it).first) {
			return true;
		}
	}

	return false;
}
void CLL1Grammar::PrintSelectTable(bool print) 
{
	if (print) {
		GetSelectTable();
		std::cout <<"------------------the select table-------------------\n";
		for (std::map<std::string,std::vector<std::string> >::iterator it = SelectTable.begin(); it != SelectTable.end(); it++) {
			std::cout <<"\n"<<(*it).first << std::endl;
			for (std::vector<std::string>::size_type k = 0; k < (*it).second.size(); k++) {
				std::cout<<(*it).second[k]<< "\t" ;
			}
		}
		std::cout <<"\n------------------the select table-------------------\n";
	}
}
void CLL1Grammar::GetTerminalSymbol()
{
	std::string TempString;
	for (std::map<std::string, std::vector<std::string> >::iterator it = Grammar.begin(); it != Grammar.end(); it++) {
		for (std::vector<std::string>::size_type i = 0; i < (*it).second.size(); i++) {
			TempString = (*it).second[i];
			for (std::string::size_type j = 0; j < TempString.size(); j++) {
				if (TempString.substr(j, 1) != "'" && TempString.substr(j, 1) != "@") {
					if(!IsNonterminal(TempString.substr(j, 1))) {
						TerminalSymbol.push_back(TempString.substr(j, 1));	
		//				std::cout << TempString.substr(j, 1) << "\t";
					}
				} 
			}
		}
	}
	std::vector<std::string>::iterator iter;
	sort(TerminalSymbol.begin(), TerminalSymbol.end());
	iter = unique(TerminalSymbol.begin(), TerminalSymbol.end());
	if (iter != TerminalSymbol.end()) {
		TerminalSymbol.erase(iter, TerminalSymbol.end());
	}
	std::cout <<std::endl;
}
bool CLL1Grammar::IsTerminal(std::string _Terminal)
{
	for (std::vector<std::string>::size_type i = 0; i < TerminalSymbol.size(); i++) {
		if (TerminalSymbol[i] == _Terminal) {
			return true;
		}
	}
	return false;
}
bool CLL1Grammar::GetSelectTable() 
{
	if (IsLL1) {
		std::vector<std::string> Temp;
		std::vector<std::string> TempString;
		std::string String;
		for (std::map<std::string, std::vector<std::string> >::iterator it = Grammar.begin(); it != Grammar.end(); it++) {
			Temp = (*it).second;
			for (std::vector<std::string>::size_type i = 0; i < Temp.size(); i++) {
				TempString.clear();
				String = (*it).first + "->" + Temp[i];
				if (Temp[i] == "@") {
					TempString = ReturnFollowsets((*it).first);
				} else if (IsNonterminal(Temp[i].substr(0,1))) {
					TempString = ReturnFirstsets((*it).first);
				} else if (IsTerminal(Temp[i].substr(0,1))) {
					TempString.push_back(Temp[i].substr(0,1));
				}
				SelectTable.insert(make_pair(String, TempString));
			}
		}
		return true;
	}
	return false;
}

std::vector<std::string>& CLL1Grammar::ReturnFirstsets(std::string NonTerminal) 
{
	for (std::map<std::string, std::vector<std::string> >::iterator it = FirstSets.begin(); it != FirstSets.end(); it++) {
		if ((*it).first == NonTerminal) {
			return (*it).second;
		}
	}

}
std::vector<std::string>& CLL1Grammar::ReturnFollowsets(std::string NonTerminal)
{
	for (std::map<std::string, std::vector<std::string> >::iterator it = FollowSets.begin(); it != FollowSets.end(); it++) {
		if ((*it).first == NonTerminal) {
			return (*it).second;
		}
	}
}

std::vector<std::vector<std::string> >& CLL1Grammar::GetSymbolTable()
//void  CLL1Grammar::GetSymbolTable()
{
	std::vector<std::string> TempNonTerminal;
	TempNonTerminal.push_back(" ");
	std::vector<std::string> String;
	std::vector<std::string> TempString;
	for (std::map<std::string, std::vector<std::string> >::iterator it = Grammar.begin(); it != Grammar.end(); it++) {
		TempNonTerminal.push_back((*it).first);
		_NonTerminal.push_back((*it).first);
	}
	String = TerminalSymbol;
	String.push_back("#");
	SymbolTable.push_back(TempNonTerminal);
	for (std::vector<std::string>::size_type i = 0; i < String.size(); i++) {
		TempString.clear();
		TempString.push_back(String[i]);
		for (std::vector<std::string>::size_type j = 1; j < SymbolTable[0].size(); j++) {
			TempString.push_back(FindSelectTable(SymbolTable[0][j],String[i]));
		}
		SymbolTable.push_back(TempString);
	}

	for (int i = 0; i < SymbolTable.size(); i++) {
		for (int j = 0; j < SymbolTable[i].size(); j++) {
			std::cout <<std::setw(8)<<SymbolTable[i][j] << "\t|";
		}
		std::cout <<"\n";
	}
	
	return SymbolTable;
}

std::string CLL1Grammar::FindSelectTable(std::string String, std::string String1)
{
	for (std::map<std::string, std::vector<std::string> >::iterator it = SelectTable.begin(); it != SelectTable.end(); it++) { 
		if (String == (*it).first.substr(0,String.size())&&(*it).first.substr(String.size(),1) != "'") {
			for (std::vector<std::string>::size_type i = 0; i < (*it).second.size(); i++) {
				if (String1 == (*it).second[i]) {
					return (*it).first;
				}
			}
		}
	}
	return "";
}

std::vector<std::string>& CLL1Grammar::GetNonTerminal()
{
	return _NonTerminal;
}

std::vector<std::string>& CLL1Grammar::GetTerminal()
{
	return TerminalSymbol;
}
std::string CLL1Grammar::GetStartSymbol()
{
	return StartSymbol;
}
