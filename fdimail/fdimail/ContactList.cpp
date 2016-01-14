#include "ContactList.h"
#include "checkML.h"

ContactList::ContactList(std::string userID): List()
{
	tContact* me = new tContact(userID, "Me");
	insert(me);
}

std::string ContactList::SearchFastName(std::string &name)
{
	for (int j = 0; j < name.size(); j++)
	{
		if (name[j] == '@') return name;
	}

	tContact* contact = get(name);
	if (contact != nullptr) return contact->getAddress();
	else return name + "@fdimail.com";
}

void ContactList::changeMe(std::string new_name)
{
	get("Me")->user = new_name;
}

void ContactList::save(std::ofstream &file)const
{
	file << counter << std::endl;

	for (int i = 0; i < counter; i++)
	{
		list[i]->save(file);
	}
}

void ContactList::load(std::ifstream &file)
{
	int n;
	file >> n;

	for (int i = 0; i < n && !file.fail(); i++)
	{
		tContact* cont = new tContact();
		cont->load(file);
		insert(cont);
	}
}