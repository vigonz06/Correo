#include "tElemTray.h"
#include "TrayList.h"
#include "Manager.h"

tElemTray* TrayList::get(const std::string &id)
{
	int pos = 0;
	return (search(id, pos)) ? list[pos] : nullptr;
}

bool TrayList::search(const std::string &id, int &pos)
{
	for (pos = 0; pos < list->size() && list[pos]->getId() != id; pos++);

	return (pos < list->size()) ? true : false;
}

void TrayList::insert(tElemTray * const elem)
{
	list->push_back(elem);
}

bool TrayList::destroy(const std::string &id)
{
	int pos;
	if (search(id, pos))
	{
		delete list[pos];
		shiftLeft(pos);
		return true;
	}
	else return false;
}

bool TrayList::pop(const std::string id)
{
	int pos;
	if (search(id, pos))
	{
		list[pos] = nullptr;
		shiftLeft(pos);
		return true;
	}
	else return false;
}

void TrayList::load(std::ifstream &file)
{
	int n;

	file >> n;

	for (int i = 0; i < n; i++)
	{
		tElemTray* elem = new tElemTray();

		elem->load(file);

		if (elem->mail != nullptr) insert(elem);
	}
}

void TrayList::save(std::ofstream &file)const
{
	file << list->size()<< std::endl;

	for (auto i: list)
	{
		i->save(file);
	}
}

bool TrayList::readMail(const std::string &idMail)
{
	tElemTray* elem = get(idMail);

	if (elem != nullptr)
	{
		elem->read = true;
		return true;
	}
	else return false;
}