#include "tElemTray.h"
#include "Manager.h"

void tElemTray::load(std::ifstream &file)
{
	std::string idMail;
	file >> idMail >> read >> box;
	mail = Manager::getManager()->getMailList()->get(idMail);
}

void tElemTray::save(std::ofstream &file) const
{
	file << mail->getId() << " " << read << " " << box << std::endl;
}
