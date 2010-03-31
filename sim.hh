/*
 * sim.hh
 *
 *  Created on: Feb 5, 2010
 *      Author: mbhinder
 */

#ifndef sim_HH_
#define sim_HH_
#include <map>
#include <queue>
#include <iostream>
using namespace std;

struct instruction {
	string inst_type;
	queue<string> inst_queue;
};

class Sim {
public:
	map<int,instruction>& disassemble(std::string);
	int print_code(std::string , int);
	//void format_print(std::string);
	Sim();
private:
	map<int,instruction> ins_cache;
	bool brk_flag;
};

#endif /* sim_HH_ */
