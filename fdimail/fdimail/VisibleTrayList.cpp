#include "VisibleTrayList.h"

VisibleTrayList::VisibleTrayList() : List() { init(nullptr); }

void VisibleTrayList::change(int pos1, int pos2)
{
	ElemTray* aux = list[pos1];
	list[pos1] = list[pos2];
	list[pos2] = aux;
}

void VisibleTrayList::insert(ElemTray* elem)
{
	if (full()) resize(dim + 1);
	list[counter++] = elem;
}

void VisibleTrayList::init(TrayList* trayList)
{
	closeFilter();
	
	activeOrder = none;
	inverseOrder = false;

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
	

	switch (activeOrder)
	{
	case subject:
		orderBySubject();
		break;
	case date:
		orderByDate();
		break;
	}

	lastPage = (counter - 1) / MAILS_X_PAGE;
	if (inverseOrder) reverse();
	filterPage();
}

void VisibleTrayList::close()
{
	closeFilter();
	refresh();
	erase();
	link(nullptr);
}

void VisibleTrayList::sync()
{
	erase();

	for (int i = 0; i < trayList->size(); i++)
	{
		insert(trayList->operator[](i));
	}
}

template<typename Funct, typename K>
void VisibleTrayList::filterBy(Funct filter, K key)
{
	ElemTray** oldList = list;
	list = new ElemTray*[dim];
	int oldCounter = counter;
	counter = 0;

	for (int i = 0; i < oldCounter; i++)
	{
		if (filter(oldList[i], key)) insert(oldList[i]);
	}
	delete oldList;
}

void VisibleTrayList::filterByDate(time_t lower, time_t upper)
{
	filterBy([](ElemTray* a, time_t key){ return key <= a->mail->getDate(); }, lower);
	filterBy([](ElemTray* a, time_t key){ return key >= a->mail->getDate(); }, upper);
}

void VisibleTrayList::filterByRecipient(std::string key)
{
	filterBy([](ElemTray* a, std::string key){ for (auto i : a->mail->getRecipients()){ if (i.find(key) != -1) return true; } return false; }, key);
}

void VisibleTrayList::filterByEmissor(std::string key)
{
	filterBy([](ElemTray* a, std::string key){ return a->mail->getFrom().find(key) != -1; }, key);
}

void VisibleTrayList::filterBySubject(std::string key)
{
	filterBy([](ElemTray* a, std::string key){ return a->mail->getSubject().find(key) != -1; }, key);
}

void VisibleTrayList::filterByBody(std::string key)
{
	filterBy([](ElemTray* a, std::string key){ return a->mail->getBody().find(key) != -1; }, key);
}

void VisibleTrayList::filterByRead(bool is_read)
{
	filterBy([](ElemTray* a, bool is_read) { return a->read == is_read; }, is_read);
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

void VisibleTrayList::orderBySubject()
{
	orderBy([](ElemTray* a, ElemTray* b) { return a->mail->subSubject() <= b->mail->subSubject(); });
}

void VisibleTrayList::orderByDate()
{
	orderBy([](ElemTray* a, ElemTray* b){ return a->mail->getDate() >= b->mail->getDate(); });
}

void VisibleTrayList::filterPage()
{
	if (page*MAILS_X_PAGE >= counter) page = 0;

	else if (page < 0) page = lastPage;

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

void VisibleTrayList::reverse()
{
	for (int i = 0; i < counter / 2; i++)
	{
		change(i, counter - i - 1);
	}
}