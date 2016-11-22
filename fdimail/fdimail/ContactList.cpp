#include "ContactList.h"

ContactList::ContactList(std::string userID): List()
{
	tContact* me = new tContact(userID, "Me");
	insert(me);
}

void ContactList::changeMe(std::string new_name)
{
	get("Me")->setAddress(new_name);
}

std::string ContactList::SearchFastName(std::string &name)
{
	if (name != "")
	{
		for (char j: name)
		{
			if (j == '@') return name;
		}

		tContact* contact = get(name);
		if (contact != nullptr) return contact->getAddress();
		else return name + "@fdimail.com";
	}
	return name;
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

void ContactList::save(std::ofstream &file)const
{
	file << counter << std::endl;

	for (int i = 0; i < counter; i++)
	{
		list[i]->save(file);
	}
}