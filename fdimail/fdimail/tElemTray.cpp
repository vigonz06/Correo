#include "tElemTray.h"
#include "Manager.h"

void tElemTray::setId()
{
	id = mail->getId() + "_" + std::to_string(box);
}

void tElemTray::load(std::ifstream &file)
{
	int number;
	std::string idMail;

	file >> idMail >> read >> number;

	mail = Manager::getManager()->getMailList()->get(idMail);
	box = Box(number);
	setId();
}

void tElemTray::save(std::ofstream &file) const
{
	file << mail->getId() << " " << read << " " << box << std::endl;
}
