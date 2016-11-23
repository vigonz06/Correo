#ifndef CONTACTLIST
#define CONTACTLIST

#include "Contact.h"
#include "List.h"

class ContactList : public List < Contact>
{
public:

	ContactList(){}
	ContactList(std::string userID);
	Contact* getAdress(std::string name);
	bool full() { return counter == MAX_ELEMS; }
	bool searchAddress(const std::string &id, int &pos, int &left_key, int &right_key) const;

	void ChangeMe(std::string new_name);
	std::string SearchFastName(std::string &name);
	void changeAlias(std::string id, std::string new_name);

	void load(std::ifstream &file);
	void save(std::ofstream &file)const;
};

#endif
