/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年05月26日 18时40分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shui(dubingyang), dubingyangxiyou@gmail.com
 *        Company:  Class 1201 of software engineering
 *
 * =====================================================================================
 */


#include "grammar.h"
#include "parse.h"

int main(int argc, char *argv[])
{
	char tmp[1];
	std::vector<std::string> input;
	std::ifstream in("parse.txt");
	if(in){
		while(in.get(tmp[0])){
			if (tmp[0] == '\n') {
				continue;
			}
			std::string line(tmp, 1);
			input.push_back(line);
		}		
		input.push_back("#");
		LLParse parser(input);
		if(parser.parse(true)){
			std::cout<<"Succesfull. :)"<<std::endl;
		}else{
			std::cout<<"Error : Not a valid instance of the language."<<std::endl;
		}
	}else{
		std::cout<<"File doesn't exist."<<std::endl;
	}
	return 0;
}


assfsdfsdaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaasfsdfsfasfsfsafsasfsfasffaaaaaaaaaaaaffsafdf
