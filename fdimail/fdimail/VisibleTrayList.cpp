#include "VisibleTrayList.h"
#include "tElemTray.h"
#include "TrayList.h"
#include "Date.h"

VisibleTrayList::VisibleTrayList() : List()
{
	init(nullptr);
}

void VisibleTrayList::init(TrayList* trayList)
{
	for (int i = subject; i <= body; i++)
	{
		filters[Filter(i)] = false;
	}
	
	active_order = none;
	inverse_order = false;

	link(trayList);
}

void VisibleTrayList::link(TrayList* linked)
{
	trayList = linked;
	page = 0;
}

void VisibleTrayList::refresh()
{
	sync();
	
	if (filters[recipients])filterByRecipient(keys[recipients]);
	if (filters[subject])	filterBySubject(keys[subject]);
	if (filters[emissor])	filterByEmissor(keys[emissor]);
	if (filters[date])		filterByDate(lower, upper);
	if (filters[body])		filterByBody(keys[body]);
	if (filters[unread])    filterByRead(false);
	if (filters[read])      filterByRead(true);
	

	switch (active_order)
	{
	case subject:
		orderBySubject();
		break;
	default:
		orderByDate();
		break;
	}

	if (inverse_order) reverse();
	lastPage = (length()-1)/MAILS_X_PAGE;
	filterPage();
}

void VisibleTrayList::sync()
{
	erase();

	for (int i = 0; i < trayList->length(); i++)
	{
		insert(trayList->operator[](i));
	}
}

template<typename Funct, typename K>
void VisibleTrayList::filterBy(Funct filter, K key)
{
	tElemTray** oldList = list;
	list = new tElemTray*[dim];
	int oldCounter = counter;
	counter = 0;

	for (int i = 0; i < oldCounter; i++)
	{
		if (filter(oldList[i], key)) insert(oldList[i]);
	}
	delete oldList;
}

void VisibleTrayList::filterByDate(Date lower, Date upper)
{
	filterBy([](tElemTray* a, Date key){ return key <= a->mail->getDate(); }, lower);
	filterBy([](tElemTray* a, Date key){ return a->mail->getDate() <= key; }, upper);
}

void VisibleTrayList::filterBySubject(std::string key)
{
	filterBy([](tElemTray* a, std::string key){ return a->mail->getSubject().find(key) != -1; }, key);
}

void VisibleTrayList::filterByBody(std::string key)
{
	filterBy([](tElemTray* a, std::string key){ return a->mail->getBody().find(key) != -1; }, key);
}

void VisibleTrayList::filterByEmissor(std::string key)
{
	filterBy([](tElemTray* a, std::string key){ return a->mail->getFrom().find(key) != -1; }, key);
}

void VisibleTrayList::filterByRecipient(std::string key)
{
	filterBy([](tElemTray* a, std::string key){ for (int i = 0; i < a->mail->getRecipients().length(); i++){ if (a->mail->getRecipients().operator[](i)->find(key) != -1) return true; } return false; }, key);
}

void VisibleTrayList::filterByRead(bool is_read)
{
	filterBy([](tElemTray* a, bool is_read) { return a->read == is_read; }, is_read);
}

template<typename Funct>
void VisibleTrayList::orderBy(Funct order)
{
	bool change_made;

	do
	{
		change_made = false;

		for (int i = 0; i < counter - 1; i++)
		{
			if (!order(list[i], list[i + 1]))
			{
				change(i, i + 1);
				change_made = true;
			}
		}
	} while (change_made);
}

void VisibleTrayList::orderByDate()
{
	orderBy([](tElemTray* a, tElemTray* b){ return a->mail->getDate() >= b->mail->getDate(); });
}

void VisibleTrayList::orderBySubject()
{
	orderBy([](tElemTray* a, tElemTray* b) { return a->mail->getSubject() <= b->mail->getSubject(); });
}

void VisibleTrayList::reverse()
{
	for (int i = 0; i < counter / 2; i++)
	{
		change(i, counter - i - 1);
	}
}

void VisibleTrayList::filterPage()
{
	if (page*MAILS_X_PAGE >= counter) page = 0;

	else if(page < 0) page = lastPage;

	if (dim > MAILS_X_PAGE)
	{
		int i;

		for (i = 0; i < MAILS_X_PAGE && MAILS_X_PAGE*page + i < counter; i++)
		{
			list[i] = list[MAILS_X_PAGE*page + i];
		}
		
		for (int k = i; k < counter; k++)
		{
			list[k] = nullptr;
		}
		counter = i;
	}
}

void VisibleTrayList::insert(tElemTray* elem)
{
	if (full()) resize(dim*(3 / 2) + 1);
	list[counter++] = elem;
	
}

void VisibleTrayList::change(int pos1, int pos2)
{
	assert(0 <= pos1 && pos1 < counter && 0 <= pos2 && pos2 < counter);
	tElemTray* aux = list[pos1];
	list[pos1] = list[pos2];
	list[pos2] = aux;
}