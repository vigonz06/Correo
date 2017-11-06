#include "ContactList.h"

ContactList::ContactList(std::string userID): List()
{
	Contact* me = new Contact(userID, "Me");
	insert(me);
}

Contact* ContactList::getAdress(std::string name)
{
	int pos = 0;
	int ini = 0, fin = counter - 1;

	return searchAddress(name, pos, ini, fin) ? list[pos] : nullptr;
}

std::string ContactList::SearchFastName(std::string &name)
{
	if (name != "")
	{
		for (char j : name)
		{
			if (j == '@') return name;
		}

		Contact* contact = get(name);
		if (contact != nullptr) return contact->getAddress();
		else return name + "@fdimail.com";
	}
	return name;
}

bool ContactList::searchAddress(const std::string &id, int &pos, int &left_key, int &right_key) const
{
	if (left_key <= right_key)
	{
		pos = (left_key + right_key) / 2;

		if (list[pos]->getAddress() == id) return true;

		if (list[pos]->getAddress() < id) left_key = pos + 1;

		if (list[pos]->getAddress() > id) right_key = pos - 1;

		return searchAddress(id, pos, left_key, right_key);
	}
	else
	{
		pos = left_key;
		return false;
	}
}

void ContactList::load(std::ifstream &file)
{
	int n, i = 0;
	
	file >> n;

	while(i < n && !file.fail())
	{
		Contact* cont = new Contact();
		cont->load(file);
		insert(cont);
		i++;
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
