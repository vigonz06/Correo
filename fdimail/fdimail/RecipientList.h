#ifndef RECIPIENTLIST
#define RECIPIENTLIST

#include "List.h"
#include <string>
#include <fstream>
#include "GlobalConstants.h"

class RecipientList : public List<std::string>
{
private:

	bool full() { return counter == MAX_RECIPIENTS; }
	bool search(std::string name, int &pos);

public:

	bool insert(std::string* elem);
	void destroy(std::string name);
	bool load(std::ifstream &file);
	void save(std::ofstream &file);
};

#endif