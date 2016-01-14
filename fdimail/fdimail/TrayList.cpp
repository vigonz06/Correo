#include "tElemTray.h"
#include "TrayList.h"
#include "Manager.h"
#include "checkML.h"

tElemTray* TrayList::get(const std::string &id)
{
	int pos = 0;
	if (search(id, pos)) return list[pos];
	else return nullptr;
}

bool TrayList::search(const std::string &id, int &pos)
{
	for (pos = 0; pos < counter && list[pos]->getId() != id; pos++);
	if (pos < counter) return true;
	else return false;
}

void TrayList::insert(tElemTray * const elem)
{
	if (full()) resize(dim*(3/2)+1);
	list[counter++] = elem;
}

bool TrayList::destroy(const std::string &id)
{
	int pos;
	if (search(id, pos))
	{
		delete list[pos];
		shiftLeft(pos);
		counter--;
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
		tElemTray* elem = new tElemTray();

		elem->load(file);

		if (elem->mail != nullptr) insert(elem);
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