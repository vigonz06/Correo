#include "ElemTray.h"
#include "Manager.h"

bool ElemTray::load(std::ifstream &file)
{
	int number;
	std::string idMail;

	file >> idMail;
	
	if (!file.fail())
	{
		file >> read;
		
		if (!file.fail())
		{
			file >> number;
			
			if (!file.fail())
			{
				mail = Manager::getManager()->getMailList()->get(idMail);
				box = bool(number);
				setId();
				
				return true;
			}
			else return false;
		}
		else return false;
	}
	else return false;
}
	
void ElemTray::save(std::ofstream &file) const
{
	file << mail->getId() << " " << read << " " << box << std::endl;
}
