#ifndef MAILLIST
#define MAILLIST

#include "GlobalConstants.h"
#include <string>
#include "List.h"
#include "Mail.h"

/*----------------------------
Derived class from list, responsible 
for work whith all mails
------------------------------*/

class MailList: public List<Mail>
{
public:

	bool MailList::delete_mail(const std::string &id)
	{
		int pos;
		int left_key = 0, right_key = list->size() - 1;

		if (search(id, pos, left_key, right_key))
		{
			list->operator[](pos)->lowerCounter();
			if (list->operator[](pos)->getCounter() <= 0) destroy(id);
			return true;
		}
		else return false;
	}
};
#endif