#include <iostream>
#include <fstream>
#include <cstring>
// #include <cstdint>

//#include "big_consts.h"
#include "big_core.h"

void print_file(std::string filename)
{
	std::ifstream file(filename, std::ios_base::binary);
	char buff[8];
	file.read(buff, 8);

	for (int i = 0; i < 8; ++i)
	{
		int id;
		int length;
		char buffer[40];
		file.read((char*)&id, 8);
		file.read((char*)&length, 8);
		file.read((char*)&buffer, length);
		std::cout << "id: " << id << std::endl;
		std::cout << "length: " << length << std::endl;
		std::cout << "data: ";
		for (int j = 0; j < length; ++j)
			std::cout << uint64_t(buffer[j]);
		std::cout << "\n" << std::endl;
	}
	file.close();
}

void test_get_file_length()
{
	std::ifstream file("test.big", std::ios_base::binary);
	uint64_t length = BigCore::getFileLength(file);
	std::cout << "File length: " << length << std::endl;
}

void test_check_header()
{
	std::ifstream file("test.big", std::ios_base::binary);
	char buffer[8];
	if (BigCore::getFileLength(file) < 8)
	{
		std::cout << "File length is lesser than 8 bytes" << std::endl;
		return;
	}

	file.read(buffer, 8);
	if (BigCore::checkHeader(buffer))
		std::cout << "Header correctly written" << std::endl;
	else
		std::cout << "Header is wrong" << std::endl;
}


int main()
{
	/*
	std::string filename = "bigtest.big";
	std::ofstream ofile(filename, std::ios_base::binary);
	ofile.write(MAGIC, 8);
	a.i = 25;
	ofile.write(a.c, 4);
	ofile.close();

	std::ifstream ifile(filename, std::ios_base::binary);
	char buffer[8];
	ifile.read(buffer, 8);
	ifile.read(a.c, 8);
	ifile.close();
	std::cout << "integer: " << a.i << std::endl;
	*/

	// chunkIds k = DATA;
	// bool a = chunkIds.NUMBER_OF_IMAGES == 0;
	// chunkIds a = chunkIds[2];
	// std::cout << k << std::endl;
	// std::cout << k << std::endl;

	/*std::cout << "float " << sizeof(float) << std::endl;
	std::cout << "doube " << sizeof(double) << std::endl;
	std::cout << "char " << sizeof(char) << std::endl;
	std::cout << "unsigned char " << sizeof(unsigned char) << std::endl;
	std::cout << "short " << sizeof(short) << std::endl;
	std::cout << "unsigned short" << sizeof(unsigned short) << std::endl;
	std::cout << "int " << sizeof(int) << std::endl;
	std::cout << "unsigned int " << sizeof(unsigned int) << std::endl;
	std::cout << "long long " << sizeof(long long) << std::endl;
	std::cout << "unsigned long long " << sizeof(unsigned long long) << std::endl;
	std::cout << "bool " << sizeof(bool) << std::endl;*/

	uint64_t a[] = { 1, 2, 3, 4, 5 };
	char b[] = { 1, 2, 3, 4 };

// #ifdef DEBUG
	// BigCore c = BigCore(1, 1, 2, 2, 1, a, 4, (char*)&b);
	// c.writeFile("test.big");
// #endif
	// test_get_file_length();
	// test_check_header();
	// test_alignment();

	// BigCore c = BigCore(1, 1, 2, 2, 1, a, 4, 4, (char*)&b);
	// c.writeFile("test.big");
	// c.print();

	print_file("test.big");

	BigCore c = BigCore();
	c.readFile("test.big");
	c.print();

	return 0;
}
