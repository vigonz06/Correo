#include "ElemTray.h"
#include "TrayList.h"
#include "Manager.h"

bool TrayList::search(const std::string &id, int &pos)
{
	for (pos = 0; pos < counter && list[pos]->getId() != id; pos++);

	return (pos < counter) ? true : false;
}

ElemTray* TrayList::get(const std::string &id)
{
	int pos = 0;
	return (search(id, pos)) ? list[pos] : nullptr;
}

void TrayList::insert(ElemTray * const elem)
{
	if (full()) resize(dim + 1);
	list[counter++] = elem;
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

void TrayList::save(std::ofstream &file)const
{
	file << counter << std::endl;

	for (int i = 0; i < counter; i++)
	{
		list[i]->save(file);
	}
}

void TrayList::load(std::ifstream &file)
{
	int n;

	file >> n;

	for (int i = 0; i < n; i++)
	{
		ElemTray* elem = new ElemTray();

		elem->load(file);

		if (elem->mail != nullptr) insert(elem);
	}
}

bool TrayList::readMail(const std::string &idMail)
{
	ElemTray* elem = get(idMail);

	if (elem != nullptr)
	{
		elem->read = true;
		return true;
	}
	else return false;
}