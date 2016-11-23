#ifndef TELEMENTRAY
#define TELEMENTRAY

#include <string>
#include "Mail.h"

/*----------------------------
This struct is responsible for load and save the id of a mail
and its read status
------------------------------*/

struct ElemTray
{
	std::string id;
	Mail* mail;
	bool read;
	bool box;

	ElemTray() {}
	ElemTray(Mail* mail, bool box, bool read) : mail(mail), box(box), read(read) { setId(); }

	const std::string getId() const { return id; }
	void setId() { id = mail->getId() + "_" + std::to_string(box); }

	void load(std::ifstream &file);
	void save(std::ofstream &file) const;
};
#endif
