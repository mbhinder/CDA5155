/*
 * dis.hh
 *
 *  Created on: Feb 5, 2010
 *      Author: mbhinder
 */

#ifndef DIS_HH_
#define DIS_HH_

class Dis {
public:
	int disassemble(std::string, std::string );
	int print_code(std::string , int);
	void format_print(std::string);
	Dis();
private:
	bool brk_flag;
	std::string outputfile;
};

#endif /* DIS_HH_ */
