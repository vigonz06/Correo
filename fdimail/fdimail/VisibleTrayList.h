#ifndef VISIBLETRAYLIST
#define VISIBLETRAYLIST

#include "tElemTray.h"
#include "TrayList.h"
#include "Date.h"
#include "List.h"
#include <map>

/*----------------------------
Unordered list of tElemTrays (email references), with capacity to apply filters, orders and easily extendible.
The main method is refresh, which performs a sync (loading every element in the linked TrayList),
applies fliters, orders (bubble sort), and finally discards every element but those of the current page.

Setters are provided to change the filters and sort order applied.
Suborders may be achived by calling the order methods in the proper order (pun not intended).
------------------------------*/

enum Filter{ subject, date, emissor, recipients, body, read, unread, none };

class VisibleTrayList: public List<tElemTray>
{
private:

	Filter active_order;
	TrayList* trayList;
	bool inverse_order;

	int page;
	int lastPage;

	Date lower;
	Date upper;
	std::map<Filter, bool> filters;
	std::map<Filter, std::string> keys;

	void change(int pos1, int pos2);
	void insert(tElemTray* elem);

public:

	VisibleTrayList();

	void changeOrder(Filter order) { active_order = order; }

	void init(TrayList* trayList);
	void link(TrayList* trayList);
	void refresh();
	void close();
	void sync();

	template<typename Funct, typename K>
	void filterBy(Funct filter, K key);

	void filterByDate(Date lower, Date upper);
	void filterByRecipient(std::string key);
	void filterByEmissor(std::string key);
	void filterBySubject(std::string key);
	void filterByBody(std::string key);
	void filterByRead(bool is_read);

	template<typename Funct>
	void orderBy(Funct order);
	void orderBySubject();
	void orderByDate();

	void reverse();
	void filterPage();

	void setFilterRead() { filters[read] = true; }
	void setFilterUnread() { filters[unread] = true; }
	void setInvert(bool invert){ inverse_order = invert; }

	void setFilterDate(char* up, char* low)
	{
		filters[date] = true;

		Date update = turnDate(up);
		Date lowdate = turnDate(low);

		lower = update;
		upper = lowdate;
	}

	void setFilter(std::string search, Filter field)
	{
		filters[field] = true;
		keys[field] = search;
	}

	void closeFilter()
	{
		for (int i = subject; i < none; i++)
		{
			filters[Filter(i)] = false;
		}
	}

	void increasePage(){ page++; }
	void decreasePage(){ page--; }

	int getPage(){ return page; }
	int getLastPage(){ return lastPage; }
	bool LastPage(){ return page == lastPage; }
};
#endif