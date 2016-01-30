#include "tElemTray.h"
#include "Manager.h"

void tElemTray::setId()
{
	id = mail->getId() + "_" + std::to_string(box);
}

void tElemTray::load(std::ifstream &file)
{
	std::string idMail;
	file >> idMail >> read;
	mail = Manager::getManager()->getMailList()->get(idMail);
}

void tElemTray::save(std::ofstream &file) const
{
	file << mail->getId() << " " << read << std::endl;
}
