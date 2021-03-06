#ifndef TCONTACT
#define TCONTACT

#include <sstream>
#include <fstream>
#include <iomanip>

class Contact
{
private:

	std::string user;
	std::string alias;

public:

	Contact(std::string realName, std::string fastName) : user(realName), alias(fastName) {}
	Contact() {}
	
	std::string getId() { return alias; }
	std::string getAddress() { return user; }
	void setAddress(std::string newUser) { user = newUser; }
	void setAlias(std::string newAlias) { alias = newAlias; }

	void load(std::ifstream &file)
	{
		file >> user >> alias;
	}

	void save(std::ofstream &file)const
	{
		file << user << " " << alias << std::endl;
	}

	std::string header()
	{
		std::ostringstream contact;

		contact << std::setw(36) << std::left << user << std::left << alias;

		return contact.str();
	}
};
#endif
