#ifndef SHA1_HPP
#define SHA1_HPP

#include <fstream>
#include <string>

class Encript
{
private:

	typedef unsigned long long uint64;
	typedef unsigned long int uint32;

	static const unsigned int BLOCK_INTS  = 16;
	static const unsigned int BLOCK_BYTES = 64;
	static const unsigned int DIGEST_INTS = 5;

	uint32 digest[DIGEST_INTS];
	std::string buffer;
	uint64 transforms;

	void transform(uint32 block[BLOCK_BYTES]);
	void update(std::istream &is);
	void reset();

	static void buffer_to_block(const std::string &buffer, uint32 block[BLOCK_BYTES]);
	static void read(std::istream &is, std::string &s, int max);
	static std::string from_file(const std::string &filename);

public:

	Encript();

	void update(const std::string &s);
	std::string final();
};

std::string sha1(const std::string &string);

#endif