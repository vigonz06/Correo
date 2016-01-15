#include "RecipientList.h"

bool RecipientList::search(std::string name, int &pos)
{
	for (pos = 0; pos < counter; pos++)
	{
		if (*list[pos] == name) return true;
	}
	return false;
}

bool RecipientList::insert(std::string* elem)
{
	if (!full())
	{
		list[counter] = elem;
		counter++;
		return true;
	}
	else return false;
}

void RecipientList::destroy(std::string name)
{
	int pos = 0;

	if (search(name, pos))
	{
		delete list[pos];
		list[pos] = nullptr;
		shiftLeft(pos);
		counter--;
	}
}

bool RecipientList::load(std::ifstream &file)
{
	int n;
	std::string* nuevo;

	file >> n;

	for (int i = 0; i < n; i++)
	{
		nuevo = new std::string;

		file >> *nuevo;

		if (!insert(nuevo))
		{
			delete nuevo;
			return false;
		}
	}
	return true;
}

void RecipientList::save(std::ofstream &file) const
{
	file << counter << std::endl;

	for (int i = 0; i < counter; i++)
	{
		file << list[i] << std::endl;
	}
}