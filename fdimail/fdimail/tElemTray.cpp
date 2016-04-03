#include "tElemTray.h"
#include "Manager.h"

void tElemTray::load(std::ifstream &file)
{
	int number;
	std::string idMail;

	file >> idMail >> read >> number;

	mail = Manager::getManager()->getMailList()->get(idMail);
	box = bool(number);
	setId();
}

void tElemTray::save(std::ofstream &file) const
{
	file << mail->getId() << " " << read << " " << box << std::endl;
}
