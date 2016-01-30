#ifndef TELEMENTRAY
#define TELEMENTRAY

#include <string>
#include "Mail.h"

/*----------------------------
This struct is responsible for load and save the id of a mail
and its read status
------------------------------*/

enum Box { Inbox, Outbox };

struct tElemTray
{
	std::string id;
	Mail* mail;
	bool read;
	Box box;

	tElemTray() {}
	tElemTray(Mail* mail, Box box, bool read) : mail(mail), box(box), read(read) { setId(); }

	std::string getId() const { return id; }

	void setId();

	void load(std::ifstream &file);
	void save(std::ofstream &file) const;
};
#endif
