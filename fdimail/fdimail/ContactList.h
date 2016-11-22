#ifndef CONTACTLIST
#define CONTACTLIST

#include "tContact.h"
#include "List.h"

/*----------------------------
Derived class from list, responsible
for work whith the alias an user has
------------------------------*/

class ContactList : public List < tContact>
{
public:

	ContactList(){}
	ContactList(std::string userID);

	void changeMe(std::string new_name);
	std::string SearchFastName(std::string &name);

	void load(std::ifstream &file);
	void save(std::ofstream &file)const;
};

#endif
