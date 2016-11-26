#ifndef VISIBLETRAYLIST
#define VISIBLETRAYLIST

#include "TrayList.h"
#include "List.h"
#include <map>

enum Filter{ date, body, read, unread, emissor, subject, recipients, none };

class VisibleTrayList: public List<ElemTray>
{
private:

	Filter activeOrder;
	TrayList* trayList;
	bool inverseOrder;

	int lastPage;
	int page;

	std::map<Filter, std::string> keys;
	std::map<Filter, bool> filters;
	time_t lower;
	time_t upper;
	
	void change(int pos1, int pos2);
	void insert(ElemTray* elem);

public:

	VisibleTrayList();

	void changeOrder(Filter order) { activeOrder = order; }

	void init(TrayList* trayList);
	void link(TrayList* trayList);
	void refresh();
	void close();
	void sync();

	template<typename Funct, typename K>
	void filterBy(Funct filter, K key);

	void filterByDate(time_t lower, time_t upper);
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

	void setInvert(bool invert){ inverseOrder = invert; }
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

		time_t update = turnDate(up);
		time_t lowdate = turnDate(low);

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