/*
 * dis.cc
 *
 *  Created on: Feb 5, 2010
 *      Author: mbhinder
 */

#include <iostream>
#include <fstream>
#include <bitset>
#include "dis.hh"
using namespace std;

int Dis::disassemble(std::string filename, std::string outputfile)
{
	this->outputfile = outputfile;
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
				return 0;
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
		return 0;
	}
	file.close();
	return 1;
}

int Dis::print_code(string binary_code, int address)
{
	fstream fout(outputfile.c_str(), ios::out|ios::app);
	if ( !fout.is_open() )
	{
		std::cout<<"Unable to open file for writing";
		return 1;	
	}
	string opcode = binary_code.substr(0,6);
	string bgez = binary_code.substr(11,5);
	string brk = binary_code.substr(26,6);
	string rd_check = binary_code.substr(16,5);
	string sa_check = binary_code.substr(21,5);
	if (brk_flag)
	{
		if (address > 700)
		{
			fout<<binary_code<<" "<<address<<" ";
			bitset<32> data(binary_code);
			if(data.test(31))
			{
				//flip the bits and then add one
				for (int i = 0 ; i < 32 ; i ++)
				{
					data.flip(i);
				}
				fout<< "-" << ( data.to_ulong() + 1  )<<endl;
			}
			else
				fout<< data.to_ulong()<<endl;
		}
		else
		{
			fout<<binary_code<<" "<<address<<" "<<0<<endl;
		}
		return 0;
	}
	else if (opcode == "001000")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "ADDI" << " " ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<< "R" << rt.to_ulong()<< ", ";
		fout<< "R" << rs.to_ulong()<<", ";
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
			fout<< "#-" << ( imm.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << imm.to_ulong()<<endl;
	}
	else if (opcode == "001001")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "ADDIU" << " " ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<< "R" << rt.to_ulong()<< ", ";
		fout<< "R" << rs.to_ulong()<<", ";
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
			fout<< "#-" << ( imm.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << imm.to_ulong()<<endl;
	}
	else if (opcode == "101011")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"SW"<<" ";
		bitset<5> base(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rt.to_ulong()<<", ";
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
			fout<< ( offset.to_ulong() + 1  )<<"("<<"R"<<base.to_ulong()<<")"<<endl;
		}
		else
			fout<< offset.to_ulong() <<"("<<"R"<<base.to_ulong()<<")"<<endl;
	}
	else if (opcode == "100011")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"LW"<<" ";
		bitset<5> base(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rt.to_ulong()<<", ";
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
			fout<< ( offset.to_ulong() + 1  )<<"("<<"R"<<base.to_ulong()<<")"<<endl;
		}
		else
			fout<< offset.to_ulong() <<"("<<"R"<<base.to_ulong()<<")"<<endl;
	}
	else if (opcode == "000010")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"J"<<" ";
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
		fout<<"#"<<final_address.to_ulong()<<endl;
	}
	else if (opcode == "000100")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"BEQ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<", ";
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << offset.to_ulong()<<endl;
	}
	else if (opcode == "000101")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"BEN"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<", ";
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << offset.to_ulong()<<endl;
	}
	else if (opcode == "000001" && bgez == "00001")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"BGEZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong()<<", ";
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << offset.to_ulong()<<endl;
	}
	else if (opcode == "000111")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"BGTZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong()<<", ";
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << offset.to_ulong()<<endl;
	}
	else if (opcode == "000110")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"BLEZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong()<<", ";
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << offset.to_ulong()<<endl;
	}
	else if (opcode == "000001" && bgez == "00000")
	{
		format_print(binary_code);
		fout<<" "<<address<<" "<<"BLTZ"<<" ";
		bitset<5> rs(binary_code.substr(6,5));
		fout<<"R"<<rs.to_ulong()<<", ";
		bitset<16> offset(binary_code.substr(16,16));
		offset<<=2;
		if(offset.test(15))
		{
			//flip the bits and then add one
			for (int i = 0 ; i < 16 ; i ++)
			{
				offset.flip(i);
			}
			fout<< "#-" << ( offset.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << offset.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "001101")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "BREAK" << endl ;
		brk_flag = true;
	}
	else if (opcode == "000000" && brk == "101010")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SLT" << endl ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	if (opcode == "001010")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SLTI" << " " ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		fout<< "R" << rt.to_ulong()<< ", ";
		fout<< "R" << rs.to_ulong()<<", ";
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
			fout<< "#-" << ( imm.to_ulong() + 1  )<<endl;
		}
		else
			fout<< "#" << imm.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "101011")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SLTU" << " " ;
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	// checking if the destination register is not zero
	// to differentiate from the lines after the break
	// statement
	// R0 is a reserved register and cannot be used as
	// the destination register
	else if (opcode == "000000" && brk == "000000" && (rd_check != "00000" || sa_check != "00000" || bgez != "00000"))
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SLL" << " ";
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		bitset<5> sa(binary_code.substr(21,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<", ";
		fout<<"#"<<sa.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "000010" && rd_check != "00000")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SRL" << " ";
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		bitset<5> sa(binary_code.substr(21,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<", ";
		fout<<"#"<<sa.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "000011" && rd_check != "00000")
	{
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SRA" << " ";
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		bitset<5> sa(binary_code.substr(21,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<", ";
		fout<<"R"<<sa.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100010" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SUB" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100011" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "SUBU" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100000" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "ADD" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100001" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "ADDU" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100100" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "AND" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}	
	else if (opcode == "000000" && brk == "100101" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "OR" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100110" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "XOR" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && brk == "100111" && rd_check != "00000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "NOR" << " ";
		bitset<5> rs(binary_code.substr(6,5));
		bitset<5> rt(binary_code.substr(11,5));
		bitset<5> rd(binary_code.substr(16,5));
		fout<<"R"<<rd.to_ulong()<<", ";
		fout<<"R"<<rs.to_ulong()<<", ";
		fout<<"R"<<rt.to_ulong()<<endl;
	}
	else if (opcode == "000000" && rd_check == "00000" && brk == "000000")
	{	
		format_print(binary_code);
		fout<<" "<<address<< " "<< "NOP" <<endl;
	}
	fout.close();
	return 0;
}


/*
 * Print the assembly language code neatly
 * with spaces
 */
void Dis::format_print(string binary_code)
{
	fstream fout(outputfile.c_str(), ios::out|ios::app);
	if ( !fout.is_open() )
	{
		std::cout<<"Unable to open file for writing";
		return ;	
	}
	fout<<binary_code.substr(0,6)<<" "<<binary_code.substr(6,5)
			<<" "<<binary_code.substr(11,5)<<" "<<binary_code.substr(16,5)
			<<" "<<binary_code.substr(21,5) <<" "<<binary_code.substr(26,6);
	fout.close();
}

Dis::Dis()
{
	brk_flag = false;
}
