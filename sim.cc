/*
 * dis.cc
 *
 *  Created on: Feb 5, 2010
 *      Author: mbhinder
 */

#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
#include "sim.hh"
using namespace std;

map<int,instruction>& Sim::disassemble(std::string filename)
{
	int address = 596;
	std::fstream file(filename.c_str(), ios::in | ios::binary);
	if (file.is_open())
   	{
		while (file.good())
	   	{
			char * mem_block = new char[4];
			file.read(mem_block, 4);
			if (file.eof())
			{
				return ins_cache;
			}
			//put the characters into bitsets so as to get the string binary representation
			bitset<8> b1((int) mem_block[0]);
			bitset<8> b2((int) mem_block[1]);
			bitset<8> b3((int) mem_block[2]);
			bitset<8> b4((int) mem_block[3]);
			std::string binary_code;
			//get the string representation of each bitset
			binary_code   = b1.to_string<char, char_traits<char> , allocator<char> > ();
			binary_code  += b2.to_string<char, char_traits<char> , allocator<char> > ();
			binary_code  += b3.to_string<char, char_traits<char> , allocator<char> > ();
			binary_code  += b4.to_string<char, char_traits<char> , allocator<char> > ();
			print_code(binary_code,address);
			address += 4;
			//free up the memory block
			delete [] mem_block;
		}
	} else
   	{
		std::cout << "Unable to open file";
		return ins_cache;
	}
	file.close();
	return ins_cache;
}

int Sim::print_code(string binary_code, int address)
{
    instruction current_inst;
    std::ostringstream fout;
	string opcode = binary_code.substr(0,6);
	string bgez = binary_code.substr(11,5);
	string brk = binary_code.substr(26,6);
	string rd_check = binary_code.substr(16,5);
	string sa_check = binary_code.substr(21,5);
	if (brk_flag)
	{
		if (address > 700)
		{
			//fout<<binary_code<<" "<<address<<" ";
			current_inst.inst_type = "DATA";
			bitset<32> data(binary_code);
			if(data.test(31))
			{
				//flip the bits and then add one
				for (int i = 0 ; i < 32 ; i ++)
				{
					data.flip(i);
				}
				fout<< "-" << ( data.to_ulong() + 1  );
				current_inst.inst_queue.push(fout.str());
				fout.str("");
			}
			else
				fout<< data.to_ulong();
				current_inst.inst_queue.push(fout.str());
				fout.str("");
		}
		else
		{
			current_inst.inst_type = "EMPTY";
			fout << 0;
			current_inst.inst_queue.push(fout.str());
			fout.str("");
		}
		//cout<<fout.str();
		int size = current_inst.inst_queue.size();
		for (unsigned int i = 0; i < size; i++)
		{
			cout<< current_inst.inst_queue.front() << " " ;
			current_inst.inst_queue.pop();		
		}
		cout<<endl;
		return 0;
	}
	else if (opcode == "001000")
	{
		//format_print(binary_code);
		//fout<<" "<<address<< " "<< "ADDI" << " " ;
		current_inst.inst_type = "ADDI";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<< "R" << rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<< "R" << rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		string s = binary_code.substr(16,16);
		bitset<16> imm(s);
		//check if the signed bit is set or not
		if(imm.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				imm.flip(i);
			}
			fout<< "#-" << ( imm.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
			fout.str("");
		}
		else{
			fout<< "#" << imm.to_ulong();
			current_inst.inst_queue.push(fout.str());
			fout.str("");}
	}
	else if (opcode == "001001")
	{
		//format_print(binary_code);
		current_inst.inst_type = "ADDIU";
		//fout<<" "<<address<< " "<< "ADDIU" << " " ;
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<< "R" << rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<< "R" << rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		
		string s = binary_code.substr(16,16);
		bitset<16> imm(s);
		//check if the signed bit is set or not
		if(imm.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				imm.flip(i);
			}
			fout<< "#-" << ( imm.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
			fout.str("");
		}
		else{
			fout<< "#" << imm.to_ulong();
			current_inst.inst_queue.push(fout.str());
			fout.str("");}
	}
	else if (opcode == "101011")
	{
		current_inst.inst_type = "SW";
		//format_print(binary_code);
		//fout<<" "<<address<<" "<<"SW"<<" ";
		bitset<5> base(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		string s = binary_code.substr(16,16);
		bitset<16> offset(s);
		//check if the signed bit is set or not
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< ( offset.to_ulong() + 1  )<<"("<<"R"<<base.to_ulong()<<")";
			current_inst.inst_queue.push(fout.str());
			fout.str("");
		}
		else{
			fout<< offset.to_ulong() <<"("<<"R"<<base.to_ulong()<<")";
			current_inst.inst_queue.push(fout.str());
			fout.str("");}
	}
	else if (opcode == "100011")
	{
		current_inst.inst_type = "LW";
//		format_print(binary_code);
	//	fout<<" "<<address<<" "<<"LW"<<" ";
		bitset<5> base(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		string s = binary_code.substr(16,16);
		bitset<16> offset(s);
		//check if the signed bit is set or not
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< ( offset.to_ulong() + 1  )<<"("<<"R"<<base.to_ulong()<<")";
			current_inst.inst_queue.push(fout.str());
			fout.str("");
		}
		else{
			fout<< offset.to_ulong() <<"("<<"R"<<base.to_ulong()<<")";
			current_inst.inst_queue.push(fout.str());
			fout.str("");
			}
	}
	else if (opcode == "000010")
	{
		current_inst.inst_type = "J";
		//format_print(binary_code);
		//fout<<" "<<address<<" "<<"J"<<" ";
		bitset<28> target(binary_code.substr(6,26));
		//shift the target address left by two bits to get 28 bits
		//then get the upper four bits of the current address
		//concatenate the two to get the target address
		bitset<28> b = target<<2;
		bitset<32> add_bitset(address);
		string s1 = add_bitset.to_string<char, char_traits<char> , allocator<char> > ();
		string s2  = b.to_string<char, char_traits<char> , allocator<char> > ();
		string s3 = s1.substr(0,4);
	    string s4 = s3 + s2;
		bitset<32> final_address(s4);
		fout<<"#"<<final_address.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000100")
	{
		current_inst.inst_type = "BEQ";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<<" "<<"BEQ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
		   fout.str("");
		}
		else
		{
			fout<< "#" << offset.to_ulong();
			current_inst.inst_queue.push(fout.str());
		    fout.str("");
		}
	}
	else if (opcode == "000101")
	{
		current_inst.inst_type = "BEN";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<<" "<<"BEN"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		    fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		    fout.str("");
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
		    fout.str("");
		}
		else{
			fout<< "#" << offset.to_ulong();
			current_inst.inst_queue.push(fout.str());
		    fout.str("");}
	}
	else if (opcode == "000001" && bgez == "00001")
	{
		current_inst.inst_type = "BGEZ";
    	//~ format_print(binary_code);
		//~ fout<<" "<<address<<" "<<"BGEZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		    fout.str("");
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
		    fout.str("");
		}
		else{
			fout<< "#" << offset.to_ulong();
			current_inst.inst_queue.push(fout.str());
		    fout.str("");
			}
			
	}
	else if (opcode == "000111")
	{
		current_inst.inst_type = "BGTZ";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<<" "<<"BGTZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
		    fout.str("");
		}
		else {
			fout<< "#" << offset.to_ulong();
			current_inst.inst_queue.push(fout.str());
		    fout.str("");}
	}
	else if (opcode == "000110")
	{
		current_inst.inst_type = "BLEZ";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<<" "<<"BLEZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
		    fout.str("");
		}
		else {
			fout<< "#" << offset.to_ulong();
			current_inst.inst_queue.push(fout.str());
		    fout.str("");}
	}
	else if (opcode == "000001" && bgez == "00000")
	{
		current_inst.inst_type = "BLTZ";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<<" "<<"BLTZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
		fout.str("");
		}
		else {
			fout<< "#" << offset.to_ulong();
			current_inst.inst_queue.push(fout.str());
		fout.str("");
		}
	}
	else if (opcode == "000000" && brk == "001101")
	{
    	current_inst.inst_type = "BREAK";
		//format_print(binary_code);
		brk_flag = true;
	}
	else if (opcode == "000000" && brk == "101010")
	{
    	current_inst.inst_type = "SLT";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SLT" << endl ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	if (opcode == "001010")
	{
    	current_inst.inst_type = "SLTI";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SLTI" << " " ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<< "R" << rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<< "R" << rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		string s = binary_code.substr(16,16);
		bitset<16> imm(s);
		//check if the signed bit is set or not
		if(imm.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				imm.flip(i);
			}
			fout<< "#-" << ( imm.to_ulong() + 1  );
			current_inst.inst_queue.push(fout.str());
			fout.str("");
		}
		else{
			fout<< "#" << imm.to_ulong();
			current_inst.inst_queue.push(fout.str());
			fout.str(""); }
	}
	else if (opcode == "000000" && brk == "101011")
	{
		 current_inst.inst_type = "SLTU";
    	//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SLTU" << " " ;
		//~ current_inst.inst_type = "SLTI";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
					current_inst.inst_queue.push(fout.str());
			fout.str("");
		fout<<"R"<<rs.to_ulong();
					current_inst.inst_queue.push(fout.str());
			fout.str("");
		fout<<"R"<<rt.to_ulong();
					current_inst.inst_queue.push(fout.str());
			fout.str("");
	}
	// checking if the destination register is not zero
	// to differentiate from the lines after the break
	// statement
	// R0 is a reserved register and cannot be used as
	// the destination register
	else if (opcode == "000000" && brk == "000000" && (rd_check != "00000" || sa_check != "00000" || bgez != "00000"))
	{
		current_inst.inst_type = "SLL";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SLL" << " ";
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		bitset<5> sa(binary_code.substr(21,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"#"<<sa.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && brk == "000010" && rd_check != "00000")
	{
				current_inst.inst_type = "SRL";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SLL" << " ";
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		bitset<5> sa(binary_code.substr(21,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"#"<<sa.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && brk == "000011" && rd_check != "00000")
	{
					current_inst.inst_type = "SRA";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SLL" << " ";
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		bitset<5> sa(binary_code.substr(21,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"#"<<sa.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");

	}
	else if (opcode == "000000" && brk == "100010" && rd_check != "00000")
	{	
		current_inst.inst_type = "SUB";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && brk == "100011" && rd_check != "00000")
	{	
				current_inst.inst_type = "SUBU";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && brk == "100000" && rd_check != "00000")
	{	
		current_inst.inst_type = "ADD";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && brk == "100001" && rd_check != "00000")
	{	
				current_inst.inst_type = "ADDU";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
			
	}
	else if (opcode == "000000" && brk == "100100" && rd_check != "00000")
	{	
		current_inst.inst_type = "AND";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}	
	else if (opcode == "000000" && brk == "100101" && rd_check != "00000")
	{	
		current_inst.inst_type = "OR";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	
	}
	else if (opcode == "000000" && brk == "100110" && rd_check != "00000")
	{	
				current_inst.inst_type = "XOR";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && brk == "100111" && rd_check != "00000")
	{	
		current_inst.inst_type = "NOR";
		//~ format_print(binary_code);
		//~ fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rs.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
		fout<<"R"<<rt.to_ulong();
		current_inst.inst_queue.push(fout.str());
		fout.str("");
	}
	else if (opcode == "000000" && rd_check == "00000" && brk == "000000")
	{	
		current_inst.inst_type = "NOP";
		//format_print(binary_code);
		//fout<<" "<<address<< " "<< "NOP" <<endl;
	}
	cout<<current_inst.inst_type<< " ";
	int size = current_inst.inst_queue.size();
	for (unsigned int i = 0; i < size; i++)
	{
		cout<< current_inst.inst_queue.front() << " " ;
		current_inst.inst_queue.pop();		
	}
	cout<<endl;
	return 0;
}



Sim::Sim()
{
	brk_flag = false;
}
