#ifndef VISIBLETRAYLIST
#define VISIBLETRAYLIST

#include "ElemTray.h"
#include "TrayList.h"
#include "Date.h"
#include "List.h"
#include <map>

enum Filter{ date, body, read, unread, emissor, subject, recipients, none };

class VisibleTrayList: public List<ElemTray>
{
private:

	Filter active_order;
	TrayList* trayList;
	bool inverse_order;

	int lastPage;
	int page;

	std::map<Filter, std::string> keys;
	std::map<Filter, bool> filters;
	Date lower;
	Date upper;
	
	void change(int pos1, int pos2);
	void insert(ElemTray* elem);

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

	void filterPage();
	void reverse();

	void setInvert(bool invert){ inverse_order = invert; }
	void setFilterUnread() { filters[unread] = true; }
	void setFilterRead() { filters[read] = true; }

	void setFilter(std::string search, Filter field)
	{
		filters[field] = true;
		keys[field] = search;
	}

	void setFilterDate(char* up, char* low)
	{
		filters[date] = true;

		Date update = turnDate(up);
		Date lowdate = turnDate(low);

		lower = update;
		upper = lowdate;
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

	bool LastPage(){ return page == lastPage; }
	int getLastPage(){ return lastPage; }
	int getPage(){ return page; }
};
#endif