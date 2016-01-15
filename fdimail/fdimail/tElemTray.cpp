#ifndef TELEMTRAY
#define TELEMTRAY

#include "tElemTray.h"
#include "Manager.h"

void tElemTray::save(std::ofstream &file) const
{
	file << mail->getId() << " " << read << std::endl;
}

void tElemTray::load(std::ifstream &file)
{
	std::string idMail;
	file >> idMail >> read;
	mail = Manager::getManager()->getMailList()->get(idMail);
}

#endif