#ifndef MAILLIST
#define MAILLIST

#include "List.h"
#include "Mail.h"

class MailList: public List<Mail>
{
public:

	bool MailList::delete_mail(const std::string &id)
	{
		int pos;
		int left_key = 0, right_key = counter - 1;

		if (search(id, pos, left_key, right_key))
		{
			list[pos]->lowerCounter();
			if (list[pos]->getCounter() <= 0) destroy(id);
			return true;
		}
		else return false;
	}
};
#endif