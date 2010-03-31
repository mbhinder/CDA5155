/*
 * main.cc
 *
 *  Created on: Feb 5, 2010
 *      Author: mbhinder
 */
#include <iostream>
#include "dis.hh"
#include "sim.hh"
#include <stdio.h>
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 4)
	{
		std::cout<< "Valid usage : MIPSsim inputfilename outputfilename operation [-Tm:n]"<<endl; 
		return 0;
	}
	string inputfile = argv[1];
	string outputfile = argv[2];
	string operation = argv[3];
	if ( operation == "dis" )
	{
		remove(outputfile.c_str()); 
		Dis d;
		d.disassemble(inputfile, outputfile);
	}
	else if ( operation == "sim" )
	{
		remove(outputfile.c_str()); //delete existing file
		Sim s;
		s.disassemble(inputfile);
	}
	else
	{
		cout<<"Invalid operation"<<endl;
		return 0;
	}
}
